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
    auto &mgr = DocumentManager::getInstance();
    const auto rDocId = mgr.createFromScore(score);
    REQUIRE(rDocId.ok());
    int docId = rDocId.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto rDocId2 = mgr.createFromStream(iss);
    REQUIRE(rDocId2.ok());
    docId = rDocId2.value();
    const auto rOscore = mgr.getData(docId);
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
    auto &mgr = DocumentManager::getInstance();
    const auto rDocId = mgr.createFromScore(score);
    REQUIRE(rDocId.ok());
    int docId = rDocId.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{xml};
    const auto rDocId2 = mgr.createFromStream(iss);
    REQUIRE(rDocId2.ok());
    docId = rDocId2.value();
    const auto rOscore = mgr.getData(docId);
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

#endif
