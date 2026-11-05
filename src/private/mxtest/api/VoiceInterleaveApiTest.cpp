// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace voiceInterleaveApiTest
{
// A one-part, one-staff score in 3/4 with one division per quarter, holding a single measure
// whose music is inMeasureBody.
inline std::string makeScoreXml(const std::string &inMeasureBody)
{
    return std::string{R"(<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>Flute</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <time>
          <beats>3</beats>
          <beat-type>4</beat-type>
        </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
)"} + inMeasureBody +
           R"(    </measure>
  </part>
</score-partwise>
)";
}

// A <note> in octave 4. An empty inVoice leaves the <voice> element out.
inline std::string makeNoteXml(const std::string &inStep, int inDuration, const std::string &inVoice,
                               bool inIsChord = false)
{
    std::string xml = "      <note>\n";
    if (inIsChord)
    {
        xml += "        <chord/>\n";
    }
    xml += "        <pitch>\n          <step>" + inStep + "</step>\n          <octave>4</octave>\n        </pitch>\n";
    xml += "        <duration>" + std::to_string(inDuration) + "</duration>\n";
    if (!inVoice.empty())
    {
        xml += "        <voice>" + inVoice + "</voice>\n";
    }
    xml += inDuration == 3 ? "        <type>half</type>\n        <dot/>\n" : "        <type>quarter</type>\n";
    xml += "      </note>\n";
    return xml;
}

inline std::string makeBackupXml(int inDuration)
{
    return "      <backup>\n        <duration>" + std::to_string(inDuration) + "</duration>\n      </backup>\n";
}

inline size_t countOccurrences(const std::string &haystack, const std::string &needle)
{
    size_t count = 0;
    for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1))
    {
        ++count;
    }
    return count;
}

// The text of every <voice> element, in document order.
inline std::vector<std::string> findVoiceTexts(const std::string &inXml)
{
    const std::string open = "<voice>";
    const std::string close = "</voice>";
    std::vector<std::string> texts;
    for (auto pos = inXml.find(open); pos != std::string::npos; pos = inXml.find(open, pos + 1))
    {
        const auto start = pos + open.size();
        texts.push_back(inXml.substr(start, inXml.find(close, start) - start));
    }
    return texts;
}

// The pitch steps of every note, in document order.
inline std::string findSteps(const std::string &inXml)
{
    const std::string open = "<step>";
    std::string steps;
    for (auto pos = inXml.find(open); pos != std::string::npos; pos = inXml.find(open, pos + 1))
    {
        steps += inXml.at(pos + open.size());
    }
    return steps;
}

inline NoteData makeNote(Step inStep, int inTick, int inTicks, DurationName inDurationName)
{
    NoteData note;
    note.pitchData.step = inStep;
    note.pitchData.octave = 4;
    note.tickTimePosition = inTick;
    note.durationData.durationName = inDurationName;
    note.durationData.durationTimeTicks = inTicks;
    return note;
}

inline NoteData makeQuarter(Step inStep, int inTick)
{
    return makeNote(inStep, inTick, 1, DurationName::quarter);
}

// One staff at one tick per quarter; the caller fills in the voices.
inline ScoreData makeScore()
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    part.measures.back().staves.emplace_back();
    return score;
}

inline std::map<int, VoiceData> &voicesOf(ScoreData &score)
{
    return score.parts.at(0).measures.at(0).staves.at(0).voices;
}

inline const std::map<int, VoiceData> &voicesOf(const ScoreData &score)
{
    return score.parts.at(0).measures.at(0).staves.at(0).voices;
}

// Three F quarters in voices 4, 5 and 4: voice 4 has the first and third beats, voice 5 the second.
inline ScoreData makeGapFillScore()
{
    auto score = makeScore();
    auto &voices = voicesOf(score);
    voices[3].notes = {makeQuarter(Step::f, 0), makeQuarter(Step::f, 2)};
    voices[4].notes = {makeQuarter(Step::f, 1)};
    return score;
}
} // namespace voiceInterleaveApiTest

using namespace voiceInterleaveApiTest;

// Three quarters in a row, labelled voice 4, 5 and 4, with no <backup> between them. The middle note
// belongs to voice 5, so voice 4 has a rest-like gap on beat two that voice 5 fills.
TEST(aVoiceChangeWithoutABackupStartsAnotherVoice, VoiceInterleave)
{
    const auto xml = makeScoreXml(makeNoteXml("F", 1, "4") + makeNoteXml("F", 1, "5") + makeNoteXml("F", 1, "4"));
    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const int quarter = score.ticksPerQuarter;
    const auto &voices = voicesOf(score);
    REQUIRE(voices.size() == 2);
    REQUIRE(voices.count(3) == 1);
    REQUIRE(voices.count(4) == 1);

    const auto &voiceFour = voices.at(3).notes;
    REQUIRE(voiceFour.size() == 2);
    CHECK_EQUAL(0, voiceFour.at(0).tickTimePosition);
    CHECK_EQUAL(2 * quarter, voiceFour.at(1).tickTimePosition);

    const auto &voiceFive = voices.at(4).notes;
    REQUIRE(voiceFive.size() == 1);
    CHECK_EQUAL(quarter, voiceFive.at(0).tickTimePosition);
}

T_END;

// The writer puts voice 5's note in the gap it fills, so the three notes follow one another with no
// <forward> or <backup>, and each carries the name of its own voice.
TEST(aVoiceThatFillsAGapIsWrittenInline, VoiceInterleave)
{
    const auto score = makeGapFillScore();
    const auto xml = toXml(score);
    CHECK_EQUAL(0, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(0, countOccurrences(xml, "<forward>"));
    const std::vector<std::string> expectedTexts{"4", "5", "4"};
    CHECK(expectedTexts == findVoiceTexts(xml));

    const auto out = roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    const auto &voices = voicesOf(out);
    REQUIRE(voices.size() == 2);
    REQUIRE(voices.count(3) == 1);
    REQUIRE(voices.count(4) == 1);
    REQUIRE(voices.at(3).notes.size() == 2);
    CHECK_EQUAL(0, voices.at(3).notes.at(0).tickTimePosition);
    CHECK_EQUAL(2, voices.at(3).notes.at(1).tickTimePosition);
    REQUIRE(voices.at(4).notes.size() == 1);
    CHECK_EQUAL(1, voices.at(4).notes.at(0).tickTimePosition);
}

T_END;

// A <chord/> note sounds with the note before it, so it stays in that note's voice even when its own
// <voice> names another one.
TEST(aChordMemberStaysWithItsChord, VoiceInterleave)
{
    const auto xml = makeScoreXml(makeNoteXml("C", 1, "1") + makeNoteXml("E", 1, "2", true) + makeNoteXml("G", 1, "1"));
    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &voices = voicesOf(score);
    REQUIRE(voices.size() == 1);
    const auto &notes = voices.cbegin()->second.notes;
    REQUIRE(notes.size() == 3);
    CHECK(notes.at(0).isChord);
    CHECK(notes.at(1).isChord);
    CHECK(!notes.at(2).isChord);
}

T_END;

// A note that does not say which voice it is in continues the voice of the note before it.
TEST(aNoteWithoutAVoiceStaysWithThePreviousNote, VoiceInterleave)
{
    {
        const auto xml = makeScoreXml(makeNoteXml("C", 1, "1") + makeNoteXml("D", 1, "") + makeNoteXml("E", 1, "1"));
        const auto score = fromXml(xml);
        REQUIRE(score.parts.size() == 1);
        const auto &voices = voicesOf(score);
        REQUIRE(voices.size() == 1);
        CHECK_EQUAL(3, voices.cbegin()->second.notes.size());
    }
    {
        const auto xml = makeScoreXml(makeNoteXml("C", 1, "1") + makeNoteXml("D", 1, "2") + makeNoteXml("E", 1, ""));
        const auto score = fromXml(xml);
        REQUIRE(score.parts.size() == 1);
        const auto &voices = voicesOf(score);
        REQUIRE(voices.size() == 2);
        const auto &first = voices.cbegin()->second.notes;
        const auto &second = std::next(voices.cbegin())->second.notes;
        REQUIRE(first.size() == 1);
        CHECK(Step::c == first.at(0).pitchData.step);
        REQUIRE(second.size() == 2);
        CHECK(Step::d == second.at(0).pitchData.step);
        CHECK(Step::e == second.at(1).pitchData.step);
    }
}

T_END;

// The same voice name on both sides of a <backup> is two voices: a backup always starts another one.
TEST(aVoiceRepeatedAfterABackupStaysSeparate, VoiceInterleave)
{
    const auto xml = makeScoreXml(makeNoteXml("C", 3, "1") + makeBackupXml(3) + makeNoteXml("E", 3, "1"));
    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &voices = voicesOf(score);
    REQUIRE(voices.size() == 2);
    REQUIRE(voices.cbegin()->second.notes.size() == 1);
    CHECK(Step::c == voices.cbegin()->second.notes.at(0).pitchData.step);
    REQUIRE(std::next(voices.cbegin())->second.notes.size() == 1);
    CHECK(Step::e == std::next(voices.cbegin())->second.notes.at(0).pitchData.step);
}

T_END;

// Voices that sound at the same time are written one after the other, with a <backup> between them.
TEST(completeVoicesAreWrittenOneAfterAnother, VoiceInterleave)
{
    auto score = makeScore();
    auto &voices = voicesOf(score);
    voices[0].notes = {makeQuarter(Step::c, 0), makeQuarter(Step::d, 1), makeQuarter(Step::e, 2)};
    voices[1].notes = {makeNote(Step::g, 0, 2, DurationName::half), makeQuarter(Step::a, 2)};
    const auto xml = toXml(score);
    CHECK_EQUAL(1, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(0, countOccurrences(xml, "<forward>"));
    CHECK_EQUAL(std::string{"CDEGA"}, findSteps(xml));
}

T_END;

// When the gap and the voice that fills it have the same name, writing them in one stream would
// read back as a single voice, so the second voice keeps its own place after a <backup>.
TEST(aGapFillWithTheSameVoiceTextIsNotInlined, VoiceInterleave)
{
    auto score = makeGapFillScore();
    auto &voices = voicesOf(score);
    voices.at(3).label = "x";
    voices.at(4).label = "x";
    const auto xml = toXml(score);
    CHECK_EQUAL(1, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(1, countOccurrences(xml, "<forward>"));

    const auto out = roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(2, voicesOf(out).size());
}

T_END;

// A grace note at the end of the filling voice would share a tick with the next note of the voice
// it fills, and the order of the two would be ambiguous in one stream, so the voices are written
// separately.
TEST(aGraceNoteOnAVoiceBoundaryIsNotInlined, VoiceInterleave)
{
    auto score = makeGapFillScore();
    auto grace = makeNote(Step::g, 2, 0, DurationName::eighth);
    grace.isGrace = true;
    voicesOf(score).at(4).notes.push_back(grace);
    const auto xml = toXml(score);
    CHECK_EQUAL(1, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(1, countOccurrences(xml, "<forward>"));
    CHECK_EQUAL(std::string{"FFFG"}, findSteps(xml));
}

T_END;

#endif
