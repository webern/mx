// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxpugi/mxpugi.hpp"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <string>
#include <vector>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// Builds a single-part, single-measure score: one quarter note on beat 1 with a figured-bass
// direction attached at the same tick. The caller fills the returned direction's figuredBasses.
ScoreData makeScoreWithFiguredBass(FiguredBassData figuredBass)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.name = "P1";
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();

    NoteData note{};
    note.pitchData.step = Step::c;
    note.pitchData.octave = 3;
    note.durationData.durationTimeTicks = 4;
    note.durationData.durationName = DurationName::quarter;
    note.tickTimePosition = 0;
    staff.voices[0].notes.push_back(note);

    DirectionData direction{};
    direction.tickTimePosition = 0;
    direction.isStaffValueSpecified = true;
    direction.placement = Placement::below;
    direction.figuredBasses.emplace_back(std::move(figuredBass));
    staff.directions.emplace_back(std::move(direction));

    return score;
}

const FiguredBassData *firstFiguredBass(const ScoreData &score)
{
    const auto &staff = score.parts.front().measures.front().staves.front();
    for (const auto &dir : staff.directions)
    {
        if (!dir.figuredBasses.empty())
        {
            return &dir.figuredBasses.front();
        }
    }
    return nullptr;
}
} // namespace

TEST(figuredBassFiguresRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "6";
    FigureData f1{};
    f1.prefix = "flat";
    f1.figureNumber = "5";
    f1.suffix = "slash";
    fb.figures.push_back(f0);
    fb.figures.push_back(f1);

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    REQUIRE(outFb->figures.size() == 2);
    CHECK_EQUAL("6", outFb->figures.at(0).figureNumber);
    CHECK_EQUAL("", outFb->figures.at(0).prefix);
    CHECK_EQUAL("flat", outFb->figures.at(1).prefix);
    CHECK_EQUAL("5", outFb->figures.at(1).figureNumber);
    CHECK_EQUAL("slash", outFb->figures.at(1).suffix);
}

T_END;

TEST(figuredBassParenthesesAndDurationRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "7";
    fb.figures.push_back(f0);
    fb.parentheses = Bool::yes;
    fb.durationTimeTicks = 4;

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    CHECK(Bool::yes == outFb->parentheses);
    CHECK_EQUAL(4, outFb->durationTimeTicks);
    REQUIRE(outFb->figures.size() == 1);
    CHECK_EQUAL("7", outFb->figures.at(0).figureNumber);
}

T_END;

TEST(figuredBassEmptyFiguresOmitted, FiguredBassApi)
{
    // A FiguredBassData carrying no figures must not produce a <figured-bass> on output. The core
    // figure list is never-empty (OneOrMore), so emitting one anyway would fabricate a stray empty
    // <figure/> that reads back as a one-figure figured-bass -- a round-trip that invents content.
    FiguredBassData fb{};

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    CHECK(firstFiguredBass(out) == nullptr);
}

T_END;

TEST(figuredBassExtendRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "6";
    f0.extend = FigureExtend{};
    FigureData f1{};
    f1.figureNumber = "6";
    f1.extend = FigureExtend{};
    f1.extend->type = FigureExtendType::start;
    FigureData f2{};
    f2.figureNumber = "5";
    fb.figures.push_back(f0);
    fb.figures.push_back(f1);
    fb.figures.push_back(f2);

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    REQUIRE(outFb->figures.size() == 3);
    REQUIRE(outFb->figures.at(0).extend.has_value());
    CHECK(FigureExtendType::unspecified == outFb->figures.at(0).extend->type);
    REQUIRE(outFb->figures.at(1).extend.has_value());
    CHECK(FigureExtendType::start == outFb->figures.at(1).extend->type);
    CHECK(!outFb->figures.at(2).extend.has_value());
    CHECK_EQUAL("6", outFb->figures.at(0).figureNumber);
    CHECK_EQUAL("5", outFb->figures.at(2).figureNumber);
}

T_END;

TEST(figuredBassExtendOnlyFigure, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.extend = FigureExtend{};
    fb.figures.push_back(f0);

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    REQUIRE(outFb->figures.size() == 1);
    CHECK_EQUAL("", outFb->figures.at(0).prefix);
    CHECK_EQUAL("", outFb->figures.at(0).figureNumber);
    CHECK_EQUAL("", outFb->figures.at(0).suffix);
    REQUIRE(outFb->figures.at(0).extend.has_value());
    CHECK(FigureExtendType::unspecified == outFb->figures.at(0).extend->type);
}

T_END;

TEST(figuredBassExtendAllTypes, FiguredBassApi)
{
    const FigureExtendType types[] = {FigureExtendType::start, FigureExtendType::continue_, FigureExtendType::stop,
                                      FigureExtendType::unspecified};

    for (const auto type : types)
    {
        FiguredBassData fb{};
        FigureData f0{};
        f0.figureNumber = "6";
        f0.extend = FigureExtend{};
        f0.extend->type = type;
        fb.figures.push_back(f0);

        const auto out = mxtest::roundTrip(makeScoreWithFiguredBass(fb));

        const auto *outFb = firstFiguredBass(out);
        REQUIRE(outFb != nullptr);
        REQUIRE(outFb->figures.size() == 1);
        REQUIRE(outFb->figures.at(0).extend.has_value());
        CHECK(type == outFb->figures.at(0).extend->type);
    }
}

T_END;

TEST(figuredBassExtendWritesXml, FiguredBassApi)
{
    const FigureExtendType types[] = {FigureExtendType::unspecified, FigureExtendType::start,
                                      FigureExtendType::continue_, FigureExtendType::stop};

    FiguredBassData fb{};
    for (const auto type : types)
    {
        FigureData figure{};
        figure.figureNumber = "6";
        figure.extend = FigureExtend{};
        figure.extend->type = type;
        fb.figures.push_back(figure);
    }
    FigureData none{};
    none.figureNumber = "5";
    fb.figures.push_back(none);

    const auto xml = mxtest::toXml(makeScoreWithFiguredBass(fb));

    mxpugi::xml_document doc;
    REQUIRE(doc.load_string(xml.c_str()));
    const auto figuredBass = doc.select_node("//figured-bass").node();
    REQUIRE(!figuredBass.empty());

    std::vector<mxpugi::xml_node> figures;
    for (const auto figure : figuredBass.children("figure"))
    {
        figures.push_back(figure);
    }
    REQUIRE(figures.size() == 5);

    const auto bareExtend = figures.at(0).child("extend");
    REQUIRE(!bareExtend.empty());
    CHECK(bareExtend.attribute("type").empty());

    const auto startExtend = figures.at(1).child("extend");
    REQUIRE(!startExtend.empty());
    CHECK_EQUAL("start", std::string{startExtend.attribute("type").value()});

    const auto continueExtend = figures.at(2).child("extend");
    REQUIRE(!continueExtend.empty());
    CHECK_EQUAL("continue", std::string{continueExtend.attribute("type").value()});

    const auto stopExtend = figures.at(3).child("extend");
    REQUIRE(!stopExtend.empty());
    CHECK_EQUAL("stop", std::string{stopExtend.attribute("type").value()});

    CHECK(figures.at(4).child("extend").empty());
}

T_END;

TEST(figuredBassExtendEquality, FiguredBassApi)
{
    FigureData with{};
    with.figureNumber = "6";
    with.extend = FigureExtend{};
    FigureData without{};
    without.figureNumber = "6";

    CHECK(with != without);

    FigureData other = with;
    CHECK(with == other);
    other.extend->type = FigureExtendType::stop;
    CHECK(with != other);
}

T_END;

// Wrap a <figured-bass> start tag, with whatever attributes the caller gives it, in the smallest
// score that can carry one: a single figure followed by the bass note it belongs to.
static std::string figuredBassXmlForPosition(const std::string &inAttributes)
{
    return R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>P1</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <figured-bass)" +
           inAttributes + R"(>
        <figure>
          <figure-number>6</figure-number>
        </figure>
      </figured-bass>
      <note>
        <pitch>
          <step>C</step>
          <octave>3</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>)";
}

static FiguredBassData makeFiguredBassWithPositionAndPrint()
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "6";
    fb.figures.push_back(f0);
    fb.positionData.isDefaultXSpecified = true;
    fb.positionData.defaultX = 12.0;
    fb.positionData.isDefaultYSpecified = true;
    fb.positionData.defaultY = -80.0;
    fb.positionData.isRelativeXSpecified = true;
    fb.positionData.relativeX = 2.0;
    fb.positionData.isRelativeYSpecified = true;
    fb.positionData.relativeY = -3.0;
    fb.positionData.horizontalAlignment = HorizontalAlignment::center;
    fb.positionData.verticalAlignment = VerticalAlignment::top;
    fb.positionData.placement = Placement::below;
    fb.printData.printObject = Bool::no;
    fb.printData.fontData.fontFamily = {"Times"};
    fb.printData.fontData.style = FontStyle::italic;
    fb.printData.fontData.weight = FontWeight::bold;
    fb.printData.fontData.sizeType = FontSizeType::point;
    fb.printData.fontData.sizePoint = 11.0;
    fb.printData.isColorSpecified = true;
    fb.printData.color.red = 255;
    fb.printData.color.green = 0;
    fb.printData.color.blue = 0;
    return fb;
}

// Figures moved, aligned, restyled and hidden by hand come back exactly as they were set.
TEST(figuredBassPositionAndPrintRoundTrip, FiguredBassApi)
{
    const auto fb = makeFiguredBassWithPositionAndPrint();
    const auto out = mxtest::roundTrip(makeScoreWithFiguredBass(fb));

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    CHECK(fb.positionData == outFb->positionData);
    CHECK(fb.printData == outFb->printData);
    CHECK(fb == *outFb);
}

T_END;

// Figures placed above the staff come back above it.
TEST(figuredBassPlacementAboveRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "6";
    fb.figures.push_back(f0);
    fb.positionData.placement = Placement::above;

    const auto out = mxtest::roundTrip(makeScoreWithFiguredBass(fb));

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    CHECK(Placement::above == outFb->positionData.placement);
}

T_END;

// Every position, font, color and print-object attribute on <figured-bass> reaches FiguredBassData.
TEST(figuredBassPositionAndPrintReadFromXml, FiguredBassApi)
{
    const auto score = fromXml(figuredBassXmlForPosition(
        R"( default-x="12" default-y="-80" relative-x="2" relative-y="-3" font-family="Times" font-style="italic" font-size="11" font-weight="bold" color="#FF0000" halign="center" valign="top" placement="below" print-object="no")"));

    const auto *fb = firstFiguredBass(score);
    REQUIRE(fb != nullptr);
    const auto &pos = fb->positionData;
    CHECK(pos.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(12.0, pos.defaultX, 0.0001);
    CHECK(pos.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(-80.0, pos.defaultY, 0.0001);
    CHECK(pos.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(2.0, pos.relativeX, 0.0001);
    CHECK(pos.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(-3.0, pos.relativeY, 0.0001);
    CHECK(HorizontalAlignment::center == pos.horizontalAlignment);
    CHECK(VerticalAlignment::top == pos.verticalAlignment);
    CHECK(Placement::below == pos.placement);

    const auto &print = fb->printData;
    CHECK(Bool::no == print.printObject);
    REQUIRE(print.fontData.fontFamily.size() == 1);
    CHECK_EQUAL("Times", print.fontData.fontFamily.front());
    CHECK(FontStyle::italic == print.fontData.style);
    CHECK(FontWeight::bold == print.fontData.weight);
    CHECK(FontSizeType::point == print.fontData.sizeType);
    CHECK_DOUBLES_EQUAL(11.0, print.fontData.sizePoint, 0.0001);
    CHECK(print.isColorSpecified);
    CHECK_EQUAL(255, static_cast<int>(print.color.red));
    CHECK_EQUAL(0, static_cast<int>(print.color.green));
    CHECK_EQUAL(0, static_cast<int>(print.color.blue));
}

T_END;

// The position, font, color and print-object are written as attributes of <figured-bass>.
TEST(figuredBassSerializesPositionAndPrint, FiguredBassApi)
{
    const auto xml = mxtest::toXml(makeScoreWithFiguredBass(makeFiguredBassWithPositionAndPrint()));

    mxpugi::xml_document doc;
    REQUIRE(doc.load_string(xml.c_str()));
    const auto figuredBass = doc.select_node("//figured-bass").node();
    REQUIRE(!figuredBass.empty());
    CHECK_EQUAL("12", std::string{figuredBass.attribute("default-x").value()});
    CHECK_EQUAL("-80", std::string{figuredBass.attribute("default-y").value()});
    CHECK_EQUAL("2", std::string{figuredBass.attribute("relative-x").value()});
    CHECK_EQUAL("-3", std::string{figuredBass.attribute("relative-y").value()});
    CHECK_EQUAL("center", std::string{figuredBass.attribute("halign").value()});
    CHECK_EQUAL("top", std::string{figuredBass.attribute("valign").value()});
    CHECK_EQUAL("below", std::string{figuredBass.attribute("placement").value()});
    CHECK_EQUAL("no", std::string{figuredBass.attribute("print-object").value()});
    CHECK_EQUAL("Times", std::string{figuredBass.attribute("font-family").value()});
    CHECK_EQUAL("italic", std::string{figuredBass.attribute("font-style").value()});
    CHECK_EQUAL("bold", std::string{figuredBass.attribute("font-weight").value()});
    CHECK_EQUAL("11", std::string{figuredBass.attribute("font-size").value()});
    CHECK(!figuredBass.attribute("color").empty());
}

T_END;

// Figures left to the program's layout read back with default position and print data, and do not
// write any of those attributes.
TEST(figuredBassWithoutPositionAndPrintOmitsAttributes, FiguredBassApi)
{
    const auto score = fromXml(figuredBassXmlForPosition(""));
    const auto *fb = firstFiguredBass(score);
    REQUIRE(fb != nullptr);
    CHECK(PositionData{} == fb->positionData);
    CHECK(PrintData{} == fb->printData);

    const auto xml = mxtest::toXml(score);
    mxpugi::xml_document doc;
    REQUIRE(doc.load_string(xml.c_str()));
    const auto figuredBass = doc.select_node("//figured-bass").node();
    REQUIRE(!figuredBass.empty());
    CHECK(figuredBass.first_attribute().empty());
}

T_END;

// Two figured-bass groups that differ only in where or how they are drawn are not equal.
TEST(figuredBassEqualityCoversPositionAndPrint, FiguredBassApi)
{
    FiguredBassData base{};
    FigureData f0{};
    f0.figureNumber = "6";
    base.figures.push_back(f0);

    auto moved = base;
    moved.positionData.placement = Placement::above;
    CHECK(base != moved);

    auto hidden = base;
    hidden.printData.printObject = Bool::no;
    CHECK(base != hidden);
}

T_END;

#endif
