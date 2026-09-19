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

static ScoreData midiRoundTripMakeScore(const std::string &midiName)
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

// The same score, but with the instrument id left to the library.
static ScoreData midiRoundTripMakeScoreWithNoId(const std::string &midiName)
{
    auto scoreData = midiRoundTripMakeScore(midiName);
    scoreData.parts.at(0).instrumentData.uniqueId.clear();
    return scoreData;
}

// The value of the first id attribute after the given tag name.
static std::string midiRoundTripIdAfterTag(const std::string &xml, const std::string &tag)
{
    const auto tagPosition = xml.find(tag);
    if (tagPosition == std::string::npos)
    {
        return {};
    }
    const auto idPosition = xml.find("id=\"", tagPosition);
    if (idPosition == std::string::npos)
    {
        return {};
    }
    const auto valueStart = idPosition + 4;
    const auto valueEnd = xml.find('"', valueStart);
    if (valueEnd == std::string::npos)
    {
        return {};
    }
    return xml.substr(valueStart, valueEnd - valueStart);
}

TEST(midiInstrumentId, isTheSameOnEveryWrite)
{
    const auto scoreData = midiRoundTripMakeScoreWithNoId("Flute Player One");
    const auto first = mxtest::toXml(scoreData);

    // Writing an unrelated score in between used to shift the id the library gives the
    // instrument, because the counter behind it outlived the call.
    static_cast<void>(mxtest::toXml(midiRoundTripMakeScore("Other Player")));

    const auto second = mxtest::toXml(scoreData);
    CHECK(!first.empty());
    CHECK_EQUAL(first, second);
}

TEST(midiInstrumentId, isReferencedByTheMidiInstrument)
{
    const auto xml = mxtest::toXml(midiRoundTripMakeScoreWithNoId("Flute Player One"));
    const auto scoreInstrumentId = midiRoundTripIdAfterTag(xml, "<score-instrument");
    const auto midiInstrumentId = midiRoundTripIdAfterTag(xml, "<midi-instrument");
    CHECK(!scoreInstrumentId.empty());
    CHECK_EQUAL(scoreInstrumentId, midiInstrumentId);
}

TEST(midiInstrumentId, survivesWriteAndRead)
{
    const auto scoreData = midiRoundTripMakeScoreWithNoId("Flute Player One");
    const auto writtenId = midiRoundTripIdAfterTag(mxtest::toXml(scoreData), "<score-instrument");

    const auto out = mxtest::roundTrip(scoreData);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(writtenId, out.parts.at(0).instrumentData.uniqueId);
}

TEST(midiNameRoundTrip, survivesWriteAndRead)
{
    const std::string expected = "Flute Player One";
    const auto in = midiRoundTripMakeScore(expected);
    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(expected, out.parts.at(0).instrumentData.midiData.name);
}

TEST(midiDeviceRoundTrip, portSurvivesWriteAndRead)
{
    // A midi-device carrying only attributes (empty text) must still round-trip, e.g.
    // <midi-device port="1"></midi-device>. writeDeviceId defaults to unspecified, so no id is
    // emitted here; only the port is exercised.
    auto in = midiRoundTripMakeScore("Flute Player One");
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
    auto in = midiRoundTripMakeScore("Flute Player One");
    in.parts.at(0).instrumentData.midiData.devicePort = 1;
    in.parts.at(0).instrumentData.midiData.writeDeviceId = Bool::yes;

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    CHECK(out.parts.at(0).instrumentData.midiData.writeDeviceId == Bool::yes);
}

// issue #443: midi-unpitched names the MIDI note a percussion instrument sounds. The reader read
// it and the writer never wrote it.
TEST(midiUnpitchedRoundTrip, survivesWriteAndRead)
{
    auto in = midiRoundTripMakeScore("Flute Player One");
    in.parts.at(0).instrumentData.midiData.unpitched = 43;

    const auto written = mxtest::toXml(in);
    CHECK(written.find("<midi-unpitched>43</midi-unpitched>") != std::string::npos);

    const auto out = mxtest::roundTrip(in);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(43, out.parts.at(0).instrumentData.midiData.unpitched);
}

// VALUE_UNSPECIFIED means the part has no unpitched note, and an ordinary pitched instrument
// always does: nothing is written for it.
TEST(midiUnpitchedAbsentByDefault, writesNoElement)
{
    const auto in = midiRoundTripMakeScore("Flute Player One");
    const auto written = mxtest::toXml(in);
    CHECK(written.find("midi-unpitched") == std::string::npos);
}

#endif
