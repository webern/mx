// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

// Build a one-measure, one-note score whose single barline the caller can configure. Mirrors the
// scaffolding in MeasureDataTest.cpp so each test can focus on the repeat fields.
static ScoreData makeScoreWithBarlineForRepeat()
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.voices[0].notes.emplace_back();
    measure.barlines.emplace_back();
    return score;
}

// A backward (end) repeat that carries every repeat attribute must survive the round trip.
TEST(backwardRepeatAllAttributesRoundTrip, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.barlineType = BarlineType::lightHeavy;
    barline.location = HorizontalAlignment::right;
    barline.repeat = true;
    barline.repeatDirection = RepeatDirection::backward;
    barline.repeatTimes = 3;
    barline.repeatAfterJump = Bool::yes;
    barline.repeatWinged = RepeatWinged::curved;

    const auto out = roundTrip(score);

    const auto &obarlines = out.parts.back().measures.back().barlines;
    REQUIRE(obarlines.size() == 1);
    const auto &ob = obarlines.front();
    CHECK(ob.repeat);
    CHECK(RepeatDirection::backward == ob.repeatDirection);
    CHECK_EQUAL(3, ob.repeatTimes);
    CHECK(Bool::yes == ob.repeatAfterJump);
    CHECK(RepeatWinged::curved == ob.repeatWinged);
}

T_END;

// A forward (start) repeat must remain forward through the round trip, distinct from a backward
// repeat -- the direction is no longer merely inferred from the barline position.
TEST(forwardRepeatDirectionRoundTrip, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.barlineType = BarlineType::heavyLight;
    barline.location = HorizontalAlignment::left;
    barline.repeat = true;
    barline.repeatDirection = RepeatDirection::forward;
    barline.repeatWinged = RepeatWinged::doubleStraight;

    const auto out = roundTrip(score);

    const auto &ob = out.parts.back().measures.back().barlines.front();
    CHECK(ob.repeat);
    CHECK(RepeatDirection::forward == ob.repeatDirection);
    CHECK(RepeatWinged::doubleStraight == ob.repeatWinged);
}

T_END;

// The reader must surface direction, times, after-jump, and winged from the MusicXML. An explicit
// winged="none" is RepeatWinged::none, distinct from an absent attribute (unspecified).
TEST(repeatAttributesReadFromXml, Repeat)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="id1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="id1">
    <measure number="1">
      <barline location="right">
        <repeat direction="backward" times="1" after-jump="yes" winged="none" />
      </barline>
    </measure>
  </part>
</score-partwise>)";

    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &barlines = score.parts.back().measures.back().barlines;
    REQUIRE(barlines.size() == 1);
    const auto &ob = barlines.front();
    CHECK(ob.repeat);
    CHECK(RepeatDirection::backward == ob.repeatDirection);
    CHECK_EQUAL(1, ob.repeatTimes);
    CHECK(Bool::yes == ob.repeatAfterJump);
    CHECK(RepeatWinged::none == ob.repeatWinged);
}

T_END;

// When after-jump and winged are absent from the source, they must read back as unspecified, not
// as a defaulted concrete value.
TEST(repeatOmittedAttributesAreUnspecified, Repeat)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="id1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="id1">
    <measure number="1">
      <barline location="right">
        <repeat direction="backward" />
      </barline>
    </measure>
  </part>
</score-partwise>)";

    const auto score = fromXml(xml);
    const auto &ob = score.parts.back().measures.back().barlines.front();
    CHECK(ob.repeat);
    CHECK(RepeatDirection::backward == ob.repeatDirection);
    CHECK_EQUAL(0, ob.repeatTimes);
    CHECK(Bool::unspecified == ob.repeatAfterJump);
    CHECK(RepeatWinged::unspecified == ob.repeatWinged);
}

T_END;

#endif
