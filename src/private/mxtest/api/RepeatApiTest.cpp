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

// Wrap an <ending> element in the smallest score that can carry one.
static std::string endingXmlForRepeat(const std::string &inEndingElement)
{
    return R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="id1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="id1">
    <measure number="1">
      <barline location="left">
        )" +
           inEndingElement +
           R"(
      </barline>
    </measure>
  </part>
</score-partwise>)";
}

// An ending played on several passes keeps every pass number, and its display label survives
// alongside them. This is the "1, 2, 3" ending labeled "1-3" case.
TEST(endingMultipleNumbersAndTextRoundTrip, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.location = HorizontalAlignment::left;
    EndingData ending;
    ending.type = EndingType::start;
    ending.numbers = {1, 2, 3};
    ending.text = "1-3";
    barline.ending = ending;

    const auto out = roundTrip(score);

    const auto &obarlines = out.parts.back().measures.back().barlines;
    REQUIRE(obarlines.size() == 1);
    const auto &ob = obarlines.front();
    REQUIRE(ob.ending.has_value());
    CHECK(EndingType::start == ob.ending->type);
    REQUIRE(ob.ending->numbers.size() == 3);
    CHECK_EQUAL(1, ob.ending->numbers.at(0));
    CHECK_EQUAL(2, ob.ending->numbers.at(1));
    CHECK_EQUAL(3, ob.ending->numbers.at(2));
    CHECK_EQUAL(std::string{"1-3"}, ob.ending->text);
}

T_END;

// The serialized MusicXML must spell the number list the way the spec does and carry the label as
// the element's text.
TEST(endingSerializesNumberListAndText, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.location = HorizontalAlignment::left;
    EndingData ending;
    ending.type = EndingType::start;
    ending.numbers = {1, 2, 3};
    ending.text = "1-3";
    barline.ending = ending;

    const auto xml = toXml(score);

    CHECK(xml.find(R"(number="1, 2, 3")") != std::string::npos);
    CHECK(xml.find(R"(type="start")") != std::string::npos);
    CHECK(xml.find(">1-3</ending>") != std::string::npos);
}

T_END;

// The reader must surface the whole number list, not just the first pass, along with the label.
TEST(endingMultipleNumbersAndTextReadFromXml, Repeat)
{
    const auto score = fromXml(endingXmlForRepeat(R"(<ending number="1, 2, 3" type="start">1-3</ending>)"));

    REQUIRE(score.parts.size() == 1);
    const auto &barlines = score.parts.back().measures.back().barlines;
    REQUIRE(barlines.size() == 1);
    const auto &ob = barlines.front();
    REQUIRE(ob.ending.has_value());
    CHECK(EndingType::start == ob.ending->type);
    REQUIRE(ob.ending->numbers.size() == 3);
    CHECK_EQUAL(1, ob.ending->numbers.at(0));
    CHECK_EQUAL(2, ob.ending->numbers.at(1));
    CHECK_EQUAL(3, ob.ending->numbers.at(2));
    CHECK_EQUAL(std::string{"1-3"}, ob.ending->text);
}

T_END;

// An ending with no label reads back with empty text, and writing one emits no text node -- the
// numbers are what gets displayed.
TEST(endingWithoutTextOmitsTextNode, Repeat)
{
    const auto score = fromXml(endingXmlForRepeat(R"(<ending number="1" type="stop"/>)"));
    const auto &ob = score.parts.back().measures.back().barlines.front();
    REQUIRE(ob.ending.has_value());
    CHECK(EndingType::stop == ob.ending->type);
    REQUIRE(ob.ending->numbers.size() == 1);
    CHECK_EQUAL(1, ob.ending->numbers.at(0));
    CHECK(ob.ending->text.empty());

    const auto xml = toXml(score);
    CHECK(xml.find("</ending>") == std::string::npos);
    CHECK(xml.find(R"(number="1")") != std::string::npos);
}

T_END;

// A blank ending -- a bracket whose passes are unknown -- is an ending with no numbers, and writes
// as number="".
TEST(endingBlankNumberRoundTrip, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.location = HorizontalAlignment::left;
    EndingData ending;
    ending.type = EndingType::start;
    barline.ending = ending;

    const auto xml = toXml(score);
    CHECK(xml.find(R"(number="")") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &ob = out.parts.back().measures.back().barlines.front();
    REQUIRE(ob.ending.has_value());
    CHECK(EndingType::start == ob.ending->type);
    CHECK(ob.ending->numbers.empty());
    CHECK(ob.ending->text.empty());
}

T_END;

// A barline that carries no ending must report none, and must not emit an <ending> element.
TEST(barlineWithoutEndingIsAbsent, Repeat)
{
    auto score = makeScoreWithBarlineForRepeat();
    auto &barline = score.parts.back().measures.back().barlines.back();
    barline.barlineType = BarlineType::lightHeavy;
    barline.location = HorizontalAlignment::right;

    const auto xml = toXml(score);
    CHECK(xml.find("<ending") == std::string::npos);

    const auto out = roundTrip(score);
    const auto &ob = out.parts.back().measures.back().barlines.front();
    CHECK(!ob.ending.has_value());
}

T_END;

// The core -> api read path against the shape of data/synthetic/ending.3.0.xml, whose ending
// carries a text body ("x") that differs from its number attribute ("1"). The attributes mx::api
// does not model are ignored; the number and the label must both arrive.
TEST(endingTextDiffersFromNumberReadFromXml, Repeat)
{
    const auto score = fromXml(
        endingXmlForRepeat(R"(<ending default-x="1" default-y="1" relative-x="1" relative-y="1" font-family="x" )"
                           R"(font-style="normal" font-size="1" font-weight="normal" color="#FF000000" number="1" )"
                           R"(type="start" print-object="yes" end-length="1" text-x="1" text-y="1">x</ending>)"));

    const auto &ob = score.parts.back().measures.back().barlines.front();
    REQUIRE(ob.ending.has_value());
    CHECK(EndingType::start == ob.ending->type);
    REQUIRE(ob.ending->numbers.size() == 1);
    CHECK_EQUAL(1, ob.ending->numbers.at(0));
    CHECK_EQUAL(std::string{"x"}, ob.ending->text);
}

T_END;

#endif
