// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PartWriter.h"
#include "mx/api/DocumentManager.h"
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
#include "mx/impl/ScoreWriter.h"

#include <sstream>

namespace mx
{
namespace impl
{
PartWriter::PartWriter(const api::PartData &inPartData, int inPartIndex, int inTicksPerQuarter,
                       const ScoreWriter &inScoreWriter)
    : myPartData{inPartData}, myPartIndex{inPartIndex}, myTicksPerQuarter{inTicksPerQuarter}, myMutex{},
      myScoreWriter{inScoreWriter}
{
}

core::ScorePart PartWriter::getScorePart() const
{
    core::ScorePart scorePart{};
    scorePart.setID(core::Token{myPartData.uniqueId});

    core::PartName partName{};
    partName.setValue(myPartData.name);
    partName.setPrintObject(core::YesNo::no());
    scorePart.setPartName(partName);

    if (myPartData.abbreviation.size() > 0)
    {
        core::PartName abbrev{};
        abbrev.setValue(myPartData.abbreviation);
        abbrev.setPrintObject(core::YesNo::no());
        scorePart.setPartAbbreviation(abbrev);
    }

    if (myPartData.displayName.size() > 0)
    {
        core::NameDisplay nameDisplay{};
        core::FormattedText ft{};
        ft.setValue(myPartData.displayName);
        nameDisplay.addChoice(core::NameDisplayChoice::displayText(ft));
        scorePart.setPartNameDisplay(nameDisplay);
    }

    if (myPartData.displayAbbreviation.size() > 0)
    {
        core::NameDisplay nameDisplay{};
        core::FormattedText ft{};
        ft.setValue(myPartData.displayAbbreviation);
        nameDisplay.addChoice(core::NameDisplayChoice::displayText(ft));
        scorePart.setPartAbbreviationDisplay(nameDisplay);
    }

    core::ScoreInstrument scoreInstrument{};
    bool addScoreInstrument = false;
    scoreInstrument.setID(core::Token{myPartData.instrumentData.uniqueId});

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

    if (myPartData.instrumentData.midiData.device.size() > 0)
    {
        addMidiElement = true;
        midiDevice.setValue(myPartData.instrumentData.midiData.device);
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
        midiInstrument.setMIDIBank(core::MIDI16384{myPartData.instrumentData.midiData.bank});
    }

    if (myPartData.instrumentData.midiData.channel >= 0)
    {
        addMidiElement = true;
        midiInstrument.setMIDIChannel(core::MIDI16{myPartData.instrumentData.midiData.channel});
    }

    if (myPartData.instrumentData.midiData.program >= 0)
    {
        addMidiElement = true;
        midiInstrument.setMIDIProgram(core::MIDI128{myPartData.instrumentData.midiData.program});
    }

    if (myPartData.instrumentData.midiData.isElevationSpecified)
    {
        addMidiElement = true;
        midiInstrument.setElevation(core::RotationDegrees{core::Decimal{myPartData.instrumentData.midiData.elevation}});
    }

    if (myPartData.instrumentData.midiData.isPanSpecified)
    {
        addMidiElement = true;
        midiInstrument.setPan(core::RotationDegrees{core::Decimal{myPartData.instrumentData.midiData.pan}});
    }

    if (myPartData.instrumentData.midiData.isVolumeSpecified)
    {
        addMidiElement = true;
        midiInstrument.setVolume(core::Percent{core::Decimal{myPartData.instrumentData.midiData.volume}});
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
            ss << api::DocumentManager::getInstance().getUniqueId();
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
    MeasureCursor cursor{static_cast<int>(inPartData.measures.at(0).staves.size()), myTicksPerQuarter};
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
