// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"

#include <sstream>
#include <string>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// A minimal partwise document carrying exactly one <metronome> in one <direction>.
// `metronomeBody` is the inner markup of the <metronome> element.
std::string makeMetronomeDoc(const std::string &metronomeBody)
{
    return R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <direction>
        <direction-type>
          <metronome>)" +
           metronomeBody + R"(</metronome>
        </direction-type>
      </direction>
    </measure>
  </part>
</score-partwise>
)";
}

// Round-trips a score carrying one direction with the given tempo and returns the resulting tempo
// (a default-constructed TempoData if anything is missing).
TempoData roundTripTempo(const TempoData &in)
{
    ScoreData score;
    score.ticksPerQuarter = 100;
    score.parts.emplace_back();
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    score.parts.back().measures.back().staves.back().directions.emplace_back();
    score.parts.back().measures.back().staves.back().directions.back().directionTypes.emplace_back(DirectionChoice{in});

    const auto out = roundTrip(score);
    if (out.parts.empty() || out.parts.back().measures.empty() || out.parts.back().measures.back().staves.empty())
    {
        return TempoData{};
    }
    const auto &directions = out.parts.back().measures.back().staves.back().directions;
    if (directions.empty() || directions.back().directionTypes.empty() ||
        !directions.back().directionTypes.back().isTempo())
    {
        return TempoData{};
    }
    return directions.back().directionTypes.back().tempo();
}
} // namespace

TEST(roundTripBpm, MetronomeApi)
{
    BeatsPerMinute bpm;
    bpm.durationName = DurationName::dur16th;
    bpm.dots = 1;
    bpm.beatsPerMinute = "123";
    TempoData in;
    in.choice = TempoChoice{bpm};

    const auto out = roundTripTempo(in);

    CHECK(TempoChoice::Kind::beatsPerMinute == out.choice.kind());
    const auto outBpm = out.choice.beatsPerMinute();
    CHECK(DurationName::dur16th == outBpm.durationName);
    CHECK_EQUAL(1, outBpm.dots);
    CHECK(std::string{"123"} == outBpm.beatsPerMinute);
}

T_END;

// A non-numeric <per-minute> is legal -- per-minute is an xs:string. It is kept verbatim on
// BeatsPerMinute::beatsPerMinute, so a mark like "quarter = fast" round-trips faithfully instead
// of being dropped (which is what the old tempoText fallback did).
TEST(nonNumericPerMinuteRoundTrips, MetronomeApi)
{
    const std::string xml = makeMetronomeDoc(R"(
            <beat-unit>quarter</beat-unit>
            <per-minute>fast</per-minute>
          )");

    auto &mgr = DocumentManager::getInstance();
    std::istringstream iss{xml};
    const auto idResult = mgr.createFromStream(iss);
    CHECK(idResult.ok());
    if (!idResult.ok())
    {
        return;
    }
    const auto dataResult = mgr.getData(idResult.value());
    mgr.destroyDocument(idResult.value());
    CHECK(dataResult.ok());
    if (!dataResult.ok())
    {
        return;
    }

    const auto &score = dataResult.value();
    if (score.parts.empty() || score.parts.back().measures.empty() || score.parts.back().measures.back().staves.empty())
    {
        return;
    }
    const auto &directions = score.parts.back().measures.back().staves.back().directions;
    CHECK_EQUAL(1, static_cast<int>(directions.size()));
    if (directions.empty() || directions.back().directionTypes.empty() ||
        !directions.back().directionTypes.back().isTempo())
    {
        return;
    }
    const auto tempo = directions.back().directionTypes.back().tempo();
    CHECK(TempoChoice::Kind::beatsPerMinute == tempo.choice.kind());
    const auto bpm = tempo.choice.beatsPerMinute();
    CHECK(DurationName::quarter == bpm.durationName);
    CHECK(std::string{"fast"} == bpm.beatsPerMinute);

    // The mark must also write back without error.
    const auto id2Result = mgr.createFromScore(dataResult.value());
    CHECK(id2Result.ok());
    if (id2Result.ok())
    {
        mgr.destroyDocument(id2Result.value());
    }
}

T_END;

// Metric modulation (two beat-units) round-trips through the api.
TEST(roundTripMetricModulation, MetronomeApi)
{
    MetricModulation mm;
    mm.leftDurationName = DurationName::quarter;
    mm.leftDots = 1;
    mm.rightDurationName = DurationName::half;
    mm.rightDots = 0;
    TempoData in;
    in.choice = TempoChoice{mm};

    const auto out = roundTripTempo(in);

    CHECK(TempoChoice::Kind::metricModulation == out.choice.kind());
    const auto outMm = out.choice.metricModulation();
    CHECK(DurationName::quarter == outMm.leftDurationName);
    CHECK_EQUAL(1, outMm.leftDots);
    CHECK(DurationName::half == outMm.rightDurationName);
    CHECK_EQUAL(0, outMm.rightDots);
}

T_END;

// A beat-unit tied to a further beat-unit ("quarter + eighth = 120") round-trips via
// BeatsPerMinute::tiedBeatUnits.
TEST(roundTripBeatUnitTied, MetronomeApi)
{
    BeatsPerMinute bpm;
    bpm.durationName = DurationName::quarter;
    BeatUnit tied;
    tied.type = DurationName::eighth;
    bpm.tiedBeatUnits.push_back(tied);
    bpm.beatsPerMinute = "120";
    TempoData in;
    in.choice = TempoChoice{bpm};

    const auto out = roundTripTempo(in);

    CHECK(TempoChoice::Kind::beatsPerMinute == out.choice.kind());
    const auto outBpm = out.choice.beatsPerMinute();
    CHECK_EQUAL(1, static_cast<int>(outBpm.tiedBeatUnits.size()));
    if (!outBpm.tiedBeatUnits.empty())
    {
        CHECK(DurationName::eighth == outBpm.tiedBeatUnits.front().type);
    }
}

T_END;

// The metronome-note form -- note figures joined by a relation symbol -- round-trips as a
// NoteRelation, including beams, ties, tuplets, arrows, and the two-sided relation.
TEST(roundTripNoteRelation, MetronomeApi)
{
    NoteRelation nr;
    nr.arrows = true;

    MetronomeNoteData left;
    left.metronomeType = DurationName::eighth;
    MetronomeBeam beam;
    beam.value = Beam::begin;
    beam.number = 1;
    left.beams.push_back(beam);
    left.tie = MetronomeTieType::start;
    nr.notes.push_back(left);

    MetronomeRelation rel;
    rel.symbol = "equals";
    MetronomeNoteData right;
    right.metronomeType = DurationName::quarter;
    MetronomeTuplet tuplet;
    tuplet.actualNotes = 3;
    tuplet.normalNotes = 2;
    tuplet.normalType = DurationName::quarter;
    tuplet.type = MetronomeTupletType::start;
    tuplet.bracket = Bool::yes;
    tuplet.showNumber = MetronomeShowNumber::actual;
    right.tuplet = tuplet;
    rel.notes.push_back(right);
    nr.relation = rel;

    TempoData in;
    in.choice = TempoChoice{nr};

    const auto out = roundTripTempo(in);

    CHECK(TempoChoice::Kind::noteRelation == out.choice.kind());
    const auto outNr = out.choice.noteRelation();
    CHECK(outNr.arrows);
    CHECK_EQUAL(1, static_cast<int>(outNr.notes.size()));
    CHECK(outNr.relation.has_value());
    // Strong check: the whole note-relation body must survive unchanged.
    CHECK(nr == outNr);
}

T_END;

TEST(roundTripParentheses, MetronomeApi)
{
    // <metronome parentheses="yes"> must round-trip via TempoData::isParenthetical.
    BeatsPerMinute bpm;
    bpm.durationName = DurationName::quarter;
    bpm.beatsPerMinute = "100";
    TempoData in;
    in.choice = TempoChoice{bpm};
    in.isParenthetical = Bool::yes;

    const auto out = roundTripTempo(in);

    CHECK(out.isParenthetical == Bool::yes);
}

T_END;

TEST(roundTripMetronomeAttributes, MetronomeApi)
{
    // The <metronome> print-style-align, justify, print-object, and id attributes must survive a
    // round trip via TempoData's positionData/fontData/justify/printObject/id.
    BeatsPerMinute bpm;
    bpm.durationName = DurationName::quarter;
    bpm.beatsPerMinute = "120";
    TempoData in;
    in.choice = TempoChoice{bpm};
    in.positionData.isDefaultYSpecified = true;
    in.positionData.defaultY = 12.0;
    in.positionData.horizontalAlignment = HorizontalAlignment::left;
    in.fontData.style = FontStyle::italic;
    in.justify = HorizontalAlignment::center;
    in.printObject = Bool::no;
    in.id = std::string{"tempo1"};

    const auto out = roundTripTempo(in);

    CHECK(out.positionData.isDefaultYSpecified);
    CHECK(HorizontalAlignment::left == out.positionData.horizontalAlignment);
    CHECK(FontStyle::italic == out.fontData.style);
    CHECK(HorizontalAlignment::center == out.justify);
    CHECK(Bool::no == out.printObject);
    CHECK(out.id.has_value());
    if (out.id.has_value())
    {
        CHECK(std::string{"tempo1"} == *out.id);
    }
}

T_END;

#endif
