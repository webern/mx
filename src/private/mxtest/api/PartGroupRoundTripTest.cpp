// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace mx::api;

namespace
{
PartData makeSimplePart(const std::string &id, const std::string &name)
{
    VoiceData voice;
    NoteData n;
    n.tickTimePosition = 0;
    n.pitchData.step = Step::c;
    n.pitchData.octave = 5;
    n.durationData.durationName = DurationName::quarter;
    n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
    voice.notes.push_back(n);
    StaffData staff{};
    staff.voices.emplace(0, voice);
    MeasureData m;
    m.staves.push_back(staff);
    PartData pd;
    pd.uniqueId = id;
    pd.name = name;
    pd.measures.push_back(m);
    return pd;
}

ScoreData makeTwoPartScore()
{
    ScoreData s;
    s.parts.push_back(makeSimplePart("P1", "Violin I"));
    s.parts.push_back(makeSimplePart("P2", "Violin II"));
    return s;
}
} // namespace

// Regression test for issue #219. A well-formed part-group spanning two parts
// must round-trip through mx::api with full fidelity. Before the fix the writer
// dropped group-abbreviation and both display names, and fabricated a constant
// <group-barline>yes</group-barline> regardless of input -- so the marked
// CHECKs below were the red lines that the fix turns green.
TEST(partGroupRoundTrip, fullFidelity)
{
    auto in = makeTwoPartScore();
    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.number = 1;
    grp.name = "Violins";
    grp.displayName = "Violins (display)";
    grp.abbreviation = "Vlns";
    grp.displayAbbreviation = "Vlns (display)";
    grp.bracketType = BracketType::bracket;
    grp.groupBarline = GroupBarline::no;
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.partGroups.size() == 1);
    const auto &got = out.partGroups.at(0);
    CHECK_EQUAL(0, got.firstPartIndex);
    CHECK_EQUAL(1, got.lastPartIndex);
    CHECK_EQUAL(1, got.number);
    CHECK_EQUAL("Violins", got.name);
    CHECK_EQUAL("Violins (display)", got.displayName);      // red before #219 fix (dropped)
    CHECK_EQUAL("Vlns", got.abbreviation);                  // red before #219 fix (dropped)
    CHECK_EQUAL("Vlns (display)", got.displayAbbreviation); // red before #219 fix (dropped)
    CHECK(BracketType::bracket == got.bracketType);
    CHECK(GroupBarline::no == got.groupBarline); // red before #219 fix (fabricated yes)
}

// A part-group that carries no <group-barline> must NOT gain a fabricated one on
// write. (Previously every group was emitted with <group-barline>yes</group-barline>.)
TEST(partGroupRoundTrip, noFabricatedBarline)
{
    auto in = makeTwoPartScore();
    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.name = "Group";
    grp.bracketType = BracketType::brace;
    // groupBarline left unspecified on purpose.
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);

    REQUIRE(out.partGroups.size() == 1);
    CHECK(GroupBarline::unspecified == out.partGroups.at(0).groupBarline);
}

// Two groups that differ only in their number are different groups.
TEST(partGroupRoundTrip, numberEquality)
{
    PartGroupData a;
    a.number = 1;
    PartGroupData b;
    b.number = 2;
    CHECK(a != b);
}

namespace
{
PartGroupData makeBracketGroup()
{
    PartGroupData grp;
    grp.firstPartIndex = 0;
    grp.lastPartIndex = 1;
    grp.number = 1;
    grp.bracketType = BracketType::bracket;
    return grp;
}
} // namespace

// A bracket pulled 10 tenths out into the left margin, nudged down, and drawn in red.
TEST(partGroupRoundTrip, bracketPositionAndColor)
{
    auto in = makeTwoPartScore();
    auto grp = makeBracketGroup();
    grp.bracketPositionData.isDefaultXSpecified = true;
    grp.bracketPositionData.defaultX = -10.0;
    grp.bracketPositionData.isDefaultYSpecified = true;
    grp.bracketPositionData.defaultY = 5.0;
    grp.bracketPositionData.isRelativeXSpecified = true;
    grp.bracketPositionData.relativeX = 1.5;
    grp.bracketPositionData.isRelativeYSpecified = true;
    grp.bracketPositionData.relativeY = -2.0;
    ColorData red;
    red.red = 255;
    red.green = 0;
    red.blue = 0;
    grp.bracketColor = red;
    in.partGroups.push_back(grp);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find(R"(default-x="-10")") != std::string::npos);
    CHECK(xml.find(R"(default-y="5")") != std::string::npos);
    CHECK(xml.find(R"(relative-x="1.5")") != std::string::npos);
    CHECK(xml.find(R"(relative-y="-2")") != std::string::npos);
    CHECK(xml.find(R"(color="#FF0000")") != std::string::npos);

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.partGroups.size() == 1);
    const auto &got = out.partGroups.at(0);
    CHECK(BracketType::bracket == got.bracketType);
    CHECK(grp.bracketPositionData == got.bracketPositionData);
    REQUIRE(got.bracketColor.has_value());
    CHECK(red == *got.bracketColor);
    CHECK(grp == got);
}

TEST(partGroupRoundTrip, bracketColorWithAlpha)
{
    auto in = makeTwoPartScore();
    auto grp = makeBracketGroup();
    ColorData color;
    color.red = 0;
    color.green = 0;
    color.blue = 255;
    color.isAlphaSpecified = true;
    color.alpha = 128;
    grp.bracketColor = color;
    in.partGroups.push_back(grp);

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.partGroups.size() == 1);
    REQUIRE(out.partGroups.at(0).bracketColor.has_value());
    CHECK(color == *out.partGroups.at(0).bracketColor);
}

TEST(partGroupRoundTrip, bracketPositionAndColorFromXml)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
    <part-group type="start" number="1">
      <group-symbol default-x="-10" default-y="3" relative-x="2" relative-y="-4" color="#00FF00">brace</group-symbol>
    </part-group>
    <score-part id="P1">
      <part-name>One</part-name>
    </score-part>
    <score-part id="P2">
      <part-name>Two</part-name>
    </score-part>
    <part-group type="stop" number="1"/>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
    </measure>
  </part>
  <part id="P2">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);
    REQUIRE(score.partGroups.size() == 1);
    const auto &grp = score.partGroups.at(0);
    CHECK(BracketType::brace == grp.bracketType);
    CHECK(grp.bracketPositionData.isDefaultXSpecified);
    CHECK_DOUBLES_EQUAL(-10.0, grp.bracketPositionData.defaultX, 0.0001);
    CHECK(grp.bracketPositionData.isDefaultYSpecified);
    CHECK_DOUBLES_EQUAL(3.0, grp.bracketPositionData.defaultY, 0.0001);
    CHECK(grp.bracketPositionData.isRelativeXSpecified);
    CHECK_DOUBLES_EQUAL(2.0, grp.bracketPositionData.relativeX, 0.0001);
    CHECK(grp.bracketPositionData.isRelativeYSpecified);
    CHECK_DOUBLES_EQUAL(-4.0, grp.bracketPositionData.relativeY, 0.0001);
    REQUIRE(grp.bracketColor.has_value());
    CHECK_EQUAL(0, static_cast<int>(grp.bracketColor->red));
    CHECK_EQUAL(255, static_cast<int>(grp.bracketColor->green));
    CHECK_EQUAL(0, static_cast<int>(grp.bracketColor->blue));
    CHECK(!grp.bracketColor->isAlphaSpecified);
}

TEST(partGroupRoundTrip, bracketPositionAndColorAbsentIsNotWritten)
{
    auto in = makeTwoPartScore();
    in.partGroups.push_back(makeBracketGroup());

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("<group-symbol>bracket</group-symbol>") != std::string::npos);

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.partGroups.size() == 1);
    const auto &got = out.partGroups.at(0);
    CHECK(PositionData{} == got.bracketPositionData);
    CHECK(!got.bracketColor.has_value());
}

// The position and color belong to the group symbol, so without a symbol there is nothing to write.
TEST(partGroupRoundTrip, bracketPositionAndColorWithoutBracketTypeIsNotWritten)
{
    auto in = makeTwoPartScore();
    auto grp = makeBracketGroup();
    grp.bracketType = BracketType::unspecified;
    grp.bracketPositionData.isDefaultXSpecified = true;
    grp.bracketPositionData.defaultX = -10.0;
    grp.bracketColor = ColorData{};
    in.partGroups.push_back(grp);

    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("<group-symbol") == std::string::npos);
    CHECK(xml.find("default-x") == std::string::npos);

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.partGroups.size() == 1);
    CHECK(PositionData{} == out.partGroups.at(0).bracketPositionData);
    CHECK(!out.partGroups.at(0).bracketColor.has_value());
}

TEST(partGroupRoundTrip, bracketPositionAndColorEquality)
{
    const PartGroupData base;

    PartGroupData withPosition;
    withPosition.bracketPositionData.isDefaultXSpecified = true;
    withPosition.bracketPositionData.defaultX = -10.0;
    CHECK(base != withPosition);

    PartGroupData withColor;
    withColor.bracketColor = ColorData{};
    CHECK(base != withColor);
}

#endif
