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
ScoreData makeScoreWithMidiName(const std::string &midiName)
{
    VoiceData voiceData;
    NoteData n;
    n.tickTimePosition = 0;
    n.pitchData.step = Step::c;
    n.pitchData.octave = 5;
    n.durationData.durationName = DurationName::quarter;
    n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
    voiceData.notes.push_back(n);
    StaffData staff{};
    staff.voices.emplace(0, voiceData);
    MeasureData m;
    m.staves.push_back(staff);

    PartData pd;
    pd.uniqueId = "P1";
    pd.name = "Flute";
    pd.displayName = "Flute";
    pd.instrumentData.uniqueId = "P1-I1";
    pd.instrumentData.midiData.name = midiName;
    pd.instrumentData.midiData.channel = 1;
    pd.measures.push_back(m);

    ScoreData s;
    s.parts.push_back(pd);
    return s;
}
} // namespace

TEST(midiNameRoundTrip, survivesWriteAndRead)
{
    const std::string expected = "Flute Player One";
    const auto in = makeScoreWithMidiName(expected);
    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(expected, out.parts.at(0).instrumentData.midiData.name);
}

#endif
