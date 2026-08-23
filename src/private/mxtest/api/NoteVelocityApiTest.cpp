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

// A one-measure, one-note score. The caller fills in the note's velocities.
ScoreData noteVelocityMakeScore()
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 4;
    return score;
}

const NoteData &noteVelocityFirstNote(const ScoreData &score)
{
    return score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
}

TEST(velocitiesSurviveRoundTrip, NoteVelocity)
{
    auto score = noteVelocityMakeScore();
    auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    note.velocityStart = 71.5;
    note.velocityStop = 40.0;

    const auto out = roundTrip(score);
    const auto &outNote = noteVelocityFirstNote(out);
    REQUIRE(outNote.velocityStart.has_value());
    REQUIRE(outNote.velocityStop.has_value());
    CHECK_DOUBLES_EQUAL(71.5, *outNote.velocityStart, 0.0001);
    CHECK_DOUBLES_EQUAL(40.0, *outNote.velocityStop, 0.0001);
}

TEST(velocitiesWriteMusicXmlAttributes, NoteVelocity)
{
    auto score = noteVelocityMakeScore();
    auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    note.velocityStart = 100.0;
    note.velocityStop = 60.0;

    const auto xml = toXml(score);
    CHECK(xml.find("dynamics=\"100\"") != std::string::npos);
    CHECK(xml.find("end-dynamics=\"60\"") != std::string::npos);
}

TEST(absentVelocitiesWriteNothing, NoteVelocity)
{
    const auto score = noteVelocityMakeScore();
    const auto xml = toXml(score);
    CHECK(xml.find("dynamics=") == std::string::npos);

    const auto out = roundTrip(score);
    const auto &outNote = noteVelocityFirstNote(out);
    CHECK(!outNote.velocityStart.has_value());
    CHECK(!outNote.velocityStop.has_value());
}

TEST(velocitiesAreReadFromMusicXml, NoteVelocity)
{
    auto score = noteVelocityMakeScore();
    auto &note = score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
    note.velocityStart = 12.5;

    const auto in = fromXml(toXml(score));
    const auto &inNote = noteVelocityFirstNote(in);
    REQUIRE(inNote.velocityStart.has_value());
    CHECK_DOUBLES_EQUAL(12.5, *inNote.velocityStart, 0.0001);
    CHECK(!inNote.velocityStop.has_value());
}

#endif
