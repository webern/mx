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

TEST(roundTripBpm, MetronomeApi)
{
    const auto expectedDurationName = DurationName::dur16th;
    const int expectedDots = 1;
    const std::string expectedBeatsPerMinute = "123";
    const int expectedTickTimePosition = 77;

    ScoreData expectedScoreData;
    expectedScoreData.ticksPerQuarter = 100;
    expectedScoreData.parts.emplace_back();
    auto &expectedPart = expectedScoreData.parts.back();
    expectedPart.measures.emplace_back();
    auto &expectedMeasure = expectedPart.measures.back();
    expectedMeasure.staves.emplace_back();
    auto &expectedStaff = expectedMeasure.staves.back();
    expectedStaff.directions.emplace_back();
    auto &expectedDirection = expectedStaff.directions.back();
    expectedDirection.tempos.emplace_back();
    expectedDirection.tickTimePosition = expectedTickTimePosition;
    auto &expectedTempo = expectedDirection.tempos.back();
    expectedTempo.tempoType = TempoType::beatsPerMinute;
    auto &expectedBpm = expectedTempo.beatsPerMinute;
    expectedBpm.durationName = expectedDurationName;
    expectedBpm.dots = expectedDots;
    expectedBpm.beatsPerMinute = expectedBeatsPerMinute;

    auto actualScoreData = roundTrip(expectedScoreData);

    auto &actualPart = actualScoreData.parts.back();
    auto &actualMeasure = actualPart.measures.back();
    auto &actualStaff = actualMeasure.staves.back();
    auto &actualDirection = actualStaff.directions.back();
    auto &actualTempo = actualDirection.tempos.back();
    actualTempo.tempoType = TempoType::beatsPerMinute;
    auto &actualBpm = actualTempo.beatsPerMinute;

    CHECK(expectedBeatsPerMinute == actualBpm.beatsPerMinute);
    CHECK_EQUAL(expectedDots, actualBpm.dots);
    CHECK(expectedDurationName == actualBpm.durationName);
    CHECK_EQUAL(expectedTickTimePosition, actualDirection.tickTimePosition);
}

// --- issue #218: metronome/tempo marks must not crash the api pipeline -------

namespace
{
// A minimal partwise document carrying exactly one <metronome> in one
// <direction>. `metronomeBody` is the inner markup of the <metronome> element.
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
} // namespace

// The metronome-note form has no api::TempoData representation. It used to throw
// "wtf is this" in the reader, which DocumentManager turned into a getData()
// failure -- the whole file produced no output (GETDATAFAIL). After the fix the
// tempo is dropped but reading succeeds.
TEST(metronomeNoteFormReadDoesNotFail, MetronomeApi)
{
    const std::string xml = makeMetronomeDoc(R"(
            <metronome-note>
              <metronome-type>quarter</metronome-type>
            </metronome-note>
            <metronome-relation>equals</metronome-relation>
            <metronome-note>
              <metronome-type>eighth</metronome-type>
            </metronome-note>
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
    // Before the fix this was an error (GETDATAFAIL); now reading must succeed.
    CHECK(dataResult.ok());
}

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
    CHECK_EQUAL(1, static_cast<int>(score.parts.size()));
    if (score.parts.empty() || score.parts.back().measures.empty() || score.parts.back().measures.back().staves.empty())
    {
        return;
    }
    const auto &directions = score.parts.back().measures.back().staves.back().directions;
    CHECK_EQUAL(1, static_cast<int>(directions.size()));
    if (directions.empty() || directions.back().tempos.empty())
    {
        return;
    }
    const auto &tempo = directions.back().tempos.back();
    CHECK(TempoType::beatsPerMinute == tempo.tempoType);
    CHECK(DurationName::quarter == tempo.beatsPerMinute.durationName);
    CHECK(std::string{"fast"} == tempo.beatsPerMinute.beatsPerMinute);

    // The mark must also write back without error.
    const auto id2Result = mgr.createFromScore(dataResult.value());
    CHECK(id2Result.ok());
    if (id2Result.ok())
    {
        mgr.destroyDocument(id2Result.value());
    }
}

// Metric modulation (two beat-units) used to be an empty read stub plus a writer
// throw. It now round-trips through the api.
TEST(roundTripMetricModulation, MetronomeApi)
{
    ScoreData score;
    score.ticksPerQuarter = 100;
    score.parts.emplace_back();
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    score.parts.back().measures.back().staves.back().directions.emplace_back();
    auto &direction = score.parts.back().measures.back().staves.back().directions.back();
    direction.tempos.emplace_back();
    auto &tempo = direction.tempos.back();
    tempo.tempoType = TempoType::metricModulation;
    tempo.metricModulation.leftDurationName = DurationName::quarter;
    tempo.metricModulation.leftDots = 1;
    tempo.metricModulation.rightDurationName = DurationName::half;
    tempo.metricModulation.rightDots = 0;

    const auto out = roundTrip(score);

    CHECK_EQUAL(1, static_cast<int>(out.parts.size()));
    if (out.parts.empty())
    {
        return;
    }
    const auto &measures = out.parts.back().measures;
    CHECK_EQUAL(1, static_cast<int>(measures.size()));
    if (measures.empty() || measures.back().staves.empty())
    {
        return;
    }
    const auto &directions = measures.back().staves.back().directions;
    CHECK_EQUAL(1, static_cast<int>(directions.size()));
    if (directions.empty() || directions.back().tempos.empty())
    {
        return;
    }
    const auto &outTempo = directions.back().tempos.back();
    CHECK(TempoType::metricModulation == outTempo.tempoType);
    CHECK(DurationName::quarter == outTempo.metricModulation.leftDurationName);
    CHECK_EQUAL(1, outTempo.metricModulation.leftDots);
    CHECK(DurationName::half == outTempo.metricModulation.rightDurationName);
    CHECK_EQUAL(0, outTempo.metricModulation.rightDots);
}

T_END

TEST(roundTripParentheses, MetronomeApi)
{
    // <metronome parentheses="yes"> must round-trip via TempoData::isParenthetical.
    ScoreData score;
    score.ticksPerQuarter = 100;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.directions.emplace_back();
    auto &direction = staff.directions.back();
    direction.tempos.emplace_back();
    auto &tempo = direction.tempos.back();
    tempo.tempoType = TempoType::beatsPerMinute;
    tempo.isParenthetical = Bool::yes;
    tempo.beatsPerMinute.durationName = DurationName::quarter;
    tempo.beatsPerMinute.beatsPerMinute = "100";

    auto actualScoreData = roundTrip(score);

    const auto &actualTempo =
        actualScoreData.parts.back().measures.back().staves.back().directions.back().tempos.back();
    CHECK(actualTempo.isParenthetical == Bool::yes);
}

T_END

TEST(roundTripMetronomeAttributes, MetronomeApi)
{
    // The <metronome> print-style-align, justify, print-object, color, and id attributes must
    // survive a round trip via TempoData's positionData/fontData/justify/printObject/color/id.
    ScoreData score;
    score.ticksPerQuarter = 100;
    score.parts.emplace_back();
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    score.parts.back().measures.back().staves.back().directions.emplace_back();
    auto &tempo = score.parts.back().measures.back().staves.back().directions.back().tempos.emplace_back();
    tempo.tempoType = TempoType::beatsPerMinute;
    tempo.beatsPerMinute.durationName = DurationName::quarter;
    tempo.beatsPerMinute.beatsPerMinute = "120";
    tempo.positionData.isDefaultYSpecified = true;
    tempo.positionData.defaultY = 12.0;
    tempo.positionData.horizontalAlignment = HorizontalAlignment::left;
    tempo.fontData.style = FontStyle::italic;
    tempo.justify = HorizontalAlignment::center;
    tempo.printObject = Bool::no;
    tempo.id = std::string{"tempo1"};

    const auto out = roundTrip(score);

    const auto &directions = out.parts.back().measures.back().staves.back().directions;
    CHECK_EQUAL(1, static_cast<int>(directions.size()));
    if (directions.empty() || directions.back().tempos.empty())
    {
        return;
    }
    const auto &outTempo = directions.back().tempos.back();
    CHECK(outTempo.positionData.isDefaultYSpecified);
    CHECK(HorizontalAlignment::left == outTempo.positionData.horizontalAlignment);
    CHECK(FontStyle::italic == outTempo.fontData.style);
    CHECK(HorizontalAlignment::center == outTempo.justify);
    CHECK(Bool::no == outTempo.printObject);
    CHECK(outTempo.id.has_value());
    if (outTempo.id.has_value())
    {
        CHECK(std::string{"tempo1"} == *outTempo.id);
    }
}

T_END

#endif
