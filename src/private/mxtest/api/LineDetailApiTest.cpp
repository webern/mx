// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace std;
using namespace mx::api;
using namespace mxtest;

// A one-measure, one-staff score with a single note, so the caller can set staff details on it.
static ScoreData makeScoreForLineDetail()
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.voices[0].notes.emplace_back();
    return score;
}

// Wrap the contents of a <staff-details> element in the smallest score that can carry one.
static std::string staffDetailsXmlForLineDetail(const std::string &inStaffDetailsContent)
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
        <staff-details>
          )" +
           inStaffDetailsContent + R"(
        </staff-details>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <type>whole</type>
      </note>
    </measure>
  </part>
</score-partwise>)";
}

static LineDetailData makeLineDetailForTest(int line)
{
    LineDetailData lineDetail;
    lineDetail.line = line;
    return lineDetail;
}

// A five-line staff with its middle line hidden, its top line thick and red, and its bottom line
// dashed comes back exactly as it was set.
TEST(lineDetailsRoundTrip, LineDetail)
{
    auto score = makeScoreForLineDetail();
    auto &staff = score.parts.back().measures.back().staves.back();
    staff.staffLines = 5;

    auto bottom = makeLineDetailForTest(1);
    bottom.lineType = LineType::dashed;
    auto middle = makeLineDetailForTest(3);
    middle.printObject = Bool::no;
    auto top = makeLineDetailForTest(5);
    top.width = 2.5;
    top.color = ColorData{};
    top.color->red = 255;
    top.color->green = 0;
    top.color->blue = 0;
    top.printObject = Bool::yes;
    staff.lineDetails = {bottom, middle, top};

    const auto outScore = fromXml(toXml(score));
    const auto &outStaff = outScore.parts.back().measures.back().staves.back();
    CHECK_EQUAL(5, outStaff.staffLines);
    CHECK(staff.lineDetails == outStaff.lineDetails);
}

T_END;

// Every line type comes back as it was set.
TEST(lineDetailEveryLineTypeRoundTrip, LineDetail)
{
    for (const auto lineType : {LineType::solid, LineType::dashed, LineType::dotted, LineType::wavy})
    {
        auto score = makeScoreForLineDetail();
        auto &staff = score.parts.back().measures.back().staves.back();
        staff.staffLines = 1;
        auto lineDetail = makeLineDetailForTest(1);
        lineDetail.lineType = lineType;
        staff.lineDetails = {lineDetail};

        const auto outScore = fromXml(toXml(score));
        const auto &outStaff = outScore.parts.back().measures.back().staves.back();
        REQUIRE(outStaff.lineDetails.size() == 1);
        CHECK(lineType == outStaff.lineDetails.front().lineType);
    }
}

T_END;

// Every attribute of <line-detail> reaches LineDetailData.
TEST(lineDetailReadFromXml, LineDetail)
{
    const auto score = fromXml(staffDetailsXmlForLineDetail(R"(<staff-lines>2</staff-lines>
          <line-detail line="1" width="1.5" color="#80FF0000" line-type="dotted" print-object="no"/>
          <line-detail line="2"/>)"));
    const auto &staff = score.parts.back().measures.back().staves.back();
    CHECK_EQUAL(2, staff.staffLines);
    REQUIRE(staff.lineDetails.size() == 2);

    const auto &first = staff.lineDetails.at(0);
    CHECK_EQUAL(1, first.line);
    REQUIRE(first.width.has_value());
    CHECK_DOUBLES_EQUAL(1.5, *first.width, 0.0001);
    REQUIRE(first.color.has_value());
    CHECK(first.color->isAlphaSpecified);
    CHECK_EQUAL(0x80, static_cast<int>(first.color->alpha));
    CHECK_EQUAL(255, static_cast<int>(first.color->red));
    CHECK_EQUAL(0, static_cast<int>(first.color->green));
    CHECK_EQUAL(0, static_cast<int>(first.color->blue));
    CHECK(LineType::dotted == first.lineType);
    CHECK(Bool::no == first.printObject);

    const auto &second = staff.lineDetails.at(1);
    CHECK_EQUAL(2, second.line);
    CHECK(!second.width.has_value());
    CHECK(!second.color.has_value());
    CHECK(LineType::unspecified == second.lineType);
    CHECK(Bool::unspecified == second.printObject);
}

T_END;

// The line details are written as <line-detail> elements after <staff-lines>.
TEST(lineDetailSerializes, LineDetail)
{
    auto score = makeScoreForLineDetail();
    auto &staff = score.parts.back().measures.back().staves.back();
    staff.staffLines = 3;
    auto lineDetail = makeLineDetailForTest(2);
    lineDetail.width = 2.0;
    lineDetail.color = ColorData{};
    lineDetail.color->red = 0;
    lineDetail.color->green = 0;
    lineDetail.color->blue = 255;
    lineDetail.lineType = LineType::wavy;
    lineDetail.printObject = Bool::no;
    staff.lineDetails = {lineDetail};

    const auto xml = toXml(score);
    const auto staffLinesPos = xml.find("<staff-lines>3</staff-lines>");
    const auto lineDetailPos = xml.find("<line-detail");
    REQUIRE(staffLinesPos != std::string::npos);
    REQUIRE(lineDetailPos != std::string::npos);
    CHECK(staffLinesPos < lineDetailPos);
    CHECK(xml.find(R"(line="2")") != std::string::npos);
    CHECK(xml.find(R"(width="2")") != std::string::npos);
    CHECK(xml.find(R"(color="#0000FF")") != std::string::npos);
    CHECK(xml.find(R"(line-type="wavy")") != std::string::npos);
    CHECK(xml.find(R"(print-object="no")") != std::string::npos);
}

T_END;

// A staff without line details reads back with none and does not write a <line-detail>. A
// <line-detail> that sets only its line number does not write any other attribute.
TEST(lineDetailAbsentAndBare, LineDetail)
{
    const auto plain = fromXml(staffDetailsXmlForLineDetail("<staff-lines>5</staff-lines>"));
    const auto &plainStaff = plain.parts.back().measures.back().staves.back();
    CHECK_EQUAL(5, plainStaff.staffLines);
    CHECK(plainStaff.lineDetails.empty());
    CHECK(toXml(plain).find("<line-detail") == std::string::npos);

    auto score = makeScoreForLineDetail();
    auto &staff = score.parts.back().measures.back().staves.back();
    staff.staffLines = 5;
    staff.lineDetails = {makeLineDetailForTest(4)};
    const auto xml = toXml(score);
    CHECK(xml.find(R"(<line-detail line="4")") != std::string::npos);
    CHECK(xml.find("width=") == std::string::npos);
    CHECK(xml.find("color=") == std::string::npos);
    CHECK(xml.find("line-type=") == std::string::npos);
    CHECK(xml.find("print-object=") == std::string::npos);
}

T_END;

// Two staves that differ only in how one line is drawn are not equal.
TEST(lineDetailEquality, LineDetail)
{
    StaffData base;
    base.staffLines = 5;
    base.lineDetails = {makeLineDetailForTest(3)};

    auto hidden = base;
    hidden.lineDetails.front().printObject = Bool::no;
    CHECK(base != hidden);

    auto wider = base;
    wider.lineDetails.front().width = 2.0;
    CHECK(base != wider);

    auto colored = base;
    colored.lineDetails.front().color = ColorData{};
    CHECK(base != colored);

    auto dashed = base;
    dashed.lineDetails.front().lineType = LineType::dashed;
    CHECK(base != dashed);

    auto otherLine = base;
    otherLine.lineDetails.front().line = 2;
    CHECK(base != otherLine);

    StaffData none;
    none.staffLines = 5;
    CHECK(base != none);
}

T_END;

#endif
