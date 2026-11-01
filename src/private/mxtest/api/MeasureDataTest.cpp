// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST(forwardRepeat, MeasureData)
{
    const auto expectedBarlineType = mx::api::BarlineType::heavyLight;
    const bool isRepeatExpected = true;

    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    measure.barlines.emplace_back();
    auto &barlineData = measure.barlines.back();
    barlineData.barlineType = mx::api::BarlineType::heavyLight;
    barlineData.repeat = true;

    // round trip it through xml
    auto rDocId = fromScore(score);
    REQUIRE(rDocId.ok());
    std::stringstream ss;
    std::move(rDocId).value().writeToStream(ss);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    auto rDocId2 = MusicXml::fromStream(iss);
    REQUIRE(rDocId2.ok());
    const auto rOscore = getScore(std::move(rDocId2).value());
    REQUIRE(rOscore.ok());
    const auto oscore = rOscore.value();

    // get the data after the round trip
    const auto &opart = oscore.parts.back();
    const auto &omeasure = opart.measures.back();
    const auto &obarlines = omeasure.barlines;

    CHECK_EQUAL(1, obarlines.size());

    const auto &barline = obarlines.front();

    CHECK(expectedBarlineType == barline.barlineType);
    CHECK(isRepeatExpected == barline.repeat);
}

T_END;

TEST(backwardRepeat, MeasureData)
{
    const auto expectedBarlineType = mx::api::BarlineType::lightHeavy;
    const bool isRepeatExpected = true;

    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    measure.barlines.emplace_back();
    auto &barlineData = measure.barlines.back();
    barlineData.barlineType = expectedBarlineType;
    barlineData.tickTimePosition = 480;
    barlineData.repeat = true;

    // round trip it through xml
    auto rDocId = fromScore(score);
    REQUIRE(rDocId.ok());
    std::stringstream ss;
    std::move(rDocId).value().writeToStream(ss);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    auto rDocId2 = MusicXml::fromStream(iss);
    REQUIRE(rDocId2.ok());
    const auto rOscore = getScore(std::move(rDocId2).value());
    REQUIRE(rOscore.ok());
    const auto oscore = rOscore.value();

    // get the data after the round trip
    const auto &opart = oscore.parts.back();
    const auto &omeasure = opart.measures.back();
    const auto &obarlines = omeasure.barlines;

    CHECK_EQUAL(1, obarlines.size());

    const auto &barline = obarlines.front();

    CHECK(expectedBarlineType == barline.barlineType);
    CHECK(isRepeatExpected == barline.repeat);
}

T_END;

TEST(tickAndShortBarlinesRoundTrip, MeasureData)
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
    auto &tickBarline = measure.barlines.back();
    tickBarline.barlineType = BarlineType::tick;

    measure.barlines.emplace_back();
    auto &shortBarline = measure.barlines.back();
    shortBarline.barlineType = BarlineType::short_;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<bar-style>tick</bar-style>") != std::string::npos);
    CHECK(xml.find("<bar-style>short</bar-style>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(outScore.parts.size() == 1);
    REQUIRE(outScore.parts.front().measures.size() == 1);
    const auto &outBarlines = outScore.parts.front().measures.front().barlines;
    REQUIRE(outBarlines.size() == 2);
    CHECK(outBarlines.at(0).barlineType == BarlineType::tick);
    CHECK(outBarlines.at(1).barlineType == BarlineType::short_);
}

T_END;

TEST(staffLinesRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffLines = 1;
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-details>") != std::string::npos);
    CHECK(xml.find("<staff-lines>1</staff-lines>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(1, outScore.parts.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.front().staffLines);
}

T_END;

TEST(staffSizeRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffSize = 80.5;
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-details>") != std::string::npos);
    CHECK(xml.find("<staff-size>80.5</staff-size>") != std::string::npos);
    CHECK(xml.find("<staff-lines>") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(1, outScore.parts.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.size());
    CHECK(80.5 == outScore.parts.front().measures.front().staves.front().staffSize);
    CHECK(-1.0 == outScore.parts.front().measures.front().staves.front().staffScaling);
    CHECK_EQUAL(-1, outScore.parts.front().measures.front().staves.front().staffLines);
}

T_END;

TEST(staffSizeScalingRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffSize = 80.5;
    staff.staffScaling = 75.5;
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-details>") != std::string::npos);
    CHECK(xml.find("<staff-size scaling=\"75.5\">80.5</staff-size>") != std::string::npos);
    CHECK(xml.find("<staff-lines>") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(1, outScore.parts.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.size());
    CHECK(80.5 == outScore.parts.front().measures.front().staves.front().staffSize);
    CHECK(75.5 == outScore.parts.front().measures.front().staves.front().staffScaling);
    CHECK_EQUAL(-1, outScore.parts.front().measures.front().staves.front().staffLines);
}

T_END;

TEST(staffLinesAndStaffSizeRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffLines = 1;
    staff.staffSize = 80.5;
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-details>") != std::string::npos);
    CHECK(xml.find("<staff-lines>1</staff-lines>") != std::string::npos);
    CHECK(xml.find("<staff-size>80.5</staff-size>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(1, outScore.parts.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.front().staves.front().staffLines);
    CHECK(80.5 == outScore.parts.front().measures.front().staves.front().staffSize);
    CHECK(-1.0 == outScore.parts.front().measures.front().staves.front().staffScaling);
}

T_END;

StaffTuningData makeStaffTuningForMeasureDataTest(int line, Step step, int octave, int alter = 0, double cents = 0.0)
{
    StaffTuningData tuning;
    tuning.line = line;
    tuning.tuningStep = step;
    tuning.tuningOctave = octave;
    tuning.tuningAlter = alter;
    tuning.tuningCents = cents;
    return tuning;
}

TEST(staffTuningRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffLines = 6;
    staff.staffTunings = {
        makeStaffTuningForMeasureDataTest(1, Step::e, 2), makeStaffTuningForMeasureDataTest(2, Step::a, 2),
        makeStaffTuningForMeasureDataTest(3, Step::d, 3), makeStaffTuningForMeasureDataTest(4, Step::g, 3),
        makeStaffTuningForMeasureDataTest(5, Step::b, 3), makeStaffTuningForMeasureDataTest(6, Step::e, 4)};
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-tuning line=\"1\">") != std::string::npos);
    CHECK(xml.find("<staff-tuning line=\"6\">") != std::string::npos);
    CHECK(xml.find("<tuning-step>E</tuning-step>") != std::string::npos);
    CHECK(xml.find("<tuning-alter>") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(1 == outScore.parts.size());
    REQUIRE(1 == outScore.parts.front().measures.size());
    REQUIRE(1 == outScore.parts.front().measures.front().staves.size());
    const auto &outStaff = outScore.parts.front().measures.front().staves.front();
    CHECK_EQUAL(6, outStaff.staffLines);
    CHECK(staff.staffTunings == outStaff.staffTunings);
}

T_END;

TEST(staffTuningAlterAndCentsRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffLines = 2;
    staff.staffTunings = {makeStaffTuningForMeasureDataTest(1, Step::f, 3, 1),
                          makeStaffTuningForMeasureDataTest(2, Step::c, 4, 0, 50.0)};
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<tuning-alter>1</tuning-alter>") != std::string::npos);
    CHECK(xml.find("<tuning-alter>0.5</tuning-alter>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(1 == outScore.parts.size());
    REQUIRE(1 == outScore.parts.front().measures.size());
    REQUIRE(1 == outScore.parts.front().measures.front().staves.size());
    const auto &outStaff = outScore.parts.front().measures.front().staves.front();
    REQUIRE(2 == outStaff.staffTunings.size());
    CHECK_EQUAL(1, outStaff.staffTunings.at(0).tuningAlter);
    CHECK(0.0 == outStaff.staffTunings.at(0).tuningCents);
    CHECK_EQUAL(0, outStaff.staffTunings.at(1).tuningAlter);
    CHECK(50.0 == outStaff.staffTunings.at(1).tuningCents);
    CHECK(staff.staffTunings == outStaff.staffTunings);
}

T_END;

TEST(staffTuningWithoutStaffLinesRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffTunings = {makeStaffTuningForMeasureDataTest(1, Step::d, 3)};
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-lines>") == std::string::npos);
    CHECK(xml.find("<staff-tuning line=\"1\">") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(1 == outScore.parts.size());
    REQUIRE(1 == outScore.parts.front().measures.size());
    REQUIRE(1 == outScore.parts.front().measures.front().staves.size());
    const auto &outStaff = outScore.parts.front().measures.front().staves.front();
    CHECK_EQUAL(-1, outStaff.staffLines);
    CHECK(staff.staffTunings == outStaff.staffTunings);
}

T_END;

TEST(staffTuningIsWrittenBetweenStaffLinesAndStaffSize, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    staff.staffLines = 1;
    staff.staffSize = 80.5;
    staff.staffTunings = {makeStaffTuningForMeasureDataTest(1, Step::a, 2)};
    staff.voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    const auto linesPosition = xml.find("<staff-lines>");
    const auto tuningPosition = xml.find("<staff-tuning");
    const auto sizePosition = xml.find("<staff-size>");
    REQUIRE(linesPosition != std::string::npos);
    REQUIRE(tuningPosition != std::string::npos);
    REQUIRE(sizePosition != std::string::npos);
    CHECK(linesPosition < tuningPosition);
    CHECK(tuningPosition < sizePosition);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(1 == outScore.parts.size());
    REQUIRE(1 == outScore.parts.front().measures.size());
    REQUIRE(1 == outScore.parts.front().measures.front().staves.size());
    const auto &outStaff = outScore.parts.front().measures.front().staves.front();
    CHECK_EQUAL(1, outStaff.staffLines);
    CHECK(80.5 == outStaff.staffSize);
    CHECK(staff.staffTunings == outStaff.staffTunings);
}

T_END;

TEST(staffTuningOnSecondStaffOnly, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    measure.staves.emplace_back();
    measure.staves.at(0).voices[0].notes.emplace_back();
    measure.staves.at(1).voices[0].notes.emplace_back();
    measure.staves.at(1).staffLines = 4;
    measure.staves.at(1).staffTunings = {makeStaffTuningForMeasureDataTest(1, Step::e, 1),
                                         makeStaffTuningForMeasureDataTest(2, Step::a, 1)};

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<staff-details number=\"2\">") != std::string::npos);
    CHECK(xml.find("<staff-details number=\"1\">") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    REQUIRE(1 == outScore.parts.size());
    REQUIRE(1 == outScore.parts.front().measures.size());
    REQUIRE(2 == outScore.parts.front().measures.front().staves.size());
    const auto &outStaves = outScore.parts.front().measures.front().staves;
    CHECK(outStaves.at(0).staffTunings.empty());
    CHECK(measure.staves.at(1).staffTunings == outStaves.at(1).staffTunings);
}

T_END;

TEST(measureNumberingRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.measureNumbering = MeasureNumbering::system;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<measure-numbering>system</measure-numbering>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(1, outScore.parts.size());
    CHECK_EQUAL(1, outScore.parts.front().measures.size());
    CHECK(MeasureNumbering::system == outScore.parts.front().measures.front().measureNumbering);
}

T_END;

TEST(measureNumberingAttributesRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.measureNumbering = MeasureNumbering::system;
    measure.measureNumberingMultipleRestAlways = Bool::yes;
    measure.measureNumberingMultipleRestRange = Bool::no;
    measure.measureNumberingSystemRelation = SystemRelation::onlyTop;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("multiple-rest-always=\"yes\"") != std::string::npos);
    CHECK(xml.find("multiple-rest-range=\"no\"") != std::string::npos);
    CHECK(xml.find("system=\"only-top\"") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    const auto &outMeasure = outScore.parts.front().measures.front();
    CHECK(MeasureNumbering::system == outMeasure.measureNumbering);
    CHECK(Bool::yes == outMeasure.measureNumberingMultipleRestAlways);
    CHECK(Bool::no == outMeasure.measureNumberingMultipleRestRange);
    CHECK(SystemRelation::onlyTop == outMeasure.measureNumberingSystemRelation);
}

T_END;

TEST(measureNumberingUnspecifiedOmitsElement, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<measure-numbering>") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK(MeasureNumbering::unspecified == outScore.parts.front().measures.front().measureNumbering);
}

T_END;

TEST(measureNumberingStaffRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.measureNumbering = MeasureNumbering::system;
    measure.measureNumberingStaffIndex = 1;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("staff=\"2\"") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    const auto &outMeasure = outScore.parts.front().measures.front();
    REQUIRE(outMeasure.measureNumberingStaffIndex.has_value());
    CHECK_EQUAL(1, *outMeasure.measureNumberingStaffIndex);
}

T_END;

TEST(measureNumberingStaffAbsentOmitsAttribute, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.measureNumbering = MeasureNumbering::system;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<measure-numbering staff=") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK(!outScore.parts.front().measures.front().measureNumberingStaffIndex.has_value());
}

T_END;

TEST(displayedNumberRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.number = "7";
    measure.displayedNumber = "12a";
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("number=\"7\"") != std::string::npos);
    CHECK(xml.find("text=\"12a\"") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    const auto &outMeasure = outScore.parts.front().measures.front();
    CHECK_EQUAL("7", outMeasure.number);
    REQUIRE(outMeasure.displayedNumber.has_value());
    CHECK_EQUAL("12a", *outMeasure.displayedNumber);
}

T_END;

TEST(displayedNumberAbsentOmitsAttribute, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("text=") == std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK(!outScore.parts.front().measures.front().displayedNumber.has_value());
}

T_END;

TEST(emptyDisplayedNumberOmitsAttribute, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.displayedNumber = "";
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("text=") == std::string::npos);
}

T_END;

TEST(multiMeasureRestRoundTrip, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.multiMeasureRest = 4;
    measure.multiMeasureRestUseSymbols = Bool::yes;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<measure-style>") != std::string::npos);
    CHECK(xml.find("<multiple-rest use-symbols=\"yes\">4</multiple-rest>") != std::string::npos);

    const auto outScore = mxtest::fromXml(xml);
    CHECK_EQUAL(4, outScore.parts.front().measures.front().multiMeasureRest);
    CHECK(Bool::yes == outScore.parts.front().measures.front().multiMeasureRestUseSymbols);
}

T_END;

TEST(nonPositiveMultiMeasureRestOmitsElement, MeasureData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.multiMeasureRest = 0;
    measure.multiMeasureRestUseSymbols = Bool::yes;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<multiple-rest>") == std::string::npos);
}

T_END;

TEST(printPrecedesLeftBarline, MeasureData)
{
    // MusicXML orders the start of a measure as <print>, then a left barline, then <attributes>.
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.measureNumbering = MeasureNumbering::system;
    measure.barlines.emplace_back();
    measure.barlines.back().barlineType = BarlineType::heavyLight;
    measure.barlines.back().repeat = true;
    measure.staves.emplace_back();
    measure.staves.back().voices[0].notes.emplace_back();

    SystemData system{};
    system.newSystem = Bool::yes;
    score.layout.emplace(0, LayoutData{system, PageData{}});

    const auto xml = mxtest::toXml(score);
    const auto printPos = xml.find("<print");
    const auto barlinePos = xml.find("<barline");
    const auto attributesPos = xml.find("<attributes");
    REQUIRE(printPos != std::string::npos);
    REQUIRE(barlinePos != std::string::npos);
    REQUIRE(attributesPos != std::string::npos);
    CHECK(xml.find("<barline location=\"left\"") == barlinePos);
    CHECK(printPos < barlinePos);
    CHECK(barlinePos < attributesPos);
    CHECK(xml.find("<measure-numbering>system</measure-numbering>") != std::string::npos);
    CHECK(xml.find("new-system=\"yes\"") != std::string::npos);
}

T_END;

#endif
