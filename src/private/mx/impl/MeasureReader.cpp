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
#include "mx/impl/NoteFunctions.h"
#include "mx/impl/NoteReader.h"
#include "mx/impl/SoundFunctions.h"
#include "mx/impl/TimeReader.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

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
      myPreviousMeasureCursor{previousMeasureCursor}, myHistory{}
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

    // if we are parsing the first measure of the part, then we need to return transpose information
    std::optional<api::TransposeData> transpose;

    if (myOutMeasureData.number == std::to_string(myCurrentCursor.measureIndex + 1))
    {
        myOutMeasureData.number = "";
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

    const auto mdcSpan = myPartwiseMeasure.musicData();
    auto iter = mdcSpan.begin();
    const auto endIter = mdcSpan.end();

    for (; iter != endIter; ++iter)
    {
        const auto &mdc = *iter;

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

        auto maybeTranspose = parseMusicDataChoice(mdc, nextNotePtr);
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
    TimeReader timeReader{myPartwiseMeasure.musicData()};
    api::TimeSignatureData timeSignature;

    if (timeReader.getIsTimeFound())
    {
        timeSignature = timeReader.getTimeSignatureData();
        timeSignature.isImplicit = false;
    }
    else // no time signature was found
    {
        if (myCurrentCursor.measureIndex > 0)
        {
            timeSignature = myPreviousMeasureCursor.timeSignature;
        }
        timeSignature.isImplicit = true;
    }

    myOutMeasureData.timeSignature = timeSignature;
    myCurrentCursor.timeSignature = timeSignature;
    advanceTickTimePosition(0, "parseTimeSignature");
}

// see .h file for explanation of the return value
std::optional<api::TransposeData> MeasureReader::parseMusicDataChoice(const core::MusicDataChoice &mdc,
                                                                      const core::Note *nextNotePtr) const
{
    // if we are parsing the first measure of the part, then we need to return transpose information
    std::optional<api::TransposeData> transpose;

    if (mdc.isNote())
    {
        myCurrentCursor.isBackupInProgress = false;
        parseNote(mdc.asNote(), nextNotePtr);
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

void MeasureReader::parseNote(const core::Note &inMxNote, const core::Note *nextNotePtr) const
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

    myCurrentCursor.staffIndex = noteDataStaffIndex;

    bool isThisNotePartOfAChord = noteData.isChord || isNextNotePartOfAChord;
    noteData.isChord = isThisNotePartOfAChord;

    if (!isThisNotePartOfAChord || !isNextNotePartOfAChord)
    {
        advanceTickTimePosition(noteData.durationData.durationTimeTicks, "note");
    }

    MX_ASSERT(noteDataStaffIndex >= 0);
    MX_ASSERT(static_cast<size_t>(noteDataStaffIndex) < myOutMeasureData.staves.size());
    insertNoteData(std::move(noteData), myCurrentCursor.staffIndex, myCurrentCursor.voiceIndex);
}

void MeasureReader::parseBackup(const core::Backup &inMxBackup) const
{
    if (!(myCurrentCursor.isBackupInProgress))
    {
        ++myCurrentCursor.voiceIndex;
    }

    myCurrentCursor.isBackupInProgress = true;
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
                keyData.staffIndex = -1;
            }
        }

        keyData.fifths = traditionalKey.fifths().value();
        if (traditionalKey.cancel().has_value())
        {
            keyData.cancel = traditionalKey.cancel()->value().value();
        }

        if (traditionalKey.mode().has_value())
        {
            // TODO - support all modes, not just major/minor
            const auto coreMode = traditionalKey.mode()->value();
            if (coreMode == "major")
            {
                keyData.mode = api::KeyMode::major;
            }
            else if (coreMode == "minor")
            {
                keyData.mode = api::KeyMode::minor;
            }
            else
            {
                keyData.mode = api::KeyMode::unsupported;
            }
        }
        keyData.tickTimePosition = myCurrentCursor.tickTimePosition;

        myOutMeasureData.keys.emplace_back(std::move(keyData));
    }
    importStaffDetails(inMxAttributes);
    importClefs(inMxAttributes.clef());

    if (inMxAttributes.choice().isTranspose() && !inMxAttributes.choice().asTranspose().empty())
    {
        // TODO support transpositions at places other than the start of the score
        if (myCurrentCursor.measureIndex == 0 && myCurrentCursor.tickTimePosition == 0)
        {
            const auto &coreTranspose = inMxAttributes.choice().asTranspose().front();
            transpose = Converter::convertToTransposeData(coreTranspose);
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
    // page-number, system-layout, staff-layout, and page-layout). The
    // per-measure music-data hook has no api home of its own, so nothing
    // is captured here.
    MX_UNUSED(inMxPrint);
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
    auto endingType = api::EndingType::none;
    auto endingNumber = 0;
    auto repeat = false;

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
        const auto &ending = *inMxBarline.ending();

        switch (ending.type().tag())
        {
        case core::StartStopDiscontinue::Tag::start:
            endingType = api::EndingType::start;
            break;

        case core::StartStopDiscontinue::Tag::stop:
            endingType = api::EndingType::stop;
            break;

        case core::StartStopDiscontinue::Tag::discontinue:
            endingType = api::EndingType::discontinue;
            break;
        };

        const auto &numValues = ending.number().values();
        if (!numValues.empty())
        {
            endingNumber = numValues.front();
        }
    }

    if (inMxBarline.repeat().has_value())
    {
        repeat = true;
    }

    barline.barlineType = style;
    barline.location = loc;
    barline.endingType = endingType;
    barline.endingNumber = endingNumber;
    barline.repeat = repeat;
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
        if (!staffDetails.group().has_value())
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

        myOutMeasureData.staves.at(static_cast<size_t>(staffIndex)).staffLines = staffDetails.group()->staffLines();
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
    }
    else
    {
        clefData.octaveChange = 0;
    }

    int celfStaffIndex = -1;
    if (inClef.number().has_value())
    {
        celfStaffIndex = inClef.number()->value() - 1;
    }
    else
    {
        celfStaffIndex = 0;
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

void MeasureReader::insertNoteData(api::NoteData &&noteData, int staff, int voice) const
{
    MX_ASSERT(staff >= 0);
    MX_ASSERT(static_cast<size_t>(staff) < myOutMeasureData.staves.size());
    auto &staffRef = myOutMeasureData.staves.at(static_cast<size_t>(staff));

    if (staffRef.voices.find(voice) == staffRef.voices.cend())
    {
        staffRef.voices[voice] = api::VoiceData{};
    }
    auto &voiceRef = staffRef.voices[voice];
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
        if (isUserRequestedVoiceNumberConsistentAccrossAllVoices(staff))
        {
            takeUserRequestedVoiceNumbers(staff);
        }
        else
        {
            collapseVoicesAutomatically(staff);
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

bool MeasureReader::isUserRequestedVoiceNumberConsistentAccrossAllVoices(const api::StaffData &staff) const
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
        return -1;
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
