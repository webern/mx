// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartWriter.h"
#include "mx/core/Decimal.h"
#include "mx/core/OneOrMore.h"
#include "mx/core/Token.h"
#include "mx/core/generated/Empty.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/InstrumentSound.h"
#include "mx/core/generated/MIDI128.h"
#include "mx/core/generated/MIDI16.h"
#include "mx/core/generated/MIDI16384.h"
#include "mx/core/generated/MIDIDevice.h"
#include "mx/core/generated/MIDIInstrument.h"
#include "mx/core/generated/NameDisplay.h"
#include "mx/core/generated/NameDisplayChoice.h"
#include "mx/core/generated/PartName.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PartwisePart.h"
#include "mx/core/generated/Percent.h"
#include "mx/core/generated/PositiveIntegerOrEmpty.h"
#include "mx/core/generated/RotationDegrees.h"
#include "mx/core/generated/ScoreInstrument.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartMIDIGroup.h"
#include "mx/core/generated/VirtualInstrument.h"
#include "mx/core/generated/VirtualInstrumentDataGroup.h"
#include "mx/core/generated/VirtualInstrumentDataGroupChoice.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/impl/MeasureWriter.h"
#include "mx/impl/NameDisplayFunctions.h"
#include "mx/impl/ScoreWriter.h"

#include <algorithm>
#include <atomic>
#include <sstream>

namespace mx
{
namespace impl
{
namespace
{
// Writes the print-object attribute onto a <part-name>/<part-abbreviation> only
// when the api specifies it; api::Bool::unspecified leaves the attribute off so
// the element defaults to shown.
void applyPrintObject(api::Bool printObject, core::PartName &out)
{
    if (printObject != api::Bool::unspecified)
    {
        Converter converter;
        out.setPrintObject(converter.convert(printObject));
    }
}
} // namespace

// Synthesized <score-instrument> ids, e.g. "ID1000000". Seeded high so they
// are unlikely to collide with ids already present in parsed documents. The
// sequence is shared process-wide so that instruments of different parts
// cannot collide inside one document.
int partWriterNextSynthesizedId()
{
    static std::atomic<int> nextId{1000000};
    return nextId.fetch_add(1);
}

PartWriter::PartWriter(const api::PartData &inPartData, int inPartIndex, int inTicksPerQuarter,
                       const ScoreWriter &inScoreWriter)
    : myPartData{inPartData}, myPartIndex{inPartIndex}, myTicksPerQuarter{inTicksPerQuarter}, myMutex{},
      myScoreWriter{inScoreWriter}
{
}

const DiagnosticsContext &PartWriter::diagnostics() const
{
    return myScoreWriter.getDiagnostics();
}

core::Token PartWriter::writtenToken(const char *name, const std::string &text) const
{
    core::ValueParseOutcome outcome = core::ValueParseOutcome::valid;
    auto token = core::Token::parse(text, outcome);
    if (outcome != core::ValueParseOutcome::valid)
    {
        diagnostics().report(api::Severity::warning, api::DiagnosticCode::invalidValue, partLocation(myPartIndex),
                             std::string{name} + " \"" + text + "\" is not a valid id; using \"" + token.value() +
                                 "\"");
    }
    return token;
}

core::ScorePart PartWriter::getScorePart() const
{
    core::ScorePart scorePart{};
    scorePart.setID(writtenToken("part id", myPartData.uniqueId));

    // <part-name> is required, so always write it. print-object is round-
    // tripped from the model (not force-hidden); deprecated formatting is never
    // written here -- it goes to <part-name-display> below. See api/PartData.h.
    core::PartName partName{};
    partName.setValue(myPartData.name);
    applyPrintObject(myPartData.namePrintObject, partName);
    scorePart.setPartName(partName);

    if (myPartData.abbreviation.size() > 0)
    {
        core::PartName abbrev{};
        abbrev.setValue(myPartData.abbreviation);
        applyPrintObject(myPartData.abbreviationPrintObject, abbrev);
        scorePart.setPartAbbreviation(abbrev);
    }

    if (myPartData.displayName.size() > 0)
    {
        scorePart.setPartNameDisplay(makeNameDisplay(myPartData.displayName, myPartData.displayNamePrintData,
                                                     myPartData.displayNamePositionData));
    }

    if (myPartData.displayAbbreviation.size() > 0)
    {
        scorePart.setPartAbbreviationDisplay(makeNameDisplay(myPartData.displayAbbreviation,
                                                             myPartData.displayAbbreviationPrintData,
                                                             myPartData.displayAbbreviationPositionData));
    }

    for (const auto &group : myPartData.groups)
    {
        scorePart.addGroup(group);
    }

    core::ScoreInstrument scoreInstrument{};
    bool addScoreInstrument = false;
    scoreInstrument.setID(myPartData.instrumentData.uniqueId.empty()
                              ? core::Token{}
                              : writtenToken("instrument id", myPartData.instrumentData.uniqueId));

    if (myPartData.instrumentData.name.size() > 0)
    {
        addScoreInstrument = true;
        scoreInstrument.setInstrumentName(myPartData.instrumentData.name);
    }

    if (myPartData.instrumentData.abbreviation.size() > 0)
    {
        addScoreInstrument = true;
        scoreInstrument.setInstrumentAbbreviation(myPartData.instrumentData.abbreviation);
    }

    if (myPartData.instrumentData.soloOrEnsemble != api::SoloOrEnsemble::unspecified)
    {
        addScoreInstrument = true;
        core::VirtualInstrumentDataGroup vidg = scoreInstrument.virtualInstrumentData();
        if (myPartData.instrumentData.soloOrEnsemble == api::SoloOrEnsemble::ensemble)
        {
            vidg.setChoice(
                core::VirtualInstrumentDataGroupChoice::ensemble(core::PositiveIntegerOrEmpty::positiveInteger(0)));
        }
        else
        {
            vidg.setChoice(core::VirtualInstrumentDataGroupChoice::solo(core::Empty{}));
        }
        scoreInstrument.setVirtualInstrumentData(vidg);
    }

    if (myPartData.instrumentData.midiData.virtualName.size() > 0 ||
        myPartData.instrumentData.midiData.virtualLibrary.size() > 0)
    {
        addScoreInstrument = true;
        core::VirtualInstrumentDataGroup vidg = scoreInstrument.virtualInstrumentData();
        core::VirtualInstrument virtualInstrument{};

        if (myPartData.instrumentData.midiData.virtualName.size() > 0)
        {
            virtualInstrument.setVirtualName(myPartData.instrumentData.midiData.virtualName);
        }

        if (myPartData.instrumentData.midiData.virtualLibrary.size() > 0)
        {
            virtualInstrument.setVirtualLibrary(myPartData.instrumentData.midiData.virtualLibrary);
        }

        vidg.setVirtualInstrument(virtualInstrument);
        scoreInstrument.setVirtualInstrumentData(vidg);
    }

    if (myPartData.instrumentData.soundID != api::SoundID::unspecified &&
        myPartData.instrumentData.soundID != api::SoundID::errorBadString)
    {
        addScoreInstrument = true;
        Converter c;
        core::VirtualInstrumentDataGroup vidg = scoreInstrument.virtualInstrumentData();
        vidg.setInstrumentSound(core::InstrumentSound::soundID(c.convert(myPartData.instrumentData.soundID)));
        scoreInstrument.setVirtualInstrumentData(vidg);
    }

    if (addScoreInstrument)
    {
        scorePart.addScoreInstrument(scoreInstrument);
    }

    bool addMidiElement = false;
    core::ScorePartMIDIGroup midiGroup{};
    core::MIDIDevice midiDevice{};
    core::MIDIInstrument midiInstrument{};
    midiInstrument.setID(core::Token{myPartData.instrumentData.uniqueId});

    const auto &apiMidiData = myPartData.instrumentData.midiData;
    if (apiMidiData.device.size() > 0 || apiMidiData.devicePort.has_value())
    {
        addMidiElement = true;
        midiDevice.setValue(apiMidiData.device);

        if (apiMidiData.devicePort.has_value())
        {
            const core::MIDI16 port{*apiMidiData.devicePort};
            reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-device port", *apiMidiData.devicePort,
                           port.value());
            midiDevice.setPort(port);
        }

        // The midi-device attaches to this part's instrument, the same instrument the
        // midi-instrument below is written for. Emit that link only when the source stated it;
        // for a single-instrument part it is otherwise implied.
        if (apiMidiData.writeDeviceId == api::Bool::yes && myPartData.instrumentData.uniqueId.size() > 0)
        {
            midiDevice.setID(core::Token{myPartData.instrumentData.uniqueId});
        }

        midiGroup.setMIDIDevice(midiDevice);
    }

    if (myPartData.instrumentData.midiData.name.size() > 0)
    {
        addMidiElement = true;
        midiInstrument.setMIDIName(myPartData.instrumentData.midiData.name);
    }

    if (myPartData.instrumentData.midiData.bank >= 0)
    {
        addMidiElement = true;
        const core::MIDI16384 bank{myPartData.instrumentData.midiData.bank};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-bank", myPartData.instrumentData.midiData.bank,
                       bank.value());
        midiInstrument.setMIDIBank(bank);
    }

    if (myPartData.instrumentData.midiData.channel >= 0)
    {
        addMidiElement = true;
        const core::MIDI16 channel{myPartData.instrumentData.midiData.channel};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-channel",
                       myPartData.instrumentData.midiData.channel, channel.value());
        midiInstrument.setMIDIChannel(channel);
    }

    if (myPartData.instrumentData.midiData.program >= 0)
    {
        addMidiElement = true;
        const core::MIDI128 program{myPartData.instrumentData.midiData.program};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-program",
                       myPartData.instrumentData.midiData.program, program.value());
        midiInstrument.setMIDIProgram(program);
    }

    if (myPartData.instrumentData.midiData.isElevationSpecified)
    {
        addMidiElement = true;
        const core::RotationDegrees elevation{core::Decimal{myPartData.instrumentData.midiData.elevation}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "elevation",
                       myPartData.instrumentData.midiData.elevation, elevation.value().value());
        midiInstrument.setElevation(elevation);
    }

    if (myPartData.instrumentData.midiData.isPanSpecified)
    {
        addMidiElement = true;
        const core::RotationDegrees pan{core::Decimal{myPartData.instrumentData.midiData.pan}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "pan", myPartData.instrumentData.midiData.pan,
                       pan.value().value());
        midiInstrument.setPan(pan);
    }

    if (myPartData.instrumentData.midiData.isVolumeSpecified)
    {
        addMidiElement = true;
        const core::Percent volume{core::Decimal{myPartData.instrumentData.midiData.volume}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "volume", myPartData.instrumentData.midiData.volume,
                       volume.value().value());
        midiInstrument.setVolume(volume);
    }

    if (addMidiElement)
    {
        midiGroup.setMIDIInstrument(midiInstrument);
        scorePart.addMIDIGroup(midiGroup);
    }

    if (addMidiElement && !addScoreInstrument)
    {
        if (myPartData.instrumentData.uniqueId.size() == 0)
        {
            std::stringstream ss;
            ss << "ID";
            ss << partWriterNextSynthesizedId();
            scoreInstrument.setID(core::Token{ss.str()});
        }
        else
        {
            scoreInstrument.setID(core::Token{myPartData.instrumentData.uniqueId});
        }

        scorePart.addScoreInstrument(scoreInstrument);
    }

    return scorePart;
}

core::PartwisePart PartWriter::getPartwisePart() const
{
    core::PartwisePart partwisePart{};
    // getScorePart reports a scrubbed part id
    partwisePart.setID(core::Token{myPartData.uniqueId});
    writeMeasures(partwisePart);
    return partwisePart;
}

void PartWriter::writeMeasures(core::PartwisePart &outPart) const
{
    if (myPartData.measures.size() == 0)
    {
        // since we store the part's (at least initial) transposition in the PartData,
        // and because musicxml stores this in the measure data, if there are no measures
        // then the transposition would be lost. so instead we add a measure in order to
        // preserve the transposition data.
        if (myPartData.transposition.has_value())
        {
            auto copiedPart = myPartData;
            copiedPart.measures.emplace_back(mx::api::MeasureData{});
            writeMeasures(copiedPart, outPart);
        }
    }
    else
    {
        writeMeasures(myPartData, outPart);
    }
}

void PartWriter::writeMeasures(const mx::api::PartData &inPartData, core::PartwisePart &outPart) const
{
    // The cursor's staff count is a part-wide value, not a per-measure one: PartReader gives every
    // measure of a part the same (maximum) staff count on read (PartReader.cpp calculateNumStaves),
    // so the writer must decide "is this part multi-staff" the same way. Using only the first
    // measure's count here left later measures that introduce more staves writing their extra
    // staves' notes without <staff>, silently reading back on staff 1 (issue #442).
    int partStaffCount = 0;
    for (const auto &measure : inPartData.measures)
    {
        partStaffCount = std::max(partStaffCount, static_cast<int>(measure.staves.size()));
    }

    MeasureCursor cursor{partStaffCount, myTicksPerQuarter};
    cursor.measureIndex = 0;
    cursor.partIndex = myPartIndex;
    cursor.isFirstMeasureInPart = true;

    // Collect all measures before setting them, to avoid the OneOrMore default-element
    // replacement logic that the old shared-ptr API required.
    std::vector<core::PartwiseMeasure> measures{};
    measures.reserve(inPartData.measures.size());

    for (const auto &measure : inPartData.measures)
    {
        MeasureWriter writer{measure, cursor, myScoreWriter};
        measures.push_back(writer.getPartwiseMeasure());
        cursor.isFirstMeasureInPart = false;
        ++cursor.measureIndex;
    }

    if (!measures.empty())
    {
        core::OneOrMore<core::PartwiseMeasure> oneOrMore{measures.front()};
        for (std::size_t i = 1; i < measures.size(); ++i)
        {
            oneOrMore.add(std::move(measures[i]));
        }
        outPart.setMeasure(std::move(oneOrMore));
    }
}
} // namespace impl
} // namespace mx
