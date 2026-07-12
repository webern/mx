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

namespace
{
// A minimal one-note, one-measure, one-staff score with a single part carrying the given
// <score-part>/<group> memberships.
ScoreData makeScoreWithGroups(const std::vector<std::string> &groups)
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
    pd.uniqueId = "P1";
    pd.name = "MusicXML Part";
    pd.groups = groups;
    pd.measures.push_back(m);

    ScoreData s;
    s.parts.push_back(pd);
    return s;
}
} // namespace

TEST(scorePartGroupRoundTrips, ScorePartGroup)
{
    const auto in = makeScoreWithGroups({"score"});
    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("<group>score</group>") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    REQUIRE(out.parts.size() == 1u);
    REQUIRE(out.parts.front().groups.size() == 1u);
    CHECK_EQUAL("score", out.parts.front().groups.front());
}

T_END;

TEST(scorePartMultipleGroupsRoundTrip, ScorePartGroup)
{
    const auto in = makeScoreWithGroups({"score", "parts"});
    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1u);
    REQUIRE(out.parts.front().groups.size() == 2u);
    CHECK_EQUAL("score", out.parts.front().groups.at(0));
    CHECK_EQUAL("parts", out.parts.front().groups.at(1));
}

T_END;

TEST(scorePartNoGroupsOmitsElement, ScorePartGroup)
{
    const auto in = makeScoreWithGroups({});
    const auto xml = mxtest::toXml(in);
    CHECK(xml.find("<group>") == std::string::npos);
}

T_END;

#endif
