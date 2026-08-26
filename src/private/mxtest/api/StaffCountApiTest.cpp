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

// A one-measure score whose single part has the requested number of staves, each holding one
// quarter note so the staves are real.
ScoreData staffCountMakeScore(int numStaves)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();

    for (int i = 0; i < numStaves; ++i)
    {
        measure.staves.emplace_back();
        auto &voice = measure.staves.back().voices[0];
        voice.notes.emplace_back();
        voice.notes.back().durationData.durationName = DurationName::quarter;
        voice.notes.back().durationData.durationTimeTicks = 4;
    }

    return score;
}

TEST(aSingleStaffPartOmitsTheStaffCount, StaffCount)
{
    const auto score = staffCountMakeScore(1);
    const auto xml = toXml(score);
    CHECK(xml.find("<staves>") == std::string::npos);

    const auto out = roundTrip(score);
    CHECK(!out.parts.at(0).writeSingleStaffCount);
}

TEST(aSingleStaffPartCanAskForTheStaffCount, StaffCount)
{
    auto score = staffCountMakeScore(1);
    score.parts.at(0).writeSingleStaffCount = true;

    const auto xml = toXml(score);
    CHECK(xml.find("<staves>1</staves>") != std::string::npos);

    const auto out = roundTrip(score);
    CHECK(out.parts.at(0).writeSingleStaffCount);
    REQUIRE(out.parts.at(0).measures.at(0).staves.size() == 1);
}

// A part with more than one staff needs <staves> no matter what the field says, since without it
// every note would land on staff one.
TEST(aMultiStaffPartAlwaysWritesTheStaffCount, StaffCount)
{
    const auto score = staffCountMakeScore(2);
    const auto xml = toXml(score);
    CHECK(xml.find("<staves>2</staves>") != std::string::npos);

    const auto out = roundTrip(score);
    CHECK(!out.parts.at(0).writeSingleStaffCount);
    REQUIRE(out.parts.at(0).measures.at(0).staves.size() == 2);
}

#endif
