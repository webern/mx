// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// Build a minimal one-note, one-measure, one-staff score with a single direction carrying the
// given (already-populated) SoundData on staff 0.
ScoreData makeScoreWithSound(const SoundData &inSound)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();

    DirectionData direction;
    direction.isStaffValueSpecified = false;
    direction.isSoundDataSpecified = true;
    direction.soundData = inSound;
    staff.directions.emplace_back(std::move(direction));
    return score;
}

const DirectionData *findSoundDirection(const ScoreData &score)
{
    if (score.parts.empty() || score.parts.back().measures.empty())
    {
        return nullptr;
    }
    const auto &measure = score.parts.back().measures.back();
    if (measure.staves.empty())
    {
        return nullptr;
    }
    for (const auto &direction : measure.staves.front().directions)
    {
        if (direction.isSoundDataSpecified)
        {
            return &direction;
        }
    }
    return nullptr;
}
} // namespace

TEST(standaloneSoundTempoRoundTrips, SoundApi)
{
    SoundData sound;
    sound.tempo = 120.0;

    const auto score = makeScoreWithSound(sound);
    const auto out = roundTrip(score);

    const auto *direction = findSoundDirection(out);
    REQUIRE(direction != nullptr);
    CHECK(direction->isSoundDataSpecified);
    CHECK_DOUBLES_EQUAL(120.0, direction->soundData.tempo, MX_API_EQUALITY_EPSILON);
}

T_END;

TEST(standaloneSoundEmitsSoundElement, SoundApi)
{
    SoundData sound;
    sound.tempo = 120.0;

    const auto score = makeScoreWithSound(sound);
    const auto xml = toXml(score);

    // The standalone <sound> must be present with a tempo attribute (not wrapped in a <direction>).
    CHECK(xml.find("<sound") != std::string::npos);
    CHECK(xml.find("tempo=\"120\"") != std::string::npos);
}

T_END;

TEST(standaloneSoundDynamicsRoundTrips, SoundApi)
{
    SoundData sound;
    sound.dynamics = 75.0;

    const auto score = makeScoreWithSound(sound);
    const auto out = roundTrip(score);

    const auto *direction = findSoundDirection(out);
    REQUIRE(direction != nullptr);
    CHECK_DOUBLES_EQUAL(75.0, direction->soundData.dynamics, MX_API_EQUALITY_EPSILON);
}

T_END;

TEST(standaloneSoundNavigationRoundTrips, SoundApi)
{
    SoundData sound;
    sound.dacapo = Bool::yes;
    sound.forwardRepeat = Bool::yes;
    sound.pizzicato = Bool::no;
    sound.segno = "1";
    sound.dalsegno = "2";
    sound.coda = "3";
    sound.tocoda = "4";
    sound.fine = "yes";

    const auto score = makeScoreWithSound(sound);
    const auto out = roundTrip(score);

    const auto *direction = findSoundDirection(out);
    REQUIRE(direction != nullptr);
    const auto &rt = direction->soundData;
    CHECK(Bool::yes == rt.dacapo);
    CHECK(Bool::yes == rt.forwardRepeat);
    CHECK(Bool::no == rt.pizzicato);
    CHECK_EQUAL("1", rt.segno);
    CHECK_EQUAL("2", rt.dalsegno);
    CHECK_EQUAL("3", rt.coda);
    CHECK_EQUAL("4", rt.tocoda);
    CHECK_EQUAL("yes", rt.fine);
}

T_END;

TEST(directionChildSoundRoundTrips, SoundApi)
{
    // A direction that has other content (words) plus a sound: the <sound> is written as a child of
    // the <direction>.
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();

    DirectionData direction;
    WordsData words;
    words.text = "Allegro";
    direction.words.emplace_back(std::move(words));
    direction.isSoundDataSpecified = true;
    direction.soundData.tempo = 144.0;
    staff.directions.emplace_back(std::move(direction));

    const auto out = roundTrip(score);

    const auto *outDirection = findSoundDirection(out);
    REQUIRE(outDirection != nullptr);
    REQUIRE(outDirection->words.size() == 1u);
    CHECK_EQUAL("Allegro", outDirection->words.front().text);
    CHECK_DOUBLES_EQUAL(144.0, outDirection->soundData.tempo, MX_API_EQUALITY_EPSILON);
}

T_END;

#endif
