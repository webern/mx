// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartWriter.h"
#include "mx/core/Decimal.h"
#include "mx/core/OneOrMore.h"
#include "mx/core/Token.h"
#include "mx/core/generated/FormattedText.h"
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
#include "mx/core/generated/RotationDegrees.h"
#include "mx/core/generated/ScoreInstrument.h"
#include "mx/core/generated/ScorePart.h"
#include "mx/core/generated/ScorePartMIDIGroup.h"
#include "mx/core/generated/VirtualInstrumentDataGroup.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/impl/MeasureWriter.h"
#include "mx/impl/NameDisplayFunctions.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/VirtualInstrumentFunctions.h"

#include <algorithm>

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

bool partWriterHasInstrumentChange(const api::PartData &inPartData)
{
    for (const auto &measure : inPartData.measures)
    {
        for (const auto &staff : measure.staves)
        {
            for (const auto &direction : staff.directions)
            {
                if (direction.isSoundDataSpecified && direction.soundData.instrumentChange.has_value())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool partWriterHasNoteInstrument(const api::PartData &inPartData, const std::string &instrumentId)
{
    if (instrumentId.empty())
    {
        return false;
    }

    for (const auto &measure : inPartData.measures)
    {
        for (const auto &staff : measure.staves)
        {
            for (const auto &voice : staff.voices)
            {
                for (const auto &note : voice.second.notes)
                {
                    const auto &ids = note.instruments;
                    if (std::find(ids.cbegin(), ids.cend(), instrumentId) != ids.cend())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// The base of the ids given to parts that bring no instrument id of their own, e.g. "ID1000000".
// High enough to be unlikely to collide with ids already present in a parsed score.
constexpr int kSynthesizedInstrumentIdBase = 1000000;

// The id the part's instrument carries, repeated by every element that refers to it. The
// caller's InstrumentData::uniqueId when it set one; otherwise derived from the part's place in
// the score, so the same ScoreData always writes the same file, and two parts never collide.
core::Token PartWriter::partInstrumentId(bool isReported) const
{
    if (!myPartData.instrumentData.uniqueId.empty())
    {
        if (isReported)
        {
            return writtenToken("instrument id", myPartData.instrumentData.uniqueId);
        }
        core::ValueParseOutcome outcome = core::ValueParseOutcome::valid;
        return core::Token::parse(myPartData.instrumentData.uniqueId, outcome);
    }
    return core::Token{"ID" + std::to_string(kSynthesizedInstrumentIdBase + myPartIndex)};
}

// The id an additional instrument carries: the caller's uniqueId when it set one; otherwise derived
// from the part's place in the score and the instrument's place in the part, e.g. "ID1000000-1".
core::Token PartWriter::additionalInstrumentId(size_t index) const
{
    const auto &uniqueId = myPartData.additionalInstruments.at(index).uniqueId;
    if (!uniqueId.empty())
    {
        return writtenToken("instrument id", uniqueId);
    }
    return core::Token{"ID" + std::to_string(kSynthesizedInstrumentIdBase + myPartIndex) + "-" +
                       std::to_string(index + 1)};
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

    const core::Token instrumentId = partInstrumentId();
    bool addScoreInstrument = false;
    const core::ScoreInstrument scoreInstrument =
        makeScoreInstrument(myPartData.instrumentData, instrumentId, addScoreInstrument);

    // An instrument change refers to the part's score-instrument by id, so the part needs one.
    if (partWriterHasInstrumentChange(myPartData))
    {
        addScoreInstrument = true;
    }

    // The first score-instrument is the part's instrument, so it is written ahead of any additional
    // instrument. A note can name it too.
    if (!myPartData.additionalInstruments.empty() ||
        partWriterHasNoteInstrument(myPartData, myPartData.instrumentData.uniqueId))
    {
        addScoreInstrument = true;
    }

    // Once the part has playback data, a score-instrument is required, so write one even when
    // the caller described no instrument. It already carries the id the midi elements refer to,
    // which is what makes that reference resolve.
    if (addMidiGroup(myPartData.instrumentData.midiData, instrumentId, false, scorePart))
    {
        addScoreInstrument = true;
    }

    if (addScoreInstrument)
    {
        scorePart.addScoreInstrument(scoreInstrument);
    }

    for (size_t i = 0; i < myPartData.additionalInstruments.size(); ++i)
    {
        const auto &additional = myPartData.additionalInstruments[i];
        const core::Token additionalId = additionalInstrumentId(i);
        bool hasContent = false;
        scorePart.addScoreInstrument(makeScoreInstrument(additional, additionalId, hasContent));
        addMidiGroup(additional.midiData, additionalId, true, scorePart);
    }

    return scorePart;
}

core::ScoreInstrument PartWriter::makeScoreInstrument(const api::InstrumentData &instrument, const core::Token &id,
                                                      bool &outHasContent) const
{
    core::ScoreInstrument scoreInstrument{};
    scoreInstrument.setID(id);

    if (instrument.name.size() > 0)
    {
        outHasContent = true;
        scoreInstrument.setInstrumentName(instrument.name);
    }

    if (instrument.abbreviation.size() > 0)
    {
        outHasContent = true;
        scoreInstrument.setInstrumentAbbreviation(instrument.abbreviation);
    }

    api::InstrumentChangeData sound{};
    sound.soundID = instrument.soundID;
    sound.soloOrEnsemble = instrument.soloOrEnsemble;
    sound.virtualLibrary = instrument.midiData.virtualLibrary;
    sound.virtualName = instrument.midiData.virtualName;
    scoreInstrument.setVirtualInstrumentData(writeVirtualInstrumentData(sound));

    if (scoreInstrument.virtualInstrumentData() != core::VirtualInstrumentDataGroup{})
    {
        outHasContent = true;
    }

    return scoreInstrument;
}

bool PartWriter::addMidiGroup(const api::MidiData &midiData, const core::Token &id, bool isAdditional,
                              core::ScorePart &outScorePart) const
{
    bool addMidiElement = false;
    core::ScorePartMIDIGroup midiGroup{};
    core::MIDIDevice midiDevice{};
    core::MIDIInstrument midiInstrument{};
    midiInstrument.setID(id);

    if (midiData.device.size() > 0 || midiData.devicePort.has_value())
    {
        addMidiElement = true;
        midiDevice.setValue(midiData.device);

        if (midiData.devicePort.has_value())
        {
            const core::MIDI16 port{*midiData.devicePort};
            reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-device port", *midiData.devicePort,
                           port.value());
            midiDevice.setPort(port);
        }

        // The midi-device attaches to the same instrument the midi-instrument below is written for.
        // The part's instrument is implied, so its link is written only when asked for. Without the
        // link, a device applies to every instrument of the part, so an additional instrument's link
        // is written unless asked not to be.
        const bool writeId =
            midiData.writeDeviceId == api::Bool::yes || (isAdditional && midiData.writeDeviceId != api::Bool::no);
        if (writeId)
        {
            midiDevice.setID(id);
        }

        midiGroup.setMIDIDevice(midiDevice);
    }

    if (midiData.name.size() > 0)
    {
        addMidiElement = true;
        midiInstrument.setMIDIName(midiData.name);
    }

    if (midiData.bank >= 0)
    {
        addMidiElement = true;
        const core::MIDI16384 bank{midiData.bank};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-bank", midiData.bank, bank.value());
        midiInstrument.setMIDIBank(bank);
    }

    if (midiData.channel >= 0)
    {
        addMidiElement = true;
        const core::MIDI16 channel{midiData.channel};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-channel", midiData.channel, channel.value());
        midiInstrument.setMIDIChannel(channel);
    }

    if (midiData.program >= 0)
    {
        addMidiElement = true;
        const core::MIDI128 program{midiData.program};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-program", midiData.program, program.value());
        midiInstrument.setMIDIProgram(program);
    }

    if (midiData.unpitched != api::VALUE_UNSPECIFIED)
    {
        addMidiElement = true;
        const core::MIDI128 unpitched{midiData.unpitched};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "midi-unpitched", midiData.unpitched,
                       unpitched.value());
        midiInstrument.setMIDIUnpitched(unpitched);
    }

    if (midiData.isElevationSpecified)
    {
        addMidiElement = true;
        const core::RotationDegrees elevation{core::Decimal{midiData.elevation}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "elevation", midiData.elevation,
                       elevation.value().value());
        midiInstrument.setElevation(elevation);
    }

    if (midiData.isPanSpecified)
    {
        addMidiElement = true;
        const core::RotationDegrees pan{core::Decimal{midiData.pan}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "pan", midiData.pan, pan.value().value());
        midiInstrument.setPan(pan);
    }

    if (midiData.isVolumeSpecified)
    {
        addMidiElement = true;
        const core::Percent volume{core::Decimal{midiData.volume}};
        reportAdjusted(diagnostics(), partLocation(myPartIndex), "volume", midiData.volume, volume.value().value());
        midiInstrument.setVolume(volume);
    }

    if (addMidiElement)
    {
        midiGroup.setMIDIInstrument(midiInstrument);
        outScorePart.addMIDIGroup(midiGroup);
    }

    return addMidiElement;
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
    // getScorePart reports an invalid instrument id; the instrument changes only repeat it.
    cursor.instrumentId = partInstrumentId(false).value();
    if (!inPartData.instrumentData.uniqueId.empty())
    {
        cursor.partInstrumentIds.push_back(inPartData.instrumentData.uniqueId);
    }
    for (const auto &additional : inPartData.additionalInstruments)
    {
        if (!additional.uniqueId.empty())
        {
            cursor.partInstrumentIds.push_back(additional.uniqueId);
        }
    }

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
