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

// A one-measure, one-staff score with the requested number of voices, each holding a whole note
// so the voices overlap and all of them are written.
ScoreData voiceLabelMakeScore(int numVoices)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();

    for (int i = 0; i < numVoices; ++i)
    {
        auto &voice = measure.staves.back().voices[i];
        voice.notes.emplace_back();
        voice.notes.back().durationData.durationName = DurationName::whole;
        voice.notes.back().durationData.durationTimeTicks = 16;
    }

    return score;
}

TEST(anUnlabeledVoiceIsWrittenWithItsNumber, VoiceLabel)
{
    const auto score = voiceLabelMakeScore(2);
    const auto xml = toXml(score);
    CHECK(xml.find("<voice>1</voice>") != std::string::npos);
    CHECK(xml.find("<voice>2</voice>") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &voices = out.parts.at(0).measures.at(0).staves.at(0).voices;
    REQUIRE(voices.size() == 2);
    CHECK_EQUAL("", voices.at(0).label);
    CHECK_EQUAL("", voices.at(1).label);
}

// MusicXML does not require a voice to be named with a number, and some programs write words.
// The number the api would otherwise give the voice cannot express that.
TEST(aNamedVoiceKeepsItsName, VoiceLabel)
{
    auto score = voiceLabelMakeScore(2);
    score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).label = "FirstVoice";
    score.parts.at(0).measures.at(0).staves.at(0).voices.at(1).label = "SecondVoice";

    const auto xml = toXml(score);
    CHECK(xml.find("<voice>FirstVoice</voice>") != std::string::npos);
    CHECK(xml.find("<voice>SecondVoice</voice>") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &voices = out.parts.at(0).measures.at(0).staves.at(0).voices;
    REQUIRE(voices.size() == 2);
    CHECK_EQUAL("FirstVoice", voices.at(0).label);
    CHECK_EQUAL("SecondVoice", voices.at(1).label);
}

// A lone voice named 5 is written as voice 5 rather than renumbered to 1. Reading it back turns
// the number into the voice's index, so the name is no longer needed and the label comes back
// empty. Writing that again still gives voice 5.
TEST(aVoiceNamedPastTheStaffVoiceCountKeepsItsNumber, VoiceLabel)
{
    auto score = voiceLabelMakeScore(1);
    score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).label = "5";

    const auto xml = toXml(score);
    CHECK(xml.find("<voice>5</voice>") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &voices = out.parts.at(0).measures.at(0).staves.at(0).voices;
    REQUIRE(voices.size() == 1);
    REQUIRE(voices.count(4) == 1);
    CHECK_EQUAL("", voices.at(4).label);
    CHECK(toXml(out).find("<voice>5</voice>") != std::string::npos);
}

// Reading drops a name that is just the number the writer would have used anyway, so the common
// case comes back with an empty label.
TEST(aNameThatMatchesTheVoiceNumberIsDropped, VoiceLabel)
{
    auto score = voiceLabelMakeScore(2);
    score.parts.at(0).measures.at(0).staves.at(0).voices.at(0).label = "1";
    score.parts.at(0).measures.at(0).staves.at(0).voices.at(1).label = "2";

    const auto out = roundTrip(score);
    const auto &voices = out.parts.at(0).measures.at(0).staves.at(0).voices;
    REQUIRE(voices.size() == 2);
    CHECK_EQUAL("", voices.at(0).label);
    CHECK_EQUAL("", voices.at(1).label);
}

#endif
