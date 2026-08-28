// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MeasureReader.h"
#include "mx/api/ClefData.h"
#include "mx/api/DirectionData.h"
#include "mx/api/KeyData.h"
#include "mx/api/NoteData.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/AttributesChoice.h"
#include "mx/core/generated/Backup.h"
#include "mx/core/generated/BarStyleColor.h"
#include "mx/core/generated/Barline.h"
#include "mx/core/generated/Bookmark.h"
#include "mx/core/generated/Cancel.h"
#include "mx/core/generated/Clef.h"
#include "mx/core/generated/ClefGroup.h"
#include "mx/core/generated/ClefSign.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/Ending.h"
#include "mx/core/generated/Figure.h"
#include "mx/core/generated/FiguredBass.h"
#include "mx/core/generated/Forward.h"
#include "mx/core/generated/Grouping.h"
#include "mx/core/generated/Harmony.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/KeyAccidental.h"
#include "mx/core/generated/KeyChoice.h"
#include "mx/core/generated/Link.h"
#include "mx/core/generated/MeasureStyle.h"
#include "mx/core/generated/MeasureStyleChoice.h"
#include "mx/core/generated/Mode.h"
#include "mx/core/generated/MultipleRest.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NonTraditionalKeyGroup.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PositiveDivisions.h"
#include "mx/core/generated/Print.h"
#include "mx/core/generated/Repeat.h"
#include "mx/core/generated/RightLeftMiddle.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/Sound.h"
#include "mx/core/generated/StaffDetails.h"
#include "mx/core/generated/StaffDetailsGroup.h"
#include "mx/core/generated/StartStopDiscontinue.h"
#include "mx/core/generated/Step.h"
#include "mx/core/generated/StyleText.h"
#include "mx/core/generated/TraditionalKeyGroup.h"
#include "mx/core/generated/Transpose.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DirectionReader.h"
#include "mx/impl/IdFunctions.h"
#include "mx/impl/NoteFunctions.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/SoundFunctions.h"
#include "mx/impl/TimeReader.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <set>

namespace mx
{
namespace impl
{
namespace
{
api::FigureData parseFigure(const core::Figure &figure)
{
    api::FigureData figureData;

    if (figure.prefix().has_value())
    {
        figureData.prefix = figure.prefix()->value();
    }

    if (figure.figureNumber().has_value())
    {
        figureData.figureNumber = figure.figureNumber()->value();
    }

    if (figure.suffix().has_value())
    {
        figureData.suffix = figure.suffix()->value();
    }

    return figureData;
}

int getFiguredBassStaffIndex(const MeasureCursor &cursor, const api::MeasureData &measure,
                             const core::Note *nextNotePtr)
{
    auto staffIndex = cursor.staffIndex;

    if (nextNotePtr)
    {
        staffIndex = NoteReader{*nextNotePtr}.getStaffNumber() - 1;
    }

    if (staffIndex < 0 || staffIndex >= static_cast<int>(measure.staves.size()))
    {
        return 0;
    }

    return staffIndex;
}
} // namespace

MeasureReader::MeasureReader(const core::PartwiseMeasure &inPartwiseMeasureRef, const MeasureCursor &cursor,
                             const MeasureCursor &previousMeasureCursor)
    : myMutex{}, myPartwiseMeasure{inPartwiseMeasureRef}, myConverter{}, myOutMeasureData{}, myCurrentCursor{cursor},
      myPreviousMeasureCursor{previousMeasureCursor}, myHistory{}, myCrossStaffHomes{},
      myPreviousNoteBucketStaffIndex{-1}
{
    HistoryRecord initialCursorRecord;
    initialCursorRecord.amount = 0;
    initialCursorRecord.reason = "starting position";
    initialCursorRecord.timeBefore = cursor.tickTimePosition;
    initialCursorRecord.timeAfter = cursor.tickTimePosition;
    initialCursorRecord.cursorBefore = cursor;
    initialCursorRecord.cursorAfter = cursor;
    myHistory.emplace_back(std::move(initialCursorRecord));
}

void MeasureReader::addStavesToOutMeasure() const
{
    for (int i = 0; i < myCurrentCursor.getNumStaves(); ++i)
    {
        myOutMeasureData.staves.emplace_back(api::StaffData{});
    }
}

std::pair<api::MeasureData, std::optional<api::TransposeData>> MeasureReader::getMeasureData() const
{
    // lock due to the use of a mutable in const function
    // TODO - that's stupid, remove const designations
    std::lock_guard<std::mutex> lock(myMutex);
    myOutMeasureData = api::MeasureData{};
    myOutMeasureData.number = myPartwiseMeasure.number();
    myOutMeasureData.id = getId(myPartwiseMeasure);

    // if we are parsing the first measure of the part, then we need to return transpose information
    std::optional<api::TransposeData> transpose;

    if (myOutMeasureData.number == std::to_string(myCurrentCursor.measureIndex + 1))
    {
        myOutMeasureData.number = "";
    }

    if (myPartwiseMeasure.text().has_value())
    {
        myOutMeasureData.displayedNumber = myPartwiseMeasure.text()->value();
    }

    if (myPartwiseMeasure.width().has_value())
    {
        myOutMeasureData.width = static_cast<double>(myPartwiseMeasure.width()->value().value());
    }

    if (myPartwiseMeasure.implicit().has_value())
    {
        myOutMeasureData.implicit =
            myPartwiseMeasure.implicit()->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
    }

    addStavesToOutMeasure();
    parseTimeSignature();

    myCrossStaffHomes.clear();
    myPreviousNoteBucketStaffIndex = -1;
    scanForCrossStaffBeamGroups();

    const auto mdcSpan = myPartwiseMeasure.musicData();
    auto iter = mdcSpan.begin();
    const auto endIter = mdcSpan.end();

    for (; iter != endIter; ++iter)
    {
        const auto &mdc = *iter;
        const auto mdcIndex = static_cast<size_t>(iter - mdcSpan.begin());

        // incredibly, we need to know if the note following this one has a 'chord' tag
        // otherwise we don't know whether or not the current note if part of a chord,
        // which we need to know so that we can choose whether or not to increment the
        // current tick position in myCurrentCursor
        auto peekAheadAtNextNoteIter = iter + 1;

        while (mdc.isNote() && peekAheadAtNextNoteIter != endIter && !peekAheadAtNextNoteIter->isNote())
        {
            ++peekAheadAtNextNoteIter;
        }

        const core::Note *nextNotePtr = nullptr;
        if (peekAheadAtNextNoteIter != endIter && peekAheadAtNextNoteIter->isNote())
        {
            nextNotePtr = &peekAheadAtNextNoteIter->asNote();
        }

        auto maybeTranspose = parseMusicDataChoice(mdc, nextNotePtr, mdcIndex);
        if (myCurrentCursor.isFirstMeasureInPart && maybeTranspose.has_value())
        {
            transpose = maybeTranspose;
        }
    }

    myCurrentCursor.isFirstMeasureInPart = false;

    consolidateVoicesForAllStaves();

    // move the data to a temp then return the temp
    auto temp = api::MeasureData{std::move(myOutMeasureData)};
    myOutMeasureData = api::MeasureData{};
    return std::make_pair(temp, transpose);
}

impl::MeasureCursor MeasureReader::getCursor() const
{
    return myCurrentCursor;
}

void MeasureReader::parseTimeSignature() const
{
    // start from the carried-forward state; anything this measure does not restate stays implicit
    api::TimeChoice timeSignature;
    std::map<int, api::TimeChoice> staffTimeSignatures;

    if (myCurrentCursor.measureIndex > 0)
    {
        timeSignature = myPreviousMeasureCursor.timeSignature;
        staffTimeSignatures = myPreviousMeasureCursor.staffTimeSignatures;
    }

    // A carried-forward time signature is not the <time> element that stated it, so it keeps no id.
    timeSignature.isImplicit = true;
    timeSignature.id.reset();
    for (auto &entry : staffTimeSignatures)
    {
        entry.second.isImplicit = true;
        entry.second.id.reset();
    }

    TimeReader timeReader{myPartwiseMeasure.musicData()};
    for (const auto &found : timeReader.getTimeSignatures()) // isImplicit == false on each
    {
        int staffIndex = found.staffIndex;

        // clamp an out-of-range staff number to "all staves", mirroring the keys pattern
        if (staffIndex != api::INDEX_UNSPECIFIED && staffIndex > myCurrentCursor.getNumStaves() - 1)
        {
            staffIndex = api::INDEX_UNSPECIFIED;
        }

        if (staffIndex == api::INDEX_UNSPECIFIED)
        {
            // a restated unscoped time governs all staves: it supersedes carried-forward per-staff
            // overrides (but not overrides stated explicitly in this same measure)
            timeSignature = found.timeChoice;
            for (auto it = staffTimeSignatures.begin(); it != staffTimeSignatures.end();)
            {
                it = it->second.isImplicit ? staffTimeSignatures.erase(it) : std::next(it);
            }
        }
        else
        {
            staffTimeSignatures[staffIndex] = found.timeChoice;
        }
    }

    myOutMeasureData.timeSignature = timeSignature;
    myOutMeasureData.staffTimeSignatures = staffTimeSignatures;
    myCurrentCursor.timeSignature = std::move(timeSignature);
    myCurrentCursor.staffTimeSignatures = std::move(staffTimeSignatures);
    advanceTickTimePosition(0, "parseTimeSignature");
}

// see .h file for explanation of the return value
std::optional<api::TransposeData> MeasureReader::parseMusicDataChoice(const core::MusicDataChoice &mdc,
                                                                      const core::Note *nextNotePtr,
                                                                      size_t inMdcIndex) const
{
    // if we are parsing the first measure of the part, then we need to return transpose information
    std::optional<api::TransposeData> transpose;

    if (mdc.isNote())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseNote(mdc.asNote(), nextNotePtr, inMdcIndex);
    }
    else if (mdc.isBackup())
    {
        parseBackup(mdc.asBackup());
    }
    else if (mdc.isForward())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseForward(mdc.asForward());
    }
    else if (mdc.isDirection())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseDirection(mdc.asDirection());
        advanceTickTimePosition(0, "parseDirection");
    }
    else if (mdc.isAttributes())
    {
        myCurrentCursor.isBackupInProgress = false;
        transpose = parseAttributes(mdc.asAttributes());
        advanceTickTimePosition(0, "parseAttributes");
    }
    else if (mdc.isHarmony())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseHarmony(mdc.asHarmony());
        advanceTickTimePosition(0, "parseHarmony");
    }
    else if (mdc.isFiguredBass())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseFiguredBass(mdc.asFiguredBass(), nextNotePtr);
        advanceTickTimePosition(0, "parseFiguredBass");
    }
    else if (mdc.isPrint())
    {
        myCurrentCursor.isBackupInProgress = false;
        parsePrint(mdc.asPrint());
        advanceTickTimePosition(0, "parsePrint");
    }
    else if (mdc.isSound())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseSound(mdc.asSound());
        advanceTickTimePosition(0, "parseSound");
    }
    else if (mdc.isBarline())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseBarline(mdc.asBarline());
        advanceTickTimePosition(0, "parseBarline");
    }
    else if (mdc.isGrouping())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseGrouping(mdc.asGrouping());
        advanceTickTimePosition(0, "parseGrouping");
    }
    else if (mdc.isLink())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseLink(mdc.asLink());
        advanceTickTimePosition(0, "parseLink");
    }
    else if (mdc.isBookmark())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseBookmark(mdc.asBookmark());
        advanceTickTimePosition(0, "parseBookmark");
    }
    // isListening() — not handled, silently skip

    return transpose;
}

void MeasureReader::parseNote(const core::Note &inMxNote, const core::Note *nextNotePtr, size_t inMdcIndex) const
{
    bool isNextNotePartOfAChord = false;

    if (nextNotePtr)
    {
        NoteReader nextNoteReader{*nextNotePtr};
        isNextNotePartOfAChord = nextNoteReader.getIsChord();
    }

    myCurrentCursor.isBackupInProgress = false;
    impl::NoteReader noteReader{inMxNote};
    impl::NoteFunctions noteFunc{inMxNote, myCurrentCursor};
    auto noteData = noteFunc.parseNote();

    int noteDataStaffIndex = noteReader.getStaffNumber() - 1;

    if (noteDataStaffIndex < 0)
    {
        noteDataStaffIndex = 0;
    }

    // cross-staff notation (issue #289): a note whose beam group or chord spans staves is
    // filed into the unit's home staff so the unit stays adjacent in one voice, and the
    // divergent <staff> value is preserved in NoteData::crossStaffIndex
    int bucketStaffIndex = noteDataStaffIndex;

    if (noteReader.getIsChord() && myPreviousNoteBucketStaffIndex >= 0)
    {
        bucketStaffIndex = myPreviousNoteBucketStaffIndex;
    }
    else
    {
        const auto crossStaffHome = myCrossStaffHomes.find(inMdcIndex);
        if (crossStaffHome != myCrossStaffHomes.cend())
        {
            bucketStaffIndex = crossStaffHome->second;
        }
    }

    const bool isBucketSane =
        bucketStaffIndex >= 0 && static_cast<size_t>(bucketStaffIndex) < myOutMeasureData.staves.size();

    if (!isBucketSane)
    {
        bucketStaffIndex = noteDataStaffIndex;
    }
    else if (bucketStaffIndex != noteDataStaffIndex)
    {
        noteData.crossStaffIndex = noteDataStaffIndex;
    }

    myCurrentCursor.staffIndex = bucketStaffIndex;

    bool isThisNotePartOfAChord = noteData.isChord || isNextNotePartOfAChord;
    noteData.isChord = isThisNotePartOfAChord;

    if (!isThisNotePartOfAChord || !isNextNotePartOfAChord)
    {
        advanceTickTimePosition(noteData.durationData.durationTimeTicks, "note");
    }

    MX_ASSERT(bucketStaffIndex >= 0);
    MX_ASSERT(static_cast<size_t>(bucketStaffIndex) < myOutMeasureData.staves.size());
    myPreviousNoteBucketStaffIndex = bucketStaffIndex;
    insertNoteData(std::move(noteData), myCurrentCursor.staffIndex, myCurrentCursor.voiceIndex,
                   noteReader.getVoiceLabel());
}

void MeasureReader::scanForCrossStaffBeamGroups() const
{
    if (myCurrentCursor.getNumStaves() < 2)
    {
        return;
    }

    // an open level-1 beam group in one voice: the musicData indices of its non-chord,
    // non-grace member notes and the set of staves those members name
    struct OpenGroup
    {
        std::vector<size_t> memberIndices;
        std::set<int> staves;
        int beginStaffIndex = 0;
    };

    std::map<int, OpenGroup> openGroups; // key: backup-delimited voice ordinal
    std::map<int, int> stickyHomes;      // voice ordinal -> the voice's current home staff
    int voiceOrdinal = 0;
    bool isBackupInProgress = false;

    // a properly closed group that names more than one staff is a cross-staff group; its
    // home is the staff the voice was on when the group began (or, when the group is the
    // voice's first sighting, the staff of the group's first note)
    const auto closeGroup = [this, &stickyHomes](int inVoiceOrdinal, const OpenGroup &inGroup) {
        if (inGroup.staves.size() > 1)
        {
            const auto sticky = stickyHomes.find(inVoiceOrdinal);
            const int home = sticky == stickyHomes.cend() ? inGroup.beginStaffIndex : sticky->second;
            for (const auto memberIndex : inGroup.memberIndices)
            {
                myCrossStaffHomes[memberIndex] = home;
            }
            stickyHomes[inVoiceOrdinal] = home;
        }
        else if (!inGroup.staves.empty())
        {
            stickyHomes[inVoiceOrdinal] = *inGroup.staves.cbegin();
        }
    };

    const auto mdcSpan = myPartwiseMeasure.musicData();

    for (size_t mdcIndex = 0; mdcIndex < mdcSpan.size(); ++mdcIndex)
    {
        const auto &mdc = mdcSpan[mdcIndex];

        if (mdc.isBackup())
        {
            // mirror parseBackup's voice-ordinal rule
            if (!isBackupInProgress)
            {
                ++voiceOrdinal;
            }
            isBackupInProgress = true;
            continue;
        }

        if (mdc.isListening())
        {
            // mirror parseMusicDataChoice, which skips listening without touching the
            // backup-in-progress flag
            continue;
        }

        if (!mdc.isNote())
        {
            isBackupInProgress = false;
            if (mdc.isForward())
            {
                // a beam group cannot contain a time gap
                openGroups.erase(voiceOrdinal);
            }
            continue;
        }

        isBackupInProgress = false;
        const NoteReader reader{mdc.asNote()};

        if (reader.getIsGrace() || reader.getIsChord())
        {
            // grace notes beam among themselves; <chord> members follow the bucket of the
            // note they chord with (decided in parseNote), not the beam group scan
            continue;
        }

        int wireStaffIndex = reader.getStaffNumber() - 1;

        if (wireStaffIndex < 0)
        {
            wireStaffIndex = 0;
        }

        const auto &coreBeams = reader.getBeams();
        const auto beamOne = coreBeams.empty() ? api::Beam::unspecified : myConverter.convert(coreBeams.front());

        if (beamOne == api::Beam::begin)
        {
            OpenGroup group;
            group.beginStaffIndex = wireStaffIndex;
            group.memberIndices.push_back(mdcIndex);
            group.staves.insert(wireStaffIndex);
            openGroups[voiceOrdinal] = std::move(group);
            continue;
        }

        const auto openGroupIter = openGroups.find(voiceOrdinal);
        const bool continuesGroup =
            (beamOne == api::Beam::extend || beamOne == api::Beam::end) && openGroupIter != openGroups.cend();

        if (continuesGroup)
        {
            openGroupIter->second.memberIndices.push_back(mdcIndex);
            openGroupIter->second.staves.insert(wireStaffIndex);

            if (beamOne == api::Beam::end)
            {
                closeGroup(voiceOrdinal, openGroupIter->second);
                openGroups.erase(openGroupIter);
            }
            continue;
        }

        // an unbeamed note (or an orphan or hooked level-1 beam value) interrupts any open
        // group and moves the voice's home to its own staff
        openGroups.erase(voiceOrdinal);
        stickyHomes[voiceOrdinal] = wireStaffIndex;
    }
}

void MeasureReader::parseBackup(const core::Backup &inMxBackup) const
{
    if (!(myCurrentCursor.isBackupInProgress))
    {
        ++myCurrentCursor.voiceIndex;
    }

    myCurrentCursor.isBackupInProgress = true;

    // a chord cannot straddle a timeline jump
    myPreviousNoteBucketStaffIndex = -1;
    const int backupAmount = myCurrentCursor.convertDurationToGlobalTickScale(inMxBackup.duration());
    advanceTickTimePosition(-1 * backupAmount, "backup");

    if (myCurrentCursor.tickTimePosition < 0)
    {
        auto problemAmount = myCurrentCursor.tickTimePosition * -1;
        advanceTickTimePosition(problemAmount, "correct backup negative error");
    }
}

void MeasureReader::parseForward(const core::Forward &inMxForward) const
{
    // <forward>/<backup> are pure wire cursor mechanics with no api home (design principle:
    // store absolute tick positions, not running state), so their own optional <voice>/<staff>
    // children are deliberately not read here either. The writer regenerates whatever
    // <forward>/<backup> the tick math requires; it never has one to carry a voice/staff value
    // forward from, so a source's <forward>/<backup> voice/staff is expected to not round-trip.
    // a chord cannot straddle a timeline jump
    myPreviousNoteBucketStaffIndex = -1;
    const int forwardAmount = myCurrentCursor.convertDurationToGlobalTickScale(inMxForward.duration());
    advanceTickTimePosition(forwardAmount, "forward");
}

void MeasureReader::parseDirection(const core::Direction &inDirection) const
{
    DirectionReader reader{inDirection, myCurrentCursor};
    auto directionData = reader.getDirectionData();

    // make an adjustment if the directionData refers to a non-existent staff
    size_t staffIndex = 0;
    bool isStaffIndexSpecified = inDirection.staff().has_value();
    bool isStaffIndexInsane = false;

    if (isStaffIndexSpecified)
    {
        staffIndex = static_cast<size_t>(*inDirection.staff() - 1);
    }

    isStaffIndexInsane = staffIndex >= myOutMeasureData.staves.size();

    if (!isStaffIndexSpecified || isStaffIndexInsane)
    {
        staffIndex = 0;
        directionData.isStaffValueSpecified = false;
    }
    else
    {
        directionData.isStaffValueSpecified = true;
    }

    // in-case we made a mistake in the code above which calculates the staffIndex
    // make a final check to see if the staffIndex is in-bounds - throw if stupid
    MX_ASSERT(staffIndex < myOutMeasureData.staves.size());
    auto &staff = myOutMeasureData.staves.at(staffIndex);
    staff.directions.emplace_back(std::move(directionData));
}

std::optional<api::TransposeData> MeasureReader::parseAttributes(const core::Attributes &inMxAttributes) const
{
    // if we are parsing the first measure of the part, then we need to return transpose information
    std::optional<api::TransposeData> transpose;

    if (inMxAttributes.divisions().has_value())
    {
        const auto newDivisionsValueDecimal = inMxAttributes.divisions()->value().value();
        myCurrentCursor.ticksPerQuarter = static_cast<int>(std::ceil(newDivisionsValueDecimal - 0.5));
    }

    // TODO - continue work on measure numbering and style etc
    for (const auto &measureStyle : inMxAttributes.measureStyle())
    {
        if (measureStyle.choice().isMultipleRest())
        {
            const auto &mr = measureStyle.choice().asMultipleRest();
            myOutMeasureData.multiMeasureRest = mr.value();
            myOutMeasureData.multiMeasureRestUseSymbols = api::Bool::unspecified;
            if (mr.useSymbols().has_value())
            {
                myOutMeasureData.multiMeasureRestUseSymbols =
                    mr.useSymbols()->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
            }
        }
    }

    for (const auto &key : inMxAttributes.key())
    {
        const auto &keyChoiceObj = key.choice();
        const auto keyType = keyChoiceObj.kind();

        if (keyType == core::KeyChoice::Kind::nonTraditionalKey)
        {
            api::KeyData keyData;
            const auto &nonTraditionalKeyParts = keyChoiceObj.asNonTraditionalKey();
            for (const auto &nonTraditionalKeyPart : nonTraditionalKeyParts)
            {
                api::KeyComponent keyComponent{};

                if (nonTraditionalKeyPart.keyAccidental().has_value())
                {
                    keyComponent.accidental = myConverter.convert(nonTraditionalKeyPart.keyAccidental()->value());
                }

                const auto alter = nonTraditionalKeyPart.keyAlter().value().value();
                if (alter != 0.0)
                {
                    const auto semitoneAndCents = Converter::convertToSemitonesAndCents(alter);
                    keyComponent.alter = semitoneAndCents.first;
                    keyComponent.cents = semitoneAndCents.second;
                }

                keyComponent.step = myConverter.convert(nonTraditionalKeyPart.keyStep());
                keyData.nonTraditional.emplace_back(keyComponent);
            }

            keyData.id = getId(key);
            myOutMeasureData.keys.emplace_back(std::move(keyData));
            continue;
        }

        api::KeyData keyData;
        // TODO - do position attributes
        const auto &traditionalKey = keyChoiceObj.asTraditionalKey();

        if (key.number().has_value())
        {
            keyData.staffIndex = key.number()->value() - 1;
            if (keyData.staffIndex > myCurrentCursor.getNumStaves() - 1)
            {
                keyData.staffIndex = api::INDEX_UNSPECIFIED;
            }
        }

        keyData.fifths = traditionalKey.fifths().value();
        if (traditionalKey.cancel().has_value())
        {
            keyData.cancel = traditionalKey.cancel()->value().value();
            if (traditionalKey.cancel()->location().has_value())
            {
                keyData.cancelLocation = myConverter.convert(*traditionalKey.cancel()->location());
            }
        }

        if (traditionalKey.mode().has_value())
        {
            keyData.mode = myConverter.convert(*traditionalKey.mode());
        }
        keyData.tickTimePosition = myCurrentCursor.tickTimePosition;
        keyData.id = getId(key);

        myOutMeasureData.keys.emplace_back(std::move(keyData));
    }
    importStaffDetails(inMxAttributes);
    importClefs(inMxAttributes.clef());

    if (inMxAttributes.partSymbol().has_value())
    {
        const auto &ps = *inMxAttributes.partSymbol();
        api::PartSymbolData psData;
        psData.value = myConverter.convert(ps.value());
        if (ps.topStaff().has_value())
        {
            psData.topStaff = ps.topStaff()->value();
        }
        if (ps.bottomStaff().has_value())
        {
            psData.bottomStaff = ps.bottomStaff()->value();
        }
        myOutMeasureData.partSymbol = psData;
    }

    if (inMxAttributes.choice().isTranspose() && !inMxAttributes.choice().asTranspose().empty())
    {
        const auto &transposeVec = inMxAttributes.choice().asTranspose();

        // The very first transpose at measure 0/tick 0 is (still, unconditionally) treated as a
        // property of the part rather than the measure. This selection logic is unchanged.
        if (myCurrentCursor.measureIndex == 0 && myCurrentCursor.tickTimePosition == 0)
        {
            const auto &coreTranspose = transposeVec.front();
            transpose = Converter::convertToTransposeData(coreTranspose);
        }

        // Additionally and independently, every <transpose> element found anywhere is recorded
        // onto the measure's transpositions list.
        for (const auto &coreTranspose : transposeVec)
        {
            auto transposeData = Converter::convertToTransposeData(coreTranspose);

            if (coreTranspose.number().has_value())
            {
                transposeData.staffIndex = coreTranspose.number()->value() - 1;
                if (transposeData.staffIndex > myCurrentCursor.getNumStaves() - 1)
                {
                    transposeData.staffIndex = api::INDEX_UNSPECIFIED;
                }
            }

            transposeData.tickTimePosition = myCurrentCursor.tickTimePosition;
            transposeData.id = getId(coreTranspose);
            myOutMeasureData.transpositions.emplace_back(std::move(transposeData));
        }
    }
    return transpose;
}

void MeasureReader::parseHarmony(const core::Harmony &inHarmony) const
{
    DirectionReader reader{inHarmony, myCurrentCursor};
    auto directionData = reader.getDirectionData();

    // make an adjustment if the directionData refers to a non-existent staff
    size_t staffIndex = 0;
    bool isStaffIndexSpecified = inHarmony.staff().has_value();
    bool isStaffIndexInsane = false;

    if (isStaffIndexSpecified)
    {
        staffIndex = static_cast<size_t>(*inHarmony.staff() - 1);
    }

    isStaffIndexInsane = staffIndex >= myOutMeasureData.staves.size();

    if (!isStaffIndexSpecified || isStaffIndexInsane)
    {
        staffIndex = 0;
        directionData.isStaffValueSpecified = false;
    }
    else
    {
        directionData.isStaffValueSpecified = true;
    }

    MX_ASSERT(staffIndex < myOutMeasureData.staves.size());
    auto &staff = myOutMeasureData.staves.at(staffIndex);
    staff.directions.emplace_back(std::move(directionData));
}

void MeasureReader::parseFiguredBass(const core::FiguredBass &inMxFiguredBass, const core::Note *nextNotePtr) const
{
    if (myOutMeasureData.staves.empty())
    {
        return;
    }

    api::FiguredBassData figuredBass;

    for (const auto &figure : inMxFiguredBass.figure())
    {
        figuredBass.figures.emplace_back(parseFigure(figure));
    }

    if (inMxFiguredBass.parentheses().has_value())
    {
        figuredBass.parentheses =
            inMxFiguredBass.parentheses()->tag() == core::YesNo::Tag::yes ? api::Bool::yes : api::Bool::no;
    }

    if (inMxFiguredBass.duration().has_value())
    {
        figuredBass.durationTimeTicks = myCurrentCursor.convertDurationToGlobalTickScale(*inMxFiguredBass.duration());
    }

    figuredBass.id = getId(inMxFiguredBass);

    auto direction = api::DirectionData{};
    direction.tickTimePosition = myCurrentCursor.tickTimePosition;
    direction.placement = api::Placement::below;
    direction.isStaffValueSpecified = true;

    if (nextNotePtr)
    {
        direction.voice = NoteReader{*nextNotePtr}.getVoiceNumber();
    }

    direction.figuredBasses.emplace_back(std::move(figuredBass));

    const auto staffIndex = getFiguredBassStaffIndex(myCurrentCursor, myOutMeasureData, nextNotePtr);
    myOutMeasureData.staves.at(static_cast<size_t>(staffIndex)).directions.emplace_back(std::move(direction));
}

void MeasureReader::parsePrint(const core::Print &inMxPrint) const
{
    // Per-measure <print> layout is read at the score level, keyed by
    // measure index, in ScoreReader::scanForSystemInfo and
    // ScoreReader::scanForPageInfo (which capture new-system, new-page,
    // page-number, system-layout, staff-layout, and page-layout).
    // measure-numbering is measure-scoped api state (MeasureData::measureNumbering,
    // not carried forward like time/key), so it is captured directly here instead.
    if (inMxPrint.measureNumbering().has_value())
    {
        const auto &measureNumbering = *inMxPrint.measureNumbering();
        myOutMeasureData.measureNumbering = myConverter.convertMeasureNumbering(measureNumbering.value());

        if (measureNumbering.multipleRestAlways().has_value())
        {
            myOutMeasureData.measureNumberingMultipleRestAlways =
                myConverter.convert(*measureNumbering.multipleRestAlways());
        }

        if (measureNumbering.multipleRestRange().has_value())
        {
            myOutMeasureData.measureNumberingMultipleRestRange =
                myConverter.convert(*measureNumbering.multipleRestRange());
        }

        if (measureNumbering.system().has_value())
        {
            myOutMeasureData.measureNumberingSystemRelation =
                myConverter.convertSystemRelation(*measureNumbering.system());
        }

        if (measureNumbering.staff().has_value())
        {
            // core staff numbers are one-based; the api index is zero-based.
            myOutMeasureData.measureNumberingStaffIndex = measureNumbering.staff()->value() - 1;
        }
    }
}

void MeasureReader::parseSound(const core::Sound &inMxSound) const
{
    auto soundData = readSoundData(inMxSound);

    if (!soundData.isSpecified())
    {
        return;
    }

    if (myOutMeasureData.staves.empty())
    {
        return;
    }

    // A standalone <sound> has no <staff>; place it on staff 0 with isStaffValueSpecified = false
    // and no other direction content, so it round-trips as a standalone <sound> element.
    auto directionData = api::DirectionData{};
    directionData.tickTimePosition = myCurrentCursor.tickTimePosition;
    directionData.isStaffValueSpecified = false;
    directionData.isSoundDataSpecified = true;
    directionData.soundData = std::move(soundData);

    myOutMeasureData.staves.at(0).directions.emplace_back(std::move(directionData));
}

void MeasureReader::parseBarline(const core::Barline &inMxBarline) const
{
    auto barline = api::BarlineData{};
    auto loc = api::HorizontalAlignment::unspecified;
    auto style = api::BarlineType::unspecified;
    auto repeat = false;
    auto repeatTimes = 0;
    auto repeatDirection = api::RepeatDirection::unspecified;
    auto repeatAfterJump = api::Bool::unspecified;
    auto repeatWinged = api::RepeatWinged::unspecified;

    if (inMxBarline.location().has_value())
    {
        loc = myConverter.convertBarlinePlacement(*inMxBarline.location());
    }

    if (inMxBarline.barStyle().has_value())
    {
        style = myConverter.convert(inMxBarline.barStyle()->value());
    }

    // make a right-side barline last in the data
    if (loc == api::HorizontalAlignment::right)
    {
        barline.tickTimePosition = api::TICK_TIME_INFINITY;
    }
    else
    {
        barline.tickTimePosition = myCurrentCursor.tickTimePosition;
    }

    if (inMxBarline.ending().has_value())
    {
        const auto &mxEnding = *inMxBarline.ending();
        auto endingData = api::EndingData{};
        endingData.type = myConverter.convert(mxEnding.type());

        const auto numValues = mxEnding.number().values();
        endingData.numbers.assign(numValues.begin(), numValues.end());
        endingData.text = mxEnding.value();

        barline.ending = std::move(endingData);
    }

    if (inMxBarline.repeat().has_value())
    {
        const auto &mxRepeat = *inMxBarline.repeat();
        repeat = true;
        repeatTimes = mxRepeat.times().value_or(0);
        repeatDirection = myConverter.convert(mxRepeat.direction());

        if (mxRepeat.afterJump().has_value())
        {
            repeatAfterJump = myConverter.convert(*mxRepeat.afterJump());
        }

        if (mxRepeat.winged().has_value())
        {
            repeatWinged = myConverter.convert(*mxRepeat.winged());
        }
    }

    barline.barlineType = style;
    barline.location = loc;
    barline.repeat = repeat;
    barline.repeatTimes = repeatTimes;
    barline.repeatDirection = repeatDirection;
    barline.repeatAfterJump = repeatAfterJump;
    barline.repeatWinged = repeatWinged;
    barline.id = getId(inMxBarline);
    myOutMeasureData.barlines.emplace_back(std::move(barline));
}

void MeasureReader::parseGrouping(const core::Grouping &inMxGrouping) const
{
    MX_UNUSED(inMxGrouping);
    // std::cout << "grouping is not supported" << std::endl;
}

void MeasureReader::parseLink(const core::Link &inMxLink) const
{
    MX_UNUSED(inMxLink);
    // std::cout << "link is not supported" << std::endl;
}

void MeasureReader::parseBookmark(const core::Bookmark &inMxBookmark) const
{
    MX_UNUSED(inMxBookmark);
    // std::cout << "bookmark is not supported" << std::endl;
}

void MeasureReader::importStaffDetails(const core::Attributes &inMxAttributes) const
{
    for (const auto &staffDetails : inMxAttributes.staffDetails())
    {
        if (!staffDetails.group().has_value() && !staffDetails.staffSize().has_value())
        {
            continue;
        }

        auto staffIndex = 0;
        if (staffDetails.number().has_value())
        {
            staffIndex = staffDetails.number()->value() - 1;
        }

        if (staffIndex < 0 || staffIndex >= static_cast<int>(myOutMeasureData.staves.size()))
        {
            continue;
        }

        auto &staffData = myOutMeasureData.staves.at(static_cast<size_t>(staffIndex));
        if (staffDetails.group().has_value())
        {
            staffData.staffLines = staffDetails.group()->staffLines();
        }
        if (staffDetails.staffSize().has_value())
        {
            staffData.staffSize = staffDetails.staffSize()->value().value().value();
            if (staffDetails.staffSize()->scaling().has_value())
            {
                staffData.staffScaling = staffDetails.staffSize()->scaling()->value().value();
            }
        }
    }
}

void MeasureReader::importClefs(std::span<const core::Clef> inClefs) const
{
    for (const auto &clef : inClefs)
    {
        importClef(clef);
    }
}

void MeasureReader::importClef(const core::Clef &inClef) const
{
    api::ClefData clefData;
    clefData.tickTimePosition = myCurrentCursor.tickTimePosition;
    clefData.id = getId(inClef);
    auto converter = Converter{};
    clefData.symbol = converter.convert(inClef.clef().sign());

    if (inClef.clef().line().has_value())
    {
        clefData.line = inClef.clef().line()->value();
        clefData.isLineSpecified = true;
    }
    else
    {
        clefData.isLineSpecified = false;
        switch (clefData.symbol)
        {
        case api::ClefSymbol::g:
            clefData.line = 2;
            break;
        case api::ClefSymbol::f:
            clefData.line = 4;
            break;
        case api::ClefSymbol::c:
            clefData.line = 3;
            break;
        case api::ClefSymbol::jianpu:
            clefData.line = 1;
            break;
        case api::ClefSymbol::tab:
            clefData.line = 1;
            break;
        case api::ClefSymbol::percussion:
            clefData.line = 3;
            break;
        default:
            clefData.line = 3;
            break;
        }
    }

    if (inClef.clef().clefOctaveChange().has_value())
    {
        clefData.octaveChange = *inClef.clef().clefOctaveChange();
        clefData.isOctaveChangeSpecified = true;
    }
    else
    {
        clefData.octaveChange = 0;
        clefData.isOctaveChangeSpecified = false;
    }

    if (inClef.additional().has_value())
    {
        clefData.additional = converter.convert(*inClef.additional());
    }

    if (inClef.printObject().has_value())
    {
        clefData.printObject = converter.convert(*inClef.printObject());
    }

    const bool sourceHasNumber = inClef.number().has_value();
    int celfStaffIndex = sourceHasNumber ? inClef.number()->value() - 1 : 0;

    // Auto rule (see ClefData::writeStaffNumber): include the number unless this is a single-staff
    // part carrying the implied 1 (staff index 0). Record an explicit override only when the source
    // diverges from that rule, so the common case stays unspecified.
    const bool autoIncludes = !(myCurrentCursor.getNumStaves() == 1 && celfStaffIndex == 0);
    if (sourceHasNumber && !autoIncludes)
    {
        clefData.writeStaffNumber = api::Bool::yes;
    }
    else if (!sourceHasNumber && autoIncludes)
    {
        clefData.writeStaffNumber = api::Bool::no;
    }

    if (myCurrentCursor.tickTimePosition == 0)
    {
        if (inClef.afterBarline().has_value())
        {
            if (inClef.afterBarline()->tag() == core::YesNo::Tag::yes)
            {
                clefData.location = api::ClefLocation::afterBarline;
            }
            else
            {
                clefData.location = api::ClefLocation::beforeBarline;
            }
        }
        else
        {
            clefData.location = api::ClefLocation::unspecified;
        }
    }
    else
    {
        clefData.location = api::ClefLocation::midMeasure;
    }
    insertClef(std::move(clefData), celfStaffIndex);
}

void MeasureReader::insertNoteData(api::NoteData &&noteData, int staff, int voice, const std::string &voiceLabel) const
{
    MX_ASSERT(staff >= 0);
    MX_ASSERT(static_cast<size_t>(staff) < myOutMeasureData.staves.size());
    auto &staffRef = myOutMeasureData.staves.at(static_cast<size_t>(staff));

    if (staffRef.voices.find(voice) == staffRef.voices.cend())
    {
        staffRef.voices[voice] = api::VoiceData{};
    }
    auto &voiceRef = staffRef.voices[voice];

    // The first note that names the voice names it for all of them. Notes without a <voice> get
    // the name of the voice they land in.
    if (voiceRef.label.empty())
    {
        voiceRef.label = voiceLabel;
    }

    voiceRef.notes.emplace_back(std::move(noteData));
}

void MeasureReader::insertClef(api::ClefData &&clefData, int staff) const
{
    MX_ASSERT(staff >= 0);
    MX_ASSERT(static_cast<size_t>(staff) < myOutMeasureData.staves.size());
    auto &staffRef = myOutMeasureData.staves.at(static_cast<size_t>(staff));
    staffRef.clefs.emplace_back(std::move(clefData));
}

void MeasureReader::consolidateVoicesForAllStaves() const
{
    for (auto &staff : myOutMeasureData.staves)
    {
        if (isUserRequestedVoiceNumberConsistentAcrossAllVoices(staff))
        {
            takeUserRequestedVoiceNumbers(staff);
        }
        else
        {
            collapseVoicesAutomatically(staff);
        }
    }

    dropRedundantVoiceLabels();
}

// A voice whose name is the number the writer would give it anyway does not need to carry one.
// Clearing those keeps VoiceData::label empty in the common case, where it stands for a voice
// name the index cannot express.
void MeasureReader::dropRedundantVoiceLabels() const
{
    for (auto &staff : myOutMeasureData.staves)
    {
        for (auto &voicePair : staff.voices)
        {
            if (voicePair.second.label == std::to_string(voicePair.first + 1))
            {
                voicePair.second.label.clear();
            }
        }
    }
}

void MeasureReader::takeUserRequestedVoiceNumbers(api::StaffData &staff) const
{
    std::map<int, api::VoiceData> newVoiceData;
    for (const auto &voicePair : staff.voices)
    {
        const int userRequestedVoiceNumber = getUserRequestedVoiceNumber(voicePair.second);
        MX_ASSERT(userRequestedVoiceNumber >= 1);
        newVoiceData[userRequestedVoiceNumber - 1] = std::move(voicePair.second);
    }
    staff.voices = std::move(newVoiceData);
}

void MeasureReader::collapseVoicesAutomatically(api::StaffData &staff) const
{
    std::map<int, api::VoiceData> newVoiceData;
    int newVoiceIndex = 0;
    for (auto &voicePair : staff.voices)
    {
        newVoiceData[newVoiceIndex] = std::move(voicePair.second);
        ++newVoiceIndex;
    }
    staff.voices = std::move(newVoiceData);
}

bool MeasureReader::isUserRequestedVoiceNumberConsistent(const api::VoiceData &voiceData) const
{
    if (voiceData.notes.empty())
    {
        return true;
    }

    const int userRequestedVoiceNumber = voiceData.notes.front().userRequestedVoiceNumber;

    if (userRequestedVoiceNumber < 1)
    {
        return false;
    }

    for (const auto &note : voiceData.notes)
    {
        if (note.userRequestedVoiceNumber != userRequestedVoiceNumber)
        {
            return false;
        }
    }
    return true;
}

bool MeasureReader::isUserRequestedVoiceNumberConsistentAcrossAllVoices(const api::StaffData &staff) const
{
    std::set<int> userRequestedVoiceNumbers;
    for (const auto &voicePair : staff.voices)
    {
        if (!isUserRequestedVoiceNumberConsistent(voicePair.second))
        {
            return false;
        }
        const int userRequestedVoiceNumber = getUserRequestedVoiceNumber(voicePair.second);
        auto result = userRequestedVoiceNumbers.insert(userRequestedVoiceNumber);

        if (!result.second)
        {
            return false;
        }
    }
    return true;
}

int MeasureReader::getUserRequestedVoiceNumber(const api::VoiceData &voiceData) const
{
    if (voiceData.notes.empty())
    {
        return api::VALUE_UNSPECIFIED;
    }

    return voiceData.notes.front().userRequestedVoiceNumber;
}

void MeasureReader::advanceTickTimePosition(int amount, std::string reason) const
{
    HistoryRecord record;
    record.reason = std::move(reason);
    record.amount = amount;
    record.timeBefore = myCurrentCursor.tickTimePosition;
    record.cursorBefore = myCurrentCursor;
    myCurrentCursor.tickTimePosition += amount;
    record.cursorAfter = myCurrentCursor;
    record.timeAfter = myCurrentCursor.tickTimePosition;

    myHistory.push_back(record);
    //            std::cout << record.reason << std::endl;
}
} // namespace impl
} // namespace mx
