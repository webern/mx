// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/RoundTrip.h"

using namespace std;
using namespace mx::api;

namespace
{
ScoreData makeScoreWithMeasures(int numMeasures)
{
    std::vector<MeasureData> measures;
    for (int i = 0; i < numMeasures; ++i)
    {
        VoiceData voiceData;
        NoteData n;
        n.tickTimePosition = 0;
        n.pitchData.step = Step::c;
        n.pitchData.octave = 5;
        n.durationData.durationName = DurationName::whole;
        n.durationData.durationTimeTicks = 4 * DEFAULT_TICKS_PER_QUARTER;
        voiceData.notes.push_back(n);
        StaffData staff{};
        staff.voices.emplace(0, voiceData);
        MeasureData m;
        m.staves.push_back(staff);
        measures.push_back(m);
    }

    PartData pd;
    pd.uniqueId = "P1";
    pd.name = "Flute";
    pd.displayName = "Flute";
    pd.measures = measures;

    ScoreData s;
    s.parts.push_back(pd);
    return s;
}
} // namespace

// Per-measure <print> <staff-layout> staff-distance was written by
// MeasureWriter but never read back (parsePrint was a no-op and the
// score-level scan ignored staff-layout). Verify it now survives.
TEST(printLayoutRoundTrip, perMeasureStaffDistance)
{
    auto in = makeScoreWithMeasures(4);

    SystemData sys{};
    sys.newSystem = Bool::yes;
    sys.layout.staffDistance = 87.5;
    sys.layout.systemDistance = 120.0;
    in.layout.emplace(2, LayoutData{sys, PageData{}});

    const auto out = mxtest::roundTrip(in);

    const auto it = out.layout.find(2);
    REQUIRE(it != out.layout.end());
    const auto &sysOut = it->second.system;
    CHECK(Bool::yes == sysOut.newSystem);
    REQUIRE(static_cast<bool>(sysOut.layout.staffDistance));
    CHECK_EQUAL(87.5, sysOut.layout.staffDistance.value());
    REQUIRE(static_cast<bool>(sysOut.layout.systemDistance));
    CHECK_EQUAL(120.0, sysOut.layout.systemDistance.value());
}

#endif
