// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/MxFileRepository.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
constexpr const char *const fileName = "testAccidentals1.xml";

ScoreData makeOneMeasureScore()
{
    ScoreData score;
    score.parts.emplace_back(PartData{});
    auto &part = score.parts.back();
    part.measures.emplace_back(MeasureData{});
    auto &measure = part.measures.back();
    measure.staves.emplace_back(StaffData{});
    return score;
}
} // namespace

TEST(implicitCarryover, TimeSignatureApi)
{
    const auto scoreData = mxtest::MxFileRepository::loadFile(fileName);
    CHECK_EQUAL(1, scoreData.parts.size());
    const auto &part = scoreData.parts.front();
    CHECK_EQUAL(2, part.measures.size())
    auto measureIter = part.measures.cbegin();
    auto t = measureIter->timeSignatures.front();

    CHECK(!t.isImplicit);
    CHECK_EQUAL(1, t.components.size());
    CHECK_EQUAL("3", t.components.front().beats);
    CHECK_EQUAL("4", t.components.front().beatType);
    CHECK(t.symbol == TimeSignatureSymbol::unspecified);

    ++measureIter;
    t = measureIter->timeSignatures.front();
    CHECK(t.isImplicit);
    CHECK_EQUAL("3", t.components.front().beats);
    CHECK_EQUAL("4", t.components.front().beatType);
    CHECK(t.symbol == TimeSignatureSymbol::unspecified);
}

TEST(setSimple, TimeSignatureData)
{
    TimeSignatureData t;
    t.components.push_back(TimeSignatureComponent{"3", "8"});
    CHECK_EQUAL(2, t.components.size());
    t.setSimple("5", "4");
    CHECK_EQUAL(1, t.components.size());
    CHECK_EQUAL("5", t.components.front().beats);
    CHECK_EQUAL("4", t.components.front().beatType);
}

TEST(TimeSignatureDataEquality_change_staffIndex, TimeSignatureData)
{
    TimeSignatureData t1;
    t1.setSimple("6", "8");
    t1.isImplicit = false;
    t1.staffIndex = 0;
    auto t2 = t1;
    CHECK(t1 == t2);
    CHECK(!(t1 != t2));

    // change one thing
    t1.staffIndex += 1;
    CHECK(t1 != t2);
    CHECK(!(t1 == t2));
}

TEST(TimeSignatureDataEquality_change_components, TimeSignatureData)
{
    TimeSignatureData t1;
    t1.setSimple("6", "8");
    auto t2 = t1;
    CHECK(t1 == t2);

    t1.components.push_back(TimeSignatureComponent{"3", "4"});
    CHECK(t1 != t2);
    CHECK(!(t1 == t2));
}

TEST(staffScopedRoundTrip, TimeSignatureApi)
{
    auto score = makeOneMeasureScore();
    auto &measure = score.parts.front().measures.front();
    measure.staves.emplace_back(StaffData{});

    TimeSignatureData t1;
    t1.setSimple("3", "4");
    t1.isImplicit = false;
    t1.staffIndex = 0;

    TimeSignatureData t2;
    t2.setSimple("6", "8");
    t2.isImplicit = false;
    t2.staffIndex = 1;

    measure.timeSignatures = {t1, t2};

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("number=\"1\"") != std::string::npos);
    CHECK(xml.find("number=\"2\"") != std::string::npos);

    const auto result = mxtest::fromXml(xml);
    REQUIRE(result.parts.size() == 1);
    REQUIRE(result.parts.front().measures.size() == 1);
    const auto &resultTimeSignatures = result.parts.front().measures.front().timeSignatures;
    REQUIRE(resultTimeSignatures.size() == 2);

    bool foundStaff0 = false;
    bool foundStaff1 = false;
    for (const auto &ts : resultTimeSignatures)
    {
        if (ts.staffIndex == 0)
        {
            foundStaff0 = true;
            CHECK_EQUAL("3", ts.components.front().beats);
            CHECK_EQUAL("4", ts.components.front().beatType);
        }
        else if (ts.staffIndex == 1)
        {
            foundStaff1 = true;
            CHECK_EQUAL("6", ts.components.front().beats);
            CHECK_EQUAL("8", ts.components.front().beatType);
        }
    }
    CHECK(foundStaff0);
    CHECK(foundStaff1);
}

TEST(compositeMultiComponentRoundTrip, TimeSignatureApi)
{
    const auto scoreData = mxtest::MxFileRepository::loadFile("timesigs_composite-ref.musicxml");
    REQUIRE(scoreData.parts.size() == 1);
    const auto &part = scoreData.parts.front();
    REQUIRE(part.measures.size() == 1);
    const auto &measure = part.measures.front();
    REQUIRE(measure.timeSignatures.size() == 1);
    const auto &t = measure.timeSignatures.front();
    CHECK(!t.isImplicit);
    REQUIRE(t.components.size() == 3);
    CHECK_EQUAL("1 + 2.5", t.components.at(0).beats);
    CHECK_EQUAL("2 + 4", t.components.at(0).beatType);
    CHECK_EQUAL("3 + 2", t.components.at(1).beats);
    CHECK_EQUAL("16", t.components.at(1).beatType);
    CHECK_EQUAL("1.5 + 5", t.components.at(2).beats);
    CHECK_EQUAL("8 + 16", t.components.at(2).beatType);

    // round trip through write/read again and confirm the composite survives
    const auto xml = mxtest::toXml(scoreData);
    const auto result = mxtest::fromXml(xml);
    REQUIRE(result.parts.size() == 1);
    REQUIRE(result.parts.front().measures.size() == 1);
    const auto &resultTimeSig = result.parts.front().measures.front().timeSignatures.front();
    REQUIRE(resultTimeSig.components.size() == 3);
    CHECK(resultTimeSig.components == t.components);
}

T_END

#endif
