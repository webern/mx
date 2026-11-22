// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartReader.h"
#include "mx/core/generated/AccidentalText.h"
#include "mx/core/generated/AccidentalValue.h"
#include "mx/core/generated/FormattedText.h"
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
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureReader.h"
#include "mx/impl/NameDisplayFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/VirtualInstrumentFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/utility/Unused.h"

#include <sstream>

namespace mx
{
namespace impl
{
// True when a <part-name>/<part-abbreviation> carries any of the formatting
// attributes that MusicXML 2.0 deprecated in favor of the *-display elements.
bool PartReader::nameHasDeprecatedFormatting(const core::PartName &name)
{
    return name.fontFamily().has_value() || name.fontStyle().has_value() || name.fontSize().has_value() ||
           name.fontWeight().has_value() || name.color().has_value() || name.defaultX().has_value() ||
           name.defaultY().has_value() || name.relativeX().has_value() || name.relativeY().has_value() ||
           name.justify().has_value();
}

// Reads a name/abbreviation's display text and formatting into the api's single
// display model, following the deprecation rules documented in api/PartData.h:
// a present *-display element is canonical and wins; otherwise any deprecated
// formatting on the name element itself is migrated into the display model so it
// is re-emitted at the modern location.
void PartReader::readNameDisplay(const core::PartName &nameElement, const std::optional<core::NameDisplay> &display,
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

PartReader::PartReader(const core::ScorePart &inScorePart, const core::PartwisePart &inPartwisePartRef,
                       int globalTicksPerMeasure, int partIndex, int inDivisionsValue, DiagnosticsContext diagnostics)
    : myPartwisePart{inPartwisePartRef}, myScorePart{inScorePart}, myNumStaves{-1}, myIsStavesElementPresent{false},
      myGlobalTicksPerMeasure{globalTicksPerMeasure}, myPartIndex{partIndex},
      myConstructedDivisionsValue{inDivisionsValue}, myDiagnostics{diagnostics}
{
    const auto ppId = myPartwisePart.id().value();
    const auto spId = myScorePart.id().value();
    if (ppId != spId)
    {
        MX_THROW("the partwise-part id must match the score-part id");
    }
    myNumStaves = calculateNumStaves(myIsStavesElementPresent);
}

api::PartData PartReader::getPartData()
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutPartData = api::PartData{};
    parseScorePart();

    // A single-staff part does not need <staves>, so mx omits it there. Record an override only
    // when the source spelled the redundant element out.
    myOutPartData.writeSingleStaffCount = myIsStavesElementPresent && myNumStaves == 1;

    myCurrentCursor = MeasureCursor{myNumStaves, myGlobalTicksPerMeasure};
    myCurrentCursor.partIndex = myPartIndex;
    myCurrentCursor.instrumentId = myOutPartData.instrumentData.uniqueId;

    if (myConstructedDivisionsValue > 0)
    {
        myCurrentCursor.ticksPerQuarter = myConstructedDivisionsValue;
    }

    myPreviousCursor = myCurrentCursor;

    for (const auto &mxMeasure : myPartwisePart.measure())
    {
        MeasureReader reader{mxMeasure, myCurrentCursor, myPreviousCursor, myDiagnostics};
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
        myCurrentCursor.staffTimeSignatures = measureData.staffTimeSignatures;
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

int PartReader::calculateNumStaves(bool &outIsStavesElementPresent) const
{
    outIsStavesElementPresent = false;
    int numStaves = 1;
    int declaredStaves = 1;

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
                    outIsStavesElementPresent = true;
                    int temp = attributes.staves()->value();
                    declaredStaves = std::max(declaredStaves, temp);
                    if (temp > numStaves)
                    {
                        numStaves = temp;
                    }
                }
                for (const auto &clef : attributes.clef())
                {
                    if (clef.number().has_value() && clef.number()->value() > numStaves)
                    {
                        numStaves = clef.number()->value();
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

    if (numStaves > declaredStaves)
    {
        api::Location location;
        location.partIndex = myPartIndex;
        myDiagnostics.report(api::Severity::warning, api::DiagnosticCode::valueAdjusted, std::move(location),
                             "staff number " + std::to_string(numStaves) + " is above the staff count " +
                                 std::to_string(declaredStaves) + "; reading the part with " +
                                 std::to_string(numStaves) + " staves");
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

    for (const auto &group : myScorePart.group())
    {
        myOutPartData.groups.push_back(group);
    }

    const auto scoreInstruments = myScorePart.scoreInstrument();
    for (size_t i = 0; i < scoreInstruments.size(); ++i)
    {
        if (i == 0)
        {
            parseScoreInstrument(scoreInstruments[i], myOutPartData.instrumentData);
        }
        else
        {
            parseScoreInstrument(scoreInstruments[i], myOutPartData.additionalInstruments.emplace_back());
        }
    }

    // Each midi group names the instrument it plays by id. A first group that names no instrument
    // of the part belongs to the part's instrument; any other such group is not read.
    const auto midiGroups = myScorePart.midiGroup();
    for (size_t i = 0; i < midiGroups.size(); ++i)
    {
        bool isAdditional = false;
        api::InstrumentData *instrument = findMidiGroupInstrument(midiGroups[i], isAdditional);
        if (instrument == nullptr && i == 0)
        {
            instrument = &myOutPartData.instrumentData;
        }

        if (instrument != nullptr)
        {
            parseMidiDeviceInstrumentGroup(midiGroups[i], isAdditional, *instrument);
        }
    }
}

void PartReader::parseScoreInstrument(const core::ScoreInstrument &scoreInstrument,
                                      api::InstrumentData &outInstrument) const
{
    outInstrument.uniqueId = scoreInstrument.id().value();
    outInstrument.name = scoreInstrument.instrumentName();

    if (scoreInstrument.instrumentAbbreviation().has_value())
    {
        outInstrument.abbreviation = *scoreInstrument.instrumentAbbreviation();
    }

    auto sound =
        readVirtualInstrumentData(scoreInstrument.virtualInstrumentData(), myDiagnostics, partLocation(myPartIndex));
    outInstrument.soundID = sound.soundID;
    outInstrument.soloOrEnsemble = sound.soloOrEnsemble;
    outInstrument.midiData.virtualLibrary = std::move(sound.virtualLibrary);
    outInstrument.midiData.virtualName = std::move(sound.virtualName);
}

api::InstrumentData *PartReader::findMidiGroupInstrument(const core::ScorePartMIDIGroup &grp,
                                                         bool &outIsAdditional) const
{
    std::string id;
    if (grp.midiInstrument().has_value())
    {
        id = grp.midiInstrument()->id().value();
    }
    else if (grp.midiDevice().has_value() && grp.midiDevice()->id().has_value())
    {
        id = grp.midiDevice()->id()->value();
    }

    outIsAdditional = false;
    if (id.empty())
    {
        return nullptr;
    }

    if (myOutPartData.instrumentData.uniqueId == id)
    {
        return &myOutPartData.instrumentData;
    }

    for (auto &additional : myOutPartData.additionalInstruments)
    {
        if (additional.uniqueId == id)
        {
            outIsAdditional = true;
            return &additional;
        }
    }

    return nullptr;
}

void PartReader::parseMidiDeviceInstrumentGroup(const core::ScorePartMIDIGroup &grp, bool isAdditional,
                                                api::InstrumentData &outInstrument) const
{
    auto &midiData = outInstrument.midiData;
    if (grp.midiDevice().has_value())
    {
        midiData.device = grp.midiDevice()->value();

        if (grp.midiDevice()->port().has_value())
        {
            midiData.devicePort = grp.midiDevice()->port()->value();
        }

        // The writer links an additional instrument's device to its instrument unless told not to,
        // and leaves the link out for the part's instrument unless told to.
        if (grp.midiDevice()->id().has_value())
        {
            midiData.writeDeviceId = api::Bool::yes;
        }
        else if (isAdditional)
        {
            midiData.writeDeviceId = api::Bool::no;
        }
    }

    if (grp.midiInstrument().has_value())
    {
        parseMidiInstrument(*grp.midiInstrument(), midiData);
    }
}

void PartReader::parseMidiInstrument(const core::MIDIInstrument &inst, api::MidiData &outMidiData) const
{
    if (inst.midiChannel().has_value())
    {
        outMidiData.channel = inst.midiChannel()->value();
    }

    if (inst.midiName().has_value())
    {
        outMidiData.name = *inst.midiName();
    }

    if (inst.midiBank().has_value())
    {
        outMidiData.bank = inst.midiBank()->value();
    }

    if (inst.midiProgram().has_value())
    {
        outMidiData.program = inst.midiProgram()->value();
    }

    if (inst.midiUnpitched().has_value())
    {
        outMidiData.unpitched = inst.midiUnpitched()->value();
    }

    if (inst.volume().has_value())
    {
        outMidiData.isVolumeSpecified = true;
        outMidiData.volume = inst.volume()->value().value();
    }

    if (inst.pan().has_value())
    {
        outMidiData.isPanSpecified = true;
        outMidiData.pan = inst.pan()->value().value();
    }

    if (inst.elevation().has_value())
    {
        outMidiData.isElevationSpecified = true;
        outMidiData.elevation = inst.elevation()->value().value();
    }
}

} // namespace impl
} // namespace mx
