// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MetronomeReader.h"
#include "mx/core/generated/BeatUnitGroup.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/MetronomeChoice.h"
#include "mx/core/generated/MetronomeChoiceGroup.h"
#include "mx/core/generated/MetronomeChoiceGroupChoice.h"
#include "mx/core/generated/MetronomeChoiceGroupChoiceGroup.h"
#include "mx/core/generated/PerMinute.h"
#include "mx/impl/Converter.h"
#include "mx/utility/StringToInt.h"

namespace mx
{
namespace impl
{
MetronomeReader::MetronomeReader(MetronomeReaderParameters &&params)
    : myMutex{}, myOutTempoData{}, myMetronome{params.metronome},
      myPreviousTempoData{std::move(params.previousTempoData)}, myCursor{std::move(params.cursor)},
      myBeatUnitPerOrNoteRelationNoteChoice{myMetronome.choice()}
{
}

api::TempoData MetronomeReader::getTempoData() const
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutTempoData = api::TempoData{};
    // the old core's beatUnitPer is the new core's group; the old core's
    // noteRelationNote (metronome-note based) is group2
    using FirstChoice = core::MetronomeChoice::Kind;
    const auto firstChoice = myBeatUnitPerOrNoteRelationNoteChoice.kind();

    switch (firstChoice)
    {
    case FirstChoice::group: {
        parseBeatUnitPer();
        break;
    }
    case FirstChoice::group2: {
        parseNoteRelationNote();
        break;
    }
    default:
        break;
    }

    api::TempoData temp{std::move(myOutTempoData)};
    myOutTempoData = api::TempoData{};
    return temp;
}

void MetronomeReader::parseBeatUnitPer() const
{
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    const auto choice = beatUnitPer.choice().kind();

    switch (choice)
    {
    case core::MetronomeChoiceGroupChoice::Kind::perMinute: {
        parseBeatsPerMinute();
        break;
    }
    case core::MetronomeChoiceGroupChoice::Kind::group: {
        parseMetronomeModulation();
        break;
    }
    default:
        break;
    }
}

void MetronomeReader::parseNoteRelationNote() const
{
    // The metronome-note form -- <metronome-note> ... <metronome-relation> ...
    // <metronome-note> -- has no representation in api::TempoData yet. Leave the
    // tempo 'unspecified' rather than crashing the whole api pipeline; the writer
    // skips unspecified tempos. Previously this threw and produced no output at
    // all (GETDATAFAIL). See issue #218.
}

void MetronomeReader::parseBeatsPerMinute() const
{
    myOutTempoData.tempoType = api::TempoType::beatsPerMinute;
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    const auto &grp = beatUnitPer.beatUnit();
    Converter converter;
    myOutTempoData.beatsPerMinute.durationName = converter.convert(grp.beatUnit());
    myOutTempoData.beatsPerMinute.dots = static_cast<int>(grp.beatUnitDot().size());
    const auto bpmStr = beatUnitPer.choice().asPerMinute().value();
    int bpm = -1;
    bool isNumeric = utility::stringToInt(bpmStr, bpm);
    if (!isNumeric)
    {
        myOutTempoData.tempoType = api::TempoType::tempoText;
        myOutTempoData.tempoText.tempoText = bpmStr;
    }
    else
    {
        myOutTempoData.beatsPerMinute.beatsPerMinute = bpm;
    }
}

void MetronomeReader::parseMetronomeModulation() const
{
    // Metric modulation: <beat-unit>..</beat-unit> = <beat-unit>..</beat-unit>.
    // The left beat-unit lives directly on the group; the right beat-unit is the
    // 'group' alternative of the metronome choice. Previously this was an empty
    // stub that left the tempo 'unspecified', which then crashed the writer
    // (CREATEFAIL). See issue #218.
    myOutTempoData.tempoType = api::TempoType::metricModulation;
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    Converter converter;

    const auto &leftBeatUnit = beatUnitPer.beatUnit();
    myOutTempoData.metricModulation.leftDurationName = converter.convert(leftBeatUnit.beatUnit());
    myOutTempoData.metricModulation.leftDots = static_cast<int>(leftBeatUnit.beatUnitDot().size());

    const auto &rightBeatUnit = beatUnitPer.choice().asGroup().beatUnit();
    myOutTempoData.metricModulation.rightDurationName = converter.convert(rightBeatUnit.beatUnit());
    myOutTempoData.metricModulation.rightDots = static_cast<int>(rightBeatUnit.beatUnitDot().size());
}
} // namespace impl
} // namespace mx
