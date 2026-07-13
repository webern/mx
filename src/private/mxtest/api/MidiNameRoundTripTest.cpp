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

TEST(midiDeviceRoundTrip, portSurvivesWriteAndRead)
{
    // A midi-device carrying only attributes (empty text) must still round-trip, e.g.
    // <midi-device port="1"></midi-device>. writeDeviceId defaults to unspecified, so no id is
    // emitted here; only the port is exercised.
    auto in = makeScoreWithMidiName("Flute Player One");
    in.parts.at(0).instrumentData.midiData.devicePort = 1;

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    const auto &midiData = out.parts.at(0).instrumentData.midiData;
    REQUIRE(midiData.devicePort.has_value());
    CHECK_EQUAL(1, *midiData.devicePort);
}

TEST(midiDeviceRoundTrip, writeDeviceIdSurvivesWriteAndRead)
{
    // When a source spells out the device-to-instrument link, mx re-emits it as
    // <midi-device id="P1-I1" ...>, taking the id from the part's instrument (uniqueId "P1-I1"
    // here), so the flag survives the round-trip.
    auto in = makeScoreWithMidiName("Flute Player One");
    in.parts.at(0).instrumentData.midiData.devicePort = 1;
    in.parts.at(0).instrumentData.midiData.writeDeviceId = Bool::yes;

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    CHECK(out.parts.at(0).instrumentData.midiData.writeDeviceId == Bool::yes);
}

#endif
