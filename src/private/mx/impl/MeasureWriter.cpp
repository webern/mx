// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MeasureWriter.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/AllMarginsGroup.h"
#include "mx/core/generated/Backup.h"
#include "mx/core/generated/BackwardForward.h"
#include "mx/core/generated/BarStyleColor.h"
#include "mx/core/generated/Barline.h"
#include "mx/core/generated/Ending.h"
#include "mx/core/generated/EndingNumber.h"
#include "mx/core/generated/Forward.h"
#include "mx/core/generated/LayoutGroup.h"
#include "mx/core/generated/LeftRightMarginsGroup.h"
#include "mx/core/generated/MarginType.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/PageLayout.h"
#include "mx/core/generated/PageLayoutGroup.h"
#include "mx/core/generated/PageMargins.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PositiveDivisions.h"
#include "mx/core/generated/Print.h"
#include "mx/core/generated/Repeat.h"
#include "mx/core/generated/RightLeftMiddle.h"
#include "mx/core/generated/StaffLayout.h"
#include "mx/core/generated/SystemLayout.h"
#include "mx/core/generated/SystemMargins.h"
#include "mx/core/generated/Tenths.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DirectionWriter.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/NoteWriter.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/utility/Throw.h"

#include <limits>

namespace mx
{
namespace impl
{
MeasureWriter::MeasureWriter(const api::MeasureData &inMeasureData, const MeasureCursor &inCursor,
                             const ScoreWriter &inScoreWriter)
    : myMeasureData{inMeasureData}, myOutMeasure{}, myPreviousCursor{inCursor}, myScoreWriter{inScoreWriter},
      myPropertiesWriter{nullptr}, myConverter{}, myBarlinesIter{inMeasureData.barlines.cbegin()},
      myBarlinesEnd{inMeasureData.barlines.cend()}, myMeasureKeysIter{inMeasureData.keys.cbegin()},
      myMeasureKeysEnd{inMeasureData.keys.cend()}, myHistory{inCursor}
{
}

core::PartwiseMeasure MeasureWriter::getPartwiseMeasure()
{
    myOutMeasure = core::PartwiseMeasure{};
    myPropertiesWriter = std::unique_ptr<PropertiesWriter>{new PropertiesWriter{myOutMeasure}};
    auto cursor = myHistory.getCursor();
    cursor.reset();
    myHistory = History{cursor};
    myBarlinesIter = myMeasureData.barlines.cbegin();
    myBarlinesEnd = myMeasureData.barlines.cend();
    myMeasureKeysIter = myMeasureData.keys.cbegin();

    writeMeasureGlobals();
    writeStaves();
    myPropertiesWriter->flushBuffer();
    writeBarlines(api::TICK_TIME_INFINITY);
    myPropertiesWriter = nullptr;
    return myOutMeasure;
}

void MeasureWriter::writeMeasureGlobals()
{
    writeBarlines(0);

    if (myMeasureData.number.size() > 0)
    {
        myOutMeasure.setNumber(myMeasureData.number);
    }
    else
    {
        myOutMeasure.setNumber(std::to_string(myHistory.getCursor().measureIndex + 1));
    }

    if (myMeasureData.width >= 0.0)
    {
        myOutMeasure.setWidth(core::Tenths{core::Decimal{static_cast<double>(myMeasureData.width)}});
    }

    Converter converter;

    if (myMeasureData.implicit != api::Bool::unspecified)
    {
        myOutMeasure.setImplicit(converter.convert(myMeasureData.implicit));
    }

    if (myScoreWriter.isSystemInfo(myHistory.getCursor().measureIndex))
    {
        writeSystemInfo();
    }

    const auto &pageData = myScoreWriter.findPageLayoutData(myHistory.getCursor().measureIndex);
    if (pageData)
    {
        writePageInfo(*pageData);
    }

    if (myHistory.getCursor().isFirstMeasureInPart)
    {
        myPropertiesWriter->writeDivisions(myHistory.getCursor().getGlobalTicksPerQuarter());

        if (myMeasureData.staves.size() > 1)
        {
            myPropertiesWriter->writeNumStaves(static_cast<int>(myMeasureData.staves.size()));
        }
    }

    if (!myMeasureData.timeSignature.isImplicit)
    {
        myPropertiesWriter->writeTime(myMeasureData.timeSignature);
    }

    int localStaffCounter = 0;

    for (const auto &staff : myMeasureData.staves)
    {
        if (staff.staffLines >= 0)
        {
            int desiredStaffIndex = -1;
            if (myHistory.getCursor().getNumStaves() > 1)
            {
                desiredStaffIndex = localStaffCounter;
            }
            myPropertiesWriter->writeStaffDetails(desiredStaffIndex, staff.staffLines);
        }

        auto clefIter = staff.clefs.cbegin();
        auto clefEnd = staff.clefs.cend();
        while (clefIter != clefEnd && clefIter->tickTimePosition == 0)
        {
            int desiredStaffIndex = -1;
            if (myHistory.getCursor().getNumStaves() > 1)
            {
                desiredStaffIndex = localStaffCounter;
            }
            myPropertiesWriter->writeClef(desiredStaffIndex, *clefIter);
            ++clefIter;
        }
        ++localStaffCounter;
    }

    // TODO - these might end up in the wrong place, figure out how
    // to preserve location if the barline is not at the beginning or
    // the end

    while (myMeasureKeysIter != myMeasureKeysEnd && myMeasureKeysIter->tickTimePosition == 0)
    {
        myPropertiesWriter->writeKey(myMeasureKeysIter->staffIndex, *myMeasureKeysIter);
        ++myMeasureKeysIter;
    }

    // The transpose element goes into the measures, but for convenience we have added it to
    // the mx::api::PartData struct since the most typical use case is to add it as part of
    // an instrument/part definition.
    if (cursor().measureIndex == 0 && cursor().tickTimePosition == 0)
    {
        const auto &part = this->myScoreWriter.getPart(cursor().partIndex);
        if (part.transposition && part.transposition->isUsed())
        {
            myPropertiesWriter->writeTranspose(part.transposition.value());
        }
    }
}

void MeasureWriter::writeSystemInfo()
{
    auto systemData = myScoreWriter.getSystemData(myHistory.getCursor().measureIndex);

    // early exit if there is nothing to do
    if (!systemData.isUsed())
    {
        return;
    }

    core::Print outPrint{};
    myHistory.log("writePrint");

    if (isSpecified(systemData.newSystem))
    {
        outPrint.setNewSystem(systemData.newSystem == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    if (systemData.layout.isUsed())
    {
        const auto &inSystemLayout = systemData.layout;
        auto outSystemLayout = outPrint.layout().systemLayout().value_or(core::SystemLayout{});
        auto outLayoutGroup = outPrint.layout();
        bool needsSystemLayout = false;

        if (inSystemLayout.margins)
        {
            const auto &inMargins = inSystemLayout.margins.value();
            core::LeftRightMarginsGroup lrm;
            lrm.setLeftMargin(core::Tenths{core::Decimal{static_cast<double>(inMargins.left)}});
            lrm.setRightMargin(core::Tenths{core::Decimal{static_cast<double>(inMargins.right)}});
            core::SystemMargins sysMargins;
            sysMargins.setLeftRightMargins(lrm);
            outSystemLayout.setSystemMargins(sysMargins);
            needsSystemLayout = true;
        }

        if (inSystemLayout.topSystemDistance)
        {
            outSystemLayout.setTopSystemDistance(
                core::Tenths{core::Decimal{static_cast<double>(inSystemLayout.topSystemDistance.value())}});
            needsSystemLayout = true;
        }

        if (inSystemLayout.systemDistance)
        {
            outSystemLayout.setSystemDistance(
                core::Tenths{core::Decimal{static_cast<double>(inSystemLayout.systemDistance.value())}});
            needsSystemLayout = true;
        }

        if (needsSystemLayout)
        {
            outLayoutGroup.setSystemLayout(outSystemLayout);
            outPrint.setLayout(outLayoutGroup);
        }

        if (inSystemLayout.staffDistance)
        {
            core::StaffLayout outStaffLayout{};
            outStaffLayout.setStaffDistance(
                core::Tenths{core::Decimal{static_cast<double>(inSystemLayout.staffDistance.value())}});
            outLayoutGroup.addStaffLayout(outStaffLayout);
            outPrint.setLayout(outLayoutGroup);
        }
    }

    myOutMeasure.addMusicData(core::MusicDataChoice::print(outPrint));
}

void MeasureWriter::writePageInfo(const api::PageData &inPageData)
{
    // since a print object may have been added by writeSystemInfo, we don't want to add another one. we will
    // search for a print object and use it if we find it, otherwise we will add a new one.
    // In the new value-based API we have to find the index of an existing print MusicDataChoice.
    const auto existingData = myOutMeasure.musicData();
    int printIndex = -1;
    int idx = 0;
    for (const auto &mdc : existingData)
    {
        if (mdc.isPrint())
        {
            printIndex = idx;
            break;
        }
        ++idx;
    }

    core::Print outPrint{};
    if (printIndex >= 0)
    {
        outPrint = existingData[printIndex].asPrint();
    }

    if (isSpecified(inPageData.newPage))
    {
        outPrint.setNewPage(inPageData.newPage == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    if (inPageData.pageLayoutData.isUsed())
    {
        // createPageLayout is static in LayoutFunctions.cpp — replicate inline
        const auto &inPageLayout = inPageData.pageLayoutData;
        auto outLayoutGroup = outPrint.layout();
        // We need to build a PageLayout — defer to addPageLayout helper via a
        // temporary ScoreHeaderGroup, but that's complex. Instead, build inline.
        // The LayoutFunctions.cpp createPageLayout is static, so we inline it here.
        // The logic mirrors the function in LayoutFunctions.cpp.
        core::PageLayout outPageLayout{};

        if (inPageLayout.size)
        {
            core::PageLayoutGroup group;
            group.setPageHeight(core::Tenths{core::Decimal{static_cast<double>(inPageLayout.size->height)}});
            group.setPageWidth(core::Tenths{core::Decimal{static_cast<double>(inPageLayout.size->width)}});
            outPageLayout.setGroup(group);
        }

        // page margins
        const auto &inPageMargins = inPageLayout.margins;
        if (inPageMargins.isUsed())
        {
            const bool areEvenOddSame = inPageMargins.same();

            if (inPageMargins.odd)
            {
                core::PageMargins m;
                m.setType(areEvenOddSame ? core::MarginType::both() : core::MarginType::odd());
                core::AllMarginsGroup allMargins;
                core::LeftRightMarginsGroup lrm;
                lrm.setLeftMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.odd->left)}});
                lrm.setRightMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.odd->right)}});
                allMargins.setLeftRightMargins(lrm);
                allMargins.setTopMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.odd->top)}});
                allMargins.setBottomMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.odd->bottom)}});
                m.setAllMargins(allMargins);
                (void)outPageLayout.addPageMargins(std::move(m));
            }

            if (inPageMargins.even && !areEvenOddSame)
            {
                core::PageMargins m;
                m.setType(core::MarginType::even());
                core::AllMarginsGroup allMargins;
                core::LeftRightMarginsGroup lrm;
                lrm.setLeftMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.even->left)}});
                lrm.setRightMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.even->right)}});
                allMargins.setLeftRightMargins(lrm);
                allMargins.setTopMargin(core::Tenths{core::Decimal{static_cast<double>(inPageMargins.even->top)}});
                allMargins.setBottomMargin(
                    core::Tenths{core::Decimal{static_cast<double>(inPageMargins.even->bottom)}});
                m.setAllMargins(allMargins);
                (void)outPageLayout.addPageMargins(std::move(m));
            }
        }

        outLayoutGroup.setPageLayout(outPageLayout);
        outPrint.setLayout(outLayoutGroup);
    }

    if (inPageData.pageNumber)
    {
        outPrint.setPageNumber(*inPageData.pageNumber);
    }

    if (printIndex >= 0)
    {
        // Replace the existing print entry. We need to rebuild musicData.
        std::vector<core::MusicDataChoice> newData;
        newData.reserve(existingData.size());
        int i = 0;
        for (const auto &mdc : existingData)
        {
            if (i == printIndex)
            {
                newData.push_back(core::MusicDataChoice::print(outPrint));
            }
            else
            {
                newData.push_back(mdc);
            }
            ++i;
        }
        myOutMeasure.setMusicData(std::move(newData));
    }
    else
    {
        myOutMeasure.addMusicData(core::MusicDataChoice::print(outPrint));
    }
}

void MeasureWriter::writeStaves()
{
    myHistory.resetCursorFofStaffIterations();
    myPreviousCursor = myHistory.getCursor();
    for (const auto &staff : myMeasureData.staves)
    {
        writeVoices(staff);
        myHistory.nextStaff();
    }
}

void MeasureWriter::writeVoices(const api::StaffData &inStaff)
{
    auto clefIter = inStaff.clefs.cbegin();
    const auto clefEnd = inStaff.clefs.cend();
    while (clefIter != clefEnd && clefIter->tickTimePosition == 0)
    {
        ++clefIter;
    }
    auto staffKeyIter = inStaff.keys.cbegin();
    auto staffKeyEnd = inStaff.keys.cend();
    auto directionIter = inStaff.directions.cbegin();
    auto directionEnd = inStaff.directions.cend();

    for (const auto &voice : inStaff.voices)
    {
        bool isFirstNote = true;
        int currentChordTickPosition = -1;
        int previousChordTickPosition = -2;
        myHistory.setVoiceIndex(voice.first);
        auto noteIter = voice.second.notes.cbegin();
        auto noteEnd = voice.second.notes.cend();
        int noteIndex = 0;

        if (directionIter != directionEnd)
        {
            writeDirections(directionIter, directionEnd, noteIter, std::cbegin(voice.second.notes), noteEnd);
        }

        for (; noteIter != noteEnd; ++noteIter, ++noteIndex)
        {
            bool isStartOfChord = false;

            myHistory.setChord(noteIter->isChord);
            const auto &apiNote = *noteIter;
            writeForwardOrBackupIfNeeded(apiNote);

            if (noteIter->isChord)
            {
                currentChordTickPosition = myHistory.getCursor().tickTimePosition;
                if (isFirstNote)
                {
                    isStartOfChord = true;
                }
                else if (currentChordTickPosition != previousChordTickPosition)
                {
                    isStartOfChord = true;
                }

                previousChordTickPosition = currentChordTickPosition;
            }

            if (myMeasureKeysIter != myMeasureKeysEnd)
            {
                if (myMeasureKeysIter->tickTimePosition <= myHistory.getCursor().tickTimePosition)
                {
                    myPropertiesWriter->writeKey(-1, *myMeasureKeysIter);
                    ++myMeasureKeysIter;
                }
            }

            while (clefIter != clefEnd && clefIter->tickTimePosition <= myHistory.getCursor().tickTimePosition)
            {
                myPropertiesWriter->writeClef(myHistory.getCursor().staffIndex, *clefIter);
                ++clefIter;
            }

            for (; staffKeyIter != staffKeyEnd &&
                   staffKeyIter->tickTimePosition <= myHistory.getCursor().tickTimePosition;
                 ++staffKeyIter)
            {
                myPropertiesWriter->writeKey(myHistory.getCursor().staffIndex, *staffKeyIter);
            }

            myPropertiesWriter->flushBuffer();
            writeDirections(directionIter, directionEnd, noteIter, std::cbegin(voice.second.notes), noteEnd);

            NoteWriter writer{
                apiNote,  myHistory.getCursor(), myScoreWriter, myPreviousCursor.isChordActive, voice.second.notes,
                noteIndex};
            myOutMeasure.addMusicData(core::MusicDataChoice::note(writer.getNote(isStartOfChord)));
            myHistory.log("addNote cursorTime " + std::to_string(myHistory.getCursor().tickTimePosition) +
                          ", noteTime " + std::to_string(apiNote.tickTimePosition));
            advanceCursorIfNeeded(apiNote, noteIter, noteEnd);
            myPreviousCursor = myHistory.getCursor();
            isFirstNote = false;

        } // foreach note

        writeDirections(directionIter, directionEnd, std::cend(voice.second.notes), std::cbegin(voice.second.notes),
                        std::cend(voice.second.notes));

    } // foreach voice

    std::vector<api::NoteData> fakeEmptyNoteVector;
    const auto iterBegin = std::cbegin(fakeEmptyNoteVector);
    const auto iterEnd = std::cend(fakeEmptyNoteVector);
    writeDirections(directionIter, directionEnd, iterEnd, iterBegin, iterEnd);

    bool areClefsRemaining = clefIter != clefEnd;
    bool areMeasureKeysRemaining = myMeasureKeysIter != myMeasureKeysEnd;
    bool areStaffKeysRemaining = staffKeyIter != staffKeyEnd;

    if (areClefsRemaining || areMeasureKeysRemaining || areStaffKeysRemaining)
    {
        for (; clefIter != inStaff.clefs.cend(); ++clefIter)
        {
            MX_ASSERT(clefIter != inStaff.clefs.cend());
            api::ClefData clef = *clefIter;
            myPropertiesWriter->writeClef(myHistory.getCursor().staffIndex, clef);
        }

        if (myMeasureKeysIter != myMeasureKeysEnd)
        {
            if (myMeasureKeysIter->tickTimePosition)
            {
                myPropertiesWriter->writeKey(-1, *myMeasureKeysIter);
                ++myMeasureKeysIter;
            }
        }

        for (; staffKeyIter != staffKeyEnd; ++staffKeyIter)
        {
            myPropertiesWriter->writeKey(myHistory.getCursor().staffIndex, *staffKeyIter);
        }
        myPropertiesWriter->flushBuffer();
    }

} // func writeVoices

void MeasureWriter::writeForwardOrBackupIfNeeded(const api::NoteData &currentNote)
{
    myPropertiesWriter->flushBuffer();

    if (currentNote.tickTimePosition < 0)
    {
        return;
    }

    const int timeDifference = currentNote.tickTimePosition - myHistory.getCursor().tickTimePosition;
    if (timeDifference < 0)
    {
        backup(-1 * timeDifference);
    }
    else if (timeDifference > 0)
    {
        forward(timeDifference);
    }
}

void MeasureWriter::backup(const int inTicks)
{
    // ensure we don't go below zero

    const auto currentTime = myHistory.getCursor().tickTimePosition;
    MX_ASSERT(currentTime >= 0);
    const auto proposedNewTime = currentTime - inTicks;
    int safeTickAmount = inTicks;

    if (proposedNewTime < 0)
    {
        safeTickAmount += proposedNewTime;
    }

    MX_ASSERT(safeTickAmount >= 0);

    if (safeTickAmount == 0)
    {
        return;
    }

    core::Backup b{};
    b.setDuration(core::PositiveDivisions{core::Decimal{static_cast<double>(safeTickAmount)}});
    myOutMeasure.addMusicData(core::MusicDataChoice::backup(b));
    myHistory.advanceTickTimePosition(-1 * safeTickAmount, "write backup");
}

void MeasureWriter::forward(const int ticks)
{
    core::Forward f{};
    f.setDuration(core::PositiveDivisions{core::Decimal{static_cast<double>(ticks)}});
    myOutMeasure.addMusicData(core::MusicDataChoice::forward(f));
    myHistory.advanceTickTimePosition(ticks, "write forward");
}

bool MeasureWriter::isAdvanceNeeded(const api::NoteData &currentNote, const NoteIter &inIter, const NoteIter &inEnd)
{
    if (currentNote.isChord)
    {
        auto nextNoteIter = inIter + 1;
        if (nextNoteIter == inEnd)
        {
            return true;
        }
        else
        {
            const auto nextNoteTime = nextNoteIter->tickTimePosition;
            if (nextNoteTime != currentNote.tickTimePosition)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

void MeasureWriter::advanceCursorIfNeeded(const api::NoteData &currentNote, const NoteIter &inIter,
                                          const NoteIter &inEnd)
{
    if (isAdvanceNeeded(currentNote, inIter, inEnd))
    {
        const auto curTime = std::max(myHistory.getCursor().tickTimePosition, 0);
        const auto duration = std::max(currentNote.durationData.durationTimeTicks, 0);
        const auto newTime = curTime + duration;
        myHistory.setTime(newTime, "advance cursor");
        myPreviousCursor = myHistory.getCursor();
    }
    else
    {
        myHistory.log("no advance");
    }
}

void MeasureWriter::writeDirections(dIter &directionIter, const dIter &directionEnd, const NoteIter &inNoteIter,
                                    const NoteIter &inNotesBegin, const NoteIter &inNotesEnd)
{
    if (directionIter == directionEnd)
    {
        return;
    }

    const bool isNotesEnd = inNoteIter == inNotesEnd;
    const bool isLastNote = isNotesEnd || (inNoteIter + 1 == inNotesEnd);
    const bool isFirstNote = !isLastNote && !isNotesEnd && (inNoteIter == inNotesBegin);
    const bool isOnlyNote = inNoteIter == inNotesBegin && isLastNote;
    const auto previousNote = (isFirstNote || isOnlyNote) ? inNotesEnd : (inNoteIter - 1);
    const auto nextNote = (isLastNote || isNotesEnd) ? inNotesEnd : (inNoteIter + 1);
    const bool isTickTimeSameAsPreviousNote = (isFirstNote || isOnlyNote || isNotesEnd)
                                                  ? false
                                                  : (inNoteIter->tickTimePosition == previousNote->tickTimePosition);
    const bool isIndependentNote = !isTickTimeSameAsPreviousNote;

    if (isFirstNote)
    {
        for (; directionIter != directionEnd &&
               directionIter->tickTimePosition <= std::max(0, inNoteIter->tickTimePosition);
             ++directionIter)
        {
            writeDirection(*directionIter);
        }

        return;
    }
    else if (isNotesEnd)
    {
        for (; directionIter != directionEnd; ++directionIter)
        {
            writeDirection(*directionIter);
        }

        return;
    }
    else
    {
        const bool isLastNoteOfChord =
            !isNotesEnd && !isLastNote &&
            (inNoteIter->isChord && inNoteIter->tickTimePosition != nextNote->tickTimePosition);
        const bool isChord = !isNotesEnd && inNoteIter->isChord;

        if (isChord && !isLastNoteOfChord && !isIndependentNote)
        {
            myHistory.log("note writing any directions because we are in the midst of a chord");
            return;
        }

        const int currentNoteEndTime =
            myHistory.getCursor().tickTimePosition + std::max(0, inNoteIter->durationData.durationTimeTicks);

        for (; directionIter != directionEnd && directionIter->tickTimePosition < currentNoteEndTime; ++directionIter)
        {
            writeDirection(*directionIter);
        }

        return;
    }
}

void MeasureWriter::writeDirection(const api::DirectionData &inDirectionData)
{
    if (myPropertiesWriter)
    {
        myPropertiesWriter->flushBuffer();
    }

    DirectionWriter directionWriter{inDirectionData, myHistory.getCursor()};
    auto mdcSet = directionWriter.getDirectionLikeThings();
    for (const auto &mdc : mdcSet)
    {
        myOutMeasure.addMusicData(mdc);
    }
    myHistory.log("addDirection cursorTime " + std::to_string(myHistory.getCursor().tickTimePosition) +
                  ", directionTime " + std::to_string(inDirectionData.tickTimePosition));
}

void MeasureWriter::writeBarlines(int tickTimePosition)
{
    for (; myBarlinesIter != myBarlinesEnd && myBarlinesIter->tickTimePosition <= tickTimePosition; ++myBarlinesIter)
    {
        core::Barline barlineElement{};

        if (myBarlinesIter->barlineType != api::BarlineType::unspecified &&
            myBarlinesIter->barlineType != api::BarlineType::unsupported)
        {
            core::BarStyleColor bsc{};
            bsc.setValue(myConverter.convert(myBarlinesIter->barlineType));
            barlineElement.setBarStyle(bsc);
        }

        if (myBarlinesIter->endingType != api::EndingType::none)
        {
            core::Ending ending{};
            ending.setType(myConverter.convert(myBarlinesIter->endingType));

            if (myBarlinesIter->endingNumber > 0)
            {
                core::EndingNumber en{};
                en.addValue(myBarlinesIter->endingNumber);
                ending.setNumber(en);
            }

            barlineElement.setEnding(ending);
        }

        if (myBarlinesIter->location != api::HorizontalAlignment::unspecified)
        {
            barlineElement.setLocation(myConverter.convertBarlinePlacement(myBarlinesIter->location));
        }
        else if (myBarlinesIter->location == api::HorizontalAlignment::left || myBarlinesIter->tickTimePosition == 0)
        {
            barlineElement.setLocation(myConverter.convertBarlinePlacement(mx::api::HorizontalAlignment::left));
        }
        else
        {
            barlineElement.setLocation(myConverter.convertBarlinePlacement(mx::api::HorizontalAlignment::right));
        }

        if (myBarlinesIter->repeat)
        {
            core::Repeat repeatElement{};

            if (myBarlinesIter->location == api::HorizontalAlignment::left || myBarlinesIter->tickTimePosition == 0)
            {
                repeatElement.setDirection(mx::core::BackwardForward::forward());
            }
            else
            {
                repeatElement.setDirection(mx::core::BackwardForward::backward());
            }

            barlineElement.setRepeat(repeatElement);
        }

        myOutMeasure.addMusicData(core::MusicDataChoice::barline(barlineElement));
        myHistory.log("writeBarline");
    }
}

void MeasureWriter::History::advanceTickTimePosition(int amount, std::string reason)
{
    HistoryRecord record;
    record.reason = std::move(reason);
    record.amount = amount;
    record.timeBefore = myCursor.tickTimePosition;
    record.cursorBefore = myCursor;
    myCursor.tickTimePosition += amount;
    record.cursorAfter = myCursor;
    record.timeAfter = myCursor.tickTimePosition;
    historyPush(record);
}
} // namespace impl
} // namespace mx
