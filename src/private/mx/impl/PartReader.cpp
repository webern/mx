// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartReader.h"
#include "mx/core/generated/AccidentalText.h"
#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/InstrumentSound.h"
#include "mx/core/generated/MIDIDevice.h"
#include "mx/core/generated/MIDIInstrument.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NameDisplay.h"
#include "mx/core/generated/NameDisplayChoice.h"
#include "mx/core/generated/PartList.h"
#include "mx/core/generated/PartListChoice.h"
#include "mx/core/generated/PartName.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/core/generated/ScoreInstrument.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartMIDIGroup.h"
#include "mx/core/generated/ScorePartwise.h"
#include "mx/core/generated/SoundID.h"
#include "mx/core/generated/VirtualInstrument.h"
#include "mx/core/generated/VirtualInstrumentDataGroup.h"
#include "mx/core/generated/VirtualInstrumentDataGroupChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureReader.h"
#include "mx/impl/NameDisplayFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

#include <sstream>

namespace mx
{
namespace impl
{
namespace
{
// True when a <part-name>/<part-abbreviation> carries any of the formatting
// attributes that MusicXML 2.0 deprecated in favor of the *-display elements.
bool nameHasDeprecatedFormatting(const core::PartName &n)
{
    return n.fontFamily().has_value() || n.fontStyle().has_value() || n.fontSize().has_value() ||
           n.fontWeight().has_value() || n.color().has_value() || n.defaultX().has_value() ||
           n.defaultY().has_value() || n.relativeX().has_value() || n.relativeY().has_value() ||
           n.justify().has_value();
}

// Reads a name/abbreviation's display text and formatting into the api's single
// display model, following the deprecation rules documented in api/PartData.h:
// a present *-display element is canonical and wins; otherwise any deprecated
// formatting on the name element itself is migrated into the display model so it
// is re-emitted at the modern location.
void readNameDisplay(const core::PartName &nameElement, const std::optional<core::NameDisplay> &display,
                     std::string &outText, api::PrintData &outPrintData, api::PositionData &outPositionData)
{
    if (display.has_value())
    {
        outText = extractDisplayText(*display);
        extractDisplayFormatting(*display, outPrintData, outPositionData);
    }
    else if (nameHasDeprecatedFormatting(nameElement))
    {
        outText = nameElement.value();
        outPrintData = getPrintData(nameElement);
        // print-object belongs to the name element, not the display run.
        outPrintData.printObject = api::Bool::unspecified;
        outPositionData = getPositionData(nameElement);
    }
}
} // namespace

PartReader::PartReader(const core::ScorePart &inScorePart, const core::PartwisePart &inPartwisePartRef,
                       int globalTicksPerMeasure, const core::ScorePartwise &inScore, int inDivisionsValue)
    : myPartwisePart{inPartwisePartRef}, myScorePart{inScorePart}, myNumStaves{-1},
      myGlobalTicksPerMeasure{globalTicksPerMeasure}, myScore{inScore}, myPartIndex{-1},
      myConstructedDivisionsValue{inDivisionsValue}
{
    const auto ppId = myPartwisePart.id().value();
    const auto spId = myScorePart.id().value();
    if (ppId != spId)
    {
        MX_THROW("the partwise-part id must match the score-part id");
    }
    const auto partIndex = findPartIndex(ppId);
    MX_ASSERT(partIndex >= 0);
    myPartIndex = partIndex;
    myNumStaves = calculateNumStaves();
}

api::PartData PartReader::getPartData()
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutPartData = api::PartData{};
    parseScorePart();

    myCurrentCursor = MeasureCursor{myNumStaves, myGlobalTicksPerMeasure};
    myCurrentCursor.partIndex = myPartIndex;

    if (myConstructedDivisionsValue > 0)
    {
        myCurrentCursor.ticksPerQuarter = myConstructedDivisionsValue;
    }

    myPreviousCursor = myCurrentCursor;

    for (const auto &mxMeasure : myPartwisePart.measure())
    {
        MeasureReader reader{mxMeasure, myCurrentCursor, myPreviousCursor};
        // the reader returns the measure data and any data that needs to be written at
        // the part-level (e.g. transposition). currently this is done as a pair.
        auto measureDataPair = reader.getMeasureData();
        auto measureData = measureDataPair.first;
        auto &transpositionData = measureDataPair.second;
        // if it's the first measure, and if we received transposition data, then we
        // should write it to the part.
        if (myCurrentCursor.isFirstMeasureInPart && transpositionData.has_value())
        {
            myOutPartData.transposition = transpositionData;
        }
        myCurrentCursor.timeSignature = measureData.timeSignature;
        myCurrentCursor.ticksPerQuarter = reader.getCursor().ticksPerQuarter;
        myOutPartData.measures.emplace_back(std::move(measureData));
        ++myCurrentCursor.measureIndex;
        myPreviousCursor = myCurrentCursor;
    }

    api::PartData tempReturn = std::move(myOutPartData);
    myOutPartData = api::PartData{};
    return tempReturn;
}

MeasureCursor PartReader::getCursor() const
{
    return myCurrentCursor;
}

int PartReader::calculateNumStaves() const
{
    int numStaves = 1;

    for (const auto &measure : myPartwisePart.measure())
    {
        for (const auto &mdc : measure.musicData())
        {
            switch (mdc.kind())
            {
            case core::MusicDataChoice::Kind::note: {
                updateNumStaves(mdc.asNote(), numStaves);
                break;
            }
            case core::MusicDataChoice::Kind::forward: {
                updateNumStaves(mdc.asForward(), numStaves);
                break;
            }
            case core::MusicDataChoice::Kind::direction: {
                updateNumStaves(mdc.asDirection(), numStaves);
                break;
            }
            case core::MusicDataChoice::Kind::attributes: {
                const auto &attributes = mdc.asAttributes();
                if (attributes.staves().has_value())
                {
                    int temp = *attributes.staves();
                    if (temp > numStaves)
                    {
                        numStaves = temp;
                    }
                }
                break;
            }
            case core::MusicDataChoice::Kind::harmony: {
                updateNumStaves(mdc.asHarmony(), numStaves);
                break;
            }
            // these are not specific to a staff number
            case core::MusicDataChoice::Kind::backup:
            case core::MusicDataChoice::Kind::figuredBass:
            case core::MusicDataChoice::Kind::print:
            case core::MusicDataChoice::Kind::sound:
            case core::MusicDataChoice::Kind::listening:
            case core::MusicDataChoice::Kind::barline:
            case core::MusicDataChoice::Kind::grouping:
            case core::MusicDataChoice::Kind::link:
            case core::MusicDataChoice::Kind::bookmark:
            default:
                break;
            }
        }
    }

    return numStaves;
}

void PartReader::parseScorePart() const
{
    myOutPartData.uniqueId = myScorePart.id().value();

    const auto &corePartName = myScorePart.partName();
    myOutPartData.name = corePartName.value();
    myOutPartData.namePrintObject = getPrintObject(corePartName);
    readNameDisplay(corePartName, myScorePart.partNameDisplay(), myOutPartData.displayName,
                    myOutPartData.displayNamePrintData, myOutPartData.displayNamePositionData);

    if (myScorePart.partAbbreviation().has_value())
    {
        const auto &coreAbbreviation = *myScorePart.partAbbreviation();
        myOutPartData.abbreviation = coreAbbreviation.value();
        myOutPartData.abbreviationPrintObject = getPrintObject(coreAbbreviation);
        readNameDisplay(coreAbbreviation, myScorePart.partAbbreviationDisplay(), myOutPartData.displayAbbreviation,
                        myOutPartData.displayAbbreviationPrintData, myOutPartData.displayAbbreviationPositionData);
    }
    else if (myScorePart.partAbbreviationDisplay().has_value())
    {
        myOutPartData.displayAbbreviation = extractDisplayText(*myScorePart.partAbbreviationDisplay());
        extractDisplayFormatting(*myScorePart.partAbbreviationDisplay(), myOutPartData.displayAbbreviationPrintData,
                                 myOutPartData.displayAbbreviationPositionData);
    }

    if (!myScorePart.scoreInstrument().empty())
    {
        parseScoreInstrument(myScorePart.scoreInstrument().front());
    }

    if (!myScorePart.midiGroup().empty())
    {
        parseMidiDeviceInstrumentGroup(myScorePart.midiGroup().front());
    }
}

void PartReader::parseScoreInstrument(const core::ScoreInstrument &scoreInstrument) const
{
    myOutPartData.instrumentData.uniqueId = scoreInstrument.id().value();
    myOutPartData.instrumentData.name = scoreInstrument.instrumentName();

    if (scoreInstrument.instrumentAbbreviation().has_value())
    {
        myOutPartData.instrumentData.abbreviation = *scoreInstrument.instrumentAbbreviation();
    }

    const auto &vidg = scoreInstrument.virtualInstrumentData();

    if (vidg.choice().has_value())
    {
        const auto &choice = *vidg.choice();
        if (choice.isEnsemble())
        {
            myOutPartData.instrumentData.soloOrEnsemble = api::SoloOrEnsemble::ensemble;
        }
        else if (choice.isSolo())
        {
            myOutPartData.instrumentData.soloOrEnsemble = api::SoloOrEnsemble::solo;
        }
    }

    if (vidg.instrumentSound().has_value())
    {
        const auto &instrSound = *vidg.instrumentSound();
        if (instrSound.isSoundID())
        {
            Converter c;
            myOutPartData.instrumentData.soundID = c.convert(instrSound.asSoundID());
        }
    }

    if (vidg.virtualInstrument().has_value())
    {
        parseVirtualInstrument(*vidg.virtualInstrument());
    }
}

void PartReader::parseVirtualInstrument(const core::VirtualInstrument &virtualInstrument) const
{
    if (virtualInstrument.virtualName().has_value())
    {
        myOutPartData.instrumentData.midiData.virtualName = *virtualInstrument.virtualName();
    }

    if (virtualInstrument.virtualLibrary().has_value())
    {
        myOutPartData.instrumentData.midiData.virtualLibrary = *virtualInstrument.virtualLibrary();
    }
}

void PartReader::parseMidiDeviceInstrumentGroup(const core::ScorePartMIDIGroup &grp) const
{
    if (grp.midiDevice().has_value())
    {
        myOutPartData.instrumentData.midiData.device = grp.midiDevice()->value();
    }

    if (grp.midiInstrument().has_value())
    {
        parseMidiInstrument(*grp.midiInstrument());
    }
}

void PartReader::parseMidiInstrument(const core::MIDIInstrument &inst) const
{
    if (inst.midiChannel().has_value())
    {
        myOutPartData.instrumentData.midiData.channel = inst.midiChannel()->value();
    }

    if (inst.midiName().has_value())
    {
        myOutPartData.instrumentData.midiData.name = *inst.midiName();
    }

    if (inst.midiBank().has_value())
    {
        myOutPartData.instrumentData.midiData.bank = inst.midiBank()->value();
    }

    if (inst.midiProgram().has_value())
    {
        myOutPartData.instrumentData.midiData.program = inst.midiProgram()->value();
    }

    if (inst.midiUnpitched().has_value())
    {
        myOutPartData.instrumentData.midiData.unpitched = inst.midiUnpitched()->value();
    }

    if (inst.volume().has_value())
    {
        myOutPartData.instrumentData.midiData.isVolumeSpecified = true;
        myOutPartData.instrumentData.midiData.volume = inst.volume()->value().value();
    }

    if (inst.pan().has_value())
    {
        myOutPartData.instrumentData.midiData.isPanSpecified = true;
        myOutPartData.instrumentData.midiData.pan = inst.pan()->value().value();
    }

    if (inst.elevation().has_value())
    {
        myOutPartData.instrumentData.midiData.isElevationSpecified = true;
        myOutPartData.instrumentData.midiData.elevation = inst.elevation()->value().value();
    }
}

int PartReader::findPartIndex(const std::string &inPartId) const
{
    const auto &partList = myScore.scoreHeader().partList();
    const auto &firstPart = partList.scorePart();
    int index = 0;

    if (firstPart.id().value() == inPartId)
    {
        return index;
    }

    ++index;

    for (const auto &p : partList.choice())
    {
        if (p.isScorePart())
        {
            if (p.asScorePart().id().value() == inPartId)
            {
                return index;
            }
            ++index;
        }
    }
    return -1;
}
} // namespace impl
} // namespace mx
