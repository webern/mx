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
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace voiceInterleaveApiTest
{
// A one-part score in 3/4 with one division per quarter, holding a single measure whose music is
// inMeasureBody. With two staves, the second has a bass clef.
inline std::string makeScoreXml(const std::string &inMeasureBody, int inStaves = 1)
{
    const std::string staves = inStaves == 1 ? "" : "        <staves>2</staves>\n";
    const std::string clefNumber = inStaves == 1 ? "" : R"( number="1")";
    const std::string bassClef = inStaves == 1 ? "" : R"(        <clef number="2">
          <sign>F</sign>
          <line>4</line>
        </clef>
)";
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
)"} + staves +
           "        <clef" + clefNumber + R"(>
          <sign>G</sign>
          <line>2</line>
        </clef>
)" + bassClef +
           R"(      </attributes>
)" + inMeasureBody +
           R"(    </measure>
  </part>
</score-partwise>
)";
}

// A <note> in octave 4. An empty inVoice leaves the <voice> element out, and so does a zero
// inStaff for the <staff> element.
inline std::string makeNoteXml(const std::string &inStep, int inDuration, const std::string &inVoice,
                               bool inIsChord = false, int inStaff = 0)
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
    if (inStaff > 0)
    {
        xml += "        <staff>" + std::to_string(inStaff) + "</staff>\n";
    }
    xml += "      </note>\n";
    return xml;
}

inline std::string makeBackupXml(int inDuration)
{
    return "      <backup>\n        <duration>" + std::to_string(inDuration) + "</duration>\n      </backup>\n";
}

inline std::string makeForwardXml(int inDuration)
{
    return "      <forward>\n        <duration>" + std::to_string(inDuration) + "</duration>\n      </forward>\n";
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

// The number and type of every <slur>, in document order, as "1 start", "2 stop" and so on.
inline std::vector<std::string> findSlurs(const std::string &inXml)
{
    std::vector<std::string> slurs;
    for (auto pos = inXml.find("<slur "); pos != std::string::npos; pos = inXml.find("<slur ", pos + 1))
    {
        const auto end = inXml.find('>', pos);
        const auto element = inXml.substr(pos, end - pos);
        const auto attribute = [&](const std::string &inName) {
            const auto start = element.find(inName + "=\"") + inName.size() + 2;
            return element.substr(start, element.find('"', start) - start);
        };
        slurs.push_back(attribute("number") + " " + attribute("type"));
    }
    return slurs;
}

inline void addSlur(NoteData &inFrom, NoteData &inTo, const std::string &inIdentity)
{
    CurveStart start{CurveType::slur};
    start.number = SpannerNumber{inIdentity};
    inFrom.noteAttachmentData.curveStarts.push_back(start);
    CurveStop stop{CurveType::slur};
    stop.number = SpannerNumber{inIdentity};
    inTo.noteAttachmentData.curveStops.push_back(stop);
}

// What a note's voice must keep through a write and a read: where it is and what it is.
inline std::string describeVoices(const std::map<int, VoiceData> &inVoices)
{
    std::string text;
    for (const auto &voice : inVoices)
    {
        text += "voice " + std::to_string(voice.first) + " '" + voice.second.label + "':";
        for (const auto &note : voice.second.notes)
        {
            text += " " + std::to_string(note.tickTimePosition) + "/" +
                    std::to_string(static_cast<int>(note.pitchData.step));
            text += note.isChord ? "c" : "";
            text += note.isGrace ? "g" : "";
        }
        text += "\n";
    }
    return text;
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

// A voice change after a <forward> is another voice too. The second voice starts after the first
// ends, so it is written after it as before, with the <forward> between them and no <backup>.
TEST(aVoiceChangeAfterAForwardStartsAnotherVoice, VoiceInterleave)
{
    const auto xml = makeScoreXml(makeNoteXml("C", 1, "1") + makeForwardXml(1) + makeNoteXml("E", 1, "2"));
    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &voices = voicesOf(score);
    REQUIRE(voices.size() == 2);
    REQUIRE(voices.count(0) == 1);
    REQUIRE(voices.count(1) == 1);
    REQUIRE(voices.at(0).notes.size() == 1);
    CHECK_EQUAL(0, voices.at(0).notes.at(0).tickTimePosition);
    REQUIRE(voices.at(1).notes.size() == 1);
    CHECK_EQUAL(2 * score.ticksPerQuarter, voices.at(1).notes.at(0).tickTimePosition);

    const auto written = toXml(score);
    CHECK_EQUAL(0, countOccurrences(written, "<backup>"));
    CHECK_EQUAL(1, countOccurrences(written, "<forward>"));
    const std::vector<std::string> expectedTexts{"1", "2"};
    CHECK(expectedTexts == findVoiceTexts(written));
}

T_END;

// Voices are told apart by name within each staff: the names on each staff of a piano part split
// that staff into voices as they would on a single staff.
TEST(voiceNamesAreGroupedPerStaff, VoiceInterleave)
{
    const auto xml = makeScoreXml(makeNoteXml("F", 1, "1", false, 1) + makeNoteXml("F", 1, "2", false, 1) +
                                      makeNoteXml("F", 1, "1", false, 1) + makeBackupXml(3) +
                                      makeNoteXml("C", 1, "5", false, 2) + makeNoteXml("D", 2, "6", false, 2),
                                  2);
    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &staves = score.parts.at(0).measures.at(0).staves;
    REQUIRE(staves.size() == 2);
    REQUIRE(staves.at(0).voices.size() == 2);
    CHECK_EQUAL(2, staves.at(0).voices.cbegin()->second.notes.size());
    CHECK_EQUAL(1, std::next(staves.at(0).voices.cbegin())->second.notes.size());
    REQUIRE(staves.at(1).voices.size() == 2);

    const auto written = toXml(score);
    CHECK_EQUAL(1, countOccurrences(written, "<backup>"));
    CHECK_EQUAL(0, countOccurrences(written, "<forward>"));
    const std::vector<std::string> expectedTexts{"1", "2", "1", "5", "6"};
    CHECK(expectedTexts == findVoiceTexts(written));
    CHECK(score.parts == fromXml(written).parts);
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

// Slurs that an author names by identity are numbered in the order the notes are written. With the
// second voice written in the first voice's gap, its slur opens while the first voice's slur is
// still open, so the two need different numbers.
TEST(slursInAGapFillAreNumberedInWrittenOrder, VoiceInterleave)
{
    auto score = makeScore();
    auto &voices = voicesOf(score);
    voices[0].notes = {makeQuarter(Step::c, 0), makeQuarter(Step::f, 3)};
    voices[1].notes = {makeQuarter(Step::d, 1), makeQuarter(Step::e, 2)};
    addSlur(voices[0].notes.at(0), voices[0].notes.at(1), "outer");
    addSlur(voices[1].notes.at(0), voices[1].notes.at(1), "inner");
    const auto xml = toXml(score);
    REQUIRE(!xml.empty());
    CHECK_EQUAL(0, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(std::string{"CDEF"}, findSteps(xml));
    const std::vector<std::string> expectedSlurs{"1 start", "2 start", "2 stop", "1 stop"};
    CHECK(expectedSlurs == findSlurs(xml));
}

T_END;

// On a staff where two voices have the same name, no voice is written in another's gap: a reader
// puts notes with one name together when no <backup> lies between them, and here the voice before
// the gap ends exactly where the next pass would begin.
TEST(aGapFillOnAStaffWithARepeatedNameIsNotInlined, VoiceInterleave)
{
    auto score = makeScore();
    auto &voices = voicesOf(score);
    voices[0].notes = {makeQuarter(Step::c, 0)};
    voices[1].notes = {makeQuarter(Step::d, 1), makeQuarter(Step::f, 3)};
    voices[2].notes = {makeQuarter(Step::e, 2)};
    voices[2].label = "1";
    const auto xml = toXml(score);
    CHECK_EQUAL(1, countOccurrences(xml, "<backup>"));

    const auto out = roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(3, voicesOf(out).size());
}

T_END;

// Two voices can fill the gaps of a third, and the voice after them still backs up from the end of
// the stream they make. The next staff starts with its own <backup>, as before.
TEST(aMergedPassIsFollowedByABackup, VoiceInterleave)
{
    auto score = makeScore();
    score.parts.at(0).measures.at(0).staves.emplace_back();
    auto &voices = voicesOf(score);
    voices[0].notes = {makeQuarter(Step::c, 0), makeQuarter(Step::f, 3)};
    voices[1].notes = {makeQuarter(Step::d, 1)};
    voices[2].notes = {makeQuarter(Step::e, 2)};
    voices[3].notes = {makeNote(Step::g, 0, 4, DurationName::whole)};
    auto &lowerStaff = score.parts.at(0).measures.at(0).staves.at(1).voices;
    lowerStaff[0].notes = {makeNote(Step::a, 0, 4, DurationName::whole)};

    const auto xml = toXml(score);
    CHECK_EQUAL(2, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(0, countOccurrences(xml, "<forward>"));
    CHECK_EQUAL(std::string{"CDEFGA"}, findSteps(xml));
    const std::vector<std::string> expectedTexts{"1", "2", "3", "1", "4"};
    CHECK(expectedTexts == findVoiceTexts(xml));

    const auto out = roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(describeVoices(voicesOf(score)), describeVoices(voicesOf(out)));
    CHECK_EQUAL(describeVoices(lowerStaff), describeVoices(out.parts.at(0).measures.at(0).staves.at(1).voices));
}

T_END;

// A chord fills a gap as one unit, and a clef and a direction in the gap are written at the tick
// where they belong.
TEST(aChordClefAndDirectionInAGapFill, VoiceInterleave)
{
    auto score = makeGapFillScore();
    auto &voices = voicesOf(score);
    auto chordNote = makeQuarter(Step::a, 1);
    chordNote.isChord = true;
    voices.at(4).notes.at(0).isChord = true;
    voices.at(4).notes.push_back(chordNote);

    auto &staff = score.parts.at(0).measures.at(0).staves.at(0);
    ClefData clef;
    clef.setBass();
    clef.tickTimePosition = 1;
    clef.location = ClefLocation::midMeasure;
    staff.clefs.push_back(clef);
    DirectionData direction;
    direction.tickTimePosition = 1;
    direction.directionTypes.emplace_back(DirectionChoice{DampData{}});
    staff.directions.push_back(direction);

    const auto xml = toXml(score);
    CHECK_EQUAL(0, countOccurrences(xml, "<backup>"));
    CHECK_EQUAL(0, countOccurrences(xml, "<forward>"));
    CHECK_EQUAL(std::string{"FFAF"}, findSteps(xml));

    const auto out = roundTrip(score);
    REQUIRE(out.parts.size() == 1);
    CHECK_EQUAL(describeVoices(voicesOf(score)), describeVoices(voicesOf(out)));
    const auto &outStaff = out.parts.at(0).measures.at(0).staves.at(0);
    REQUIRE(outStaff.directions.size() == 1);
    CHECK_EQUAL(1, outStaff.directions.at(0).tickTimePosition);
    CHECK(!outStaff.directions.at(0).offset.has_value());
    REQUIRE(outStaff.clefs.size() == 1);
    CHECK_EQUAL(1, outStaff.clefs.at(0).tickTimePosition);
}

T_END;

// Whether or not the writer puts voices in one another's gaps, the voices read back as they were
// written, and slurs named by identity never share a number while both are open. Many small staves
// of one to three voices with notes, chords and grace notes on random beats.
TEST(gapFilledVoicesReadBackAsWritten, VoiceInterleave)
{
    std::mt19937 random{425};
    const auto roll = [&](unsigned inSides) { return static_cast<int>(random() % inSides); };
    const std::vector<Step> steps{Step::c, Step::d, Step::e, Step::f, Step::g, Step::a, Step::b};
    int mergedCount = 0;

    for (int i = 0; i < 400; ++i)
    {
        auto score = makeScore();
        auto &voices = voicesOf(score);
        const int voiceCount = 1 + roll(3);

        for (int v = 0; v < voiceCount; ++v)
        {
            auto &notes = voices[v].notes;
            for (int tick = 0; tick < 4; ++tick)
            {
                if (roll(2) == 0)
                {
                    continue;
                }
                if (roll(6) == 0)
                {
                    auto grace = makeNote(steps.at(static_cast<size_t>(roll(7))), tick, 0, DurationName::eighth);
                    grace.isGrace = true;
                    notes.push_back(grace);
                }
                notes.push_back(makeQuarter(steps.at(static_cast<size_t>(roll(7))), tick));
                if (roll(5) == 0)
                {
                    notes.back().isChord = true;
                    notes.push_back(notes.back());
                    notes.back().pitchData.octave = 5;
                }
            }
            if (notes.empty())
            {
                notes.push_back(makeQuarter(Step::c, roll(4)));
            }
            if (notes.size() > 1)
            {
                addSlur(notes.front(), notes.back(), "voice" + std::to_string(v));
            }
        }

        const auto xml = toXml(score);
        REQUIRE(!xml.empty());
        if (voiceCount > 1 && countOccurrences(xml, "<backup>") < static_cast<size_t>(voiceCount - 1))
        {
            ++mergedCount;
        }

        std::map<std::string, bool> isOpen;
        for (const auto &slur : findSlurs(xml))
        {
            const auto number = slur.substr(0, slur.find(' '));
            const bool isStart = slur.find("start") != std::string::npos;
            CHECK(isOpen[number] != isStart);
            isOpen[number] = isStart;
        }

        const auto out = roundTrip(score);
        REQUIRE(out.parts.size() == 1);
        CHECK_EQUAL(describeVoices(voices), describeVoices(voicesOf(out)));
    }

    // the cases above include many where voices share a stream
    CHECK(mergedCount > 20);
}

T_END;

#endif
