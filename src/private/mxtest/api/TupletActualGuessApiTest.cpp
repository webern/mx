// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// When a <tuplet> start leaves out <tuplet-actual>, mx fills it in from the note's
// <time-modification>. The schema counts both actual-notes and normal-notes in the normal-type, so
// the guessed actual notes always have the same note value as the normal notes, even when the note
// that opens the tuplet is written longer or shorter (#485). The XML round-trip cannot catch a
// wrong guess here, because the writer leaves out a guess that the time-modification already
// implies, so these tests look at the TupletStart values directly. The note's duration is checked
// against the guess, and a duration that only fits another reading is reported.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/TestHelpers.h"

#include <sstream>
#include <string>

using namespace mx::api;
using namespace mxtest;

namespace tupletActualGuessApiTest
{
inline std::string scoreXml(int inDivisions, const std::string &inNotes)
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>Music</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>)" +
           std::to_string(inDivisions) + R"(</divisions>
      </attributes>
)" + inNotes +
           R"(    </measure>
  </part>
</score-partwise>
)";
}

// A pitched note with a time modification. An empty duration makes it a grace note, and an empty
// normal type leaves the normal-type out of the time modification.
inline std::string noteXml(const std::string &inType, int inDots, const std::string &inDuration, int inActualNotes,
                           int inNormalNotes, const std::string &inNormalType, int inNormalDots,
                           const std::string &inNotations)
{
    std::string xml = "      <note>\n";
    if (inDuration.empty())
    {
        xml += "        <grace/>\n";
    }
    xml += "        <pitch><step>C</step><octave>4</octave></pitch>\n";
    if (!inDuration.empty())
    {
        xml += "        <duration>" + inDuration + "</duration>\n";
    }
    xml += "        <voice>1</voice>\n";
    xml += "        <type>" + inType + "</type>\n";
    for (int i = 0; i < inDots; ++i)
    {
        xml += "        <dot/>\n";
    }
    xml += "        <time-modification>\n";
    xml += "          <actual-notes>" + std::to_string(inActualNotes) + "</actual-notes>\n";
    xml += "          <normal-notes>" + std::to_string(inNormalNotes) + "</normal-notes>\n";
    if (!inNormalType.empty())
    {
        xml += "          <normal-type>" + inNormalType + "</normal-type>\n";
        for (int i = 0; i < inNormalDots; ++i)
        {
            xml += "          <normal-dot/>\n";
        }
    }
    xml += "        </time-modification>\n";
    xml += "        <notations>" + inNotations + "</notations>\n";
    xml += "      </note>\n";
    return xml;
}

inline const std::string &startTuplet()
{
    static const std::string value = R"(<tuplet type="start" number="1"/>)";
    return value;
}

// The eighth-note triplet from issue #485, opened by a quarter rest.
inline std::string issueXml()
{
    return scoreXml(120, R"(      <note>
        <rest/>
        <duration>80</duration>
        <voice>1</voice>
        <type>quarter</type>
        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
          <normal-type>eighth</normal-type>
        </time-modification>
        <notations><tuplet type="start" number="1" bracket="yes"/></notations>
      </note>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>40</duration>
        <voice>1</voice>
        <type>eighth</type>
        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
          <normal-type>eighth</normal-type>
        </time-modification>
        <notations><tuplet type="stop" number="1"/></notations>
      </note>
)");
}

// An eighth inside a quarter-note triplet, the shape of lysuite ly23d_Tuplets_Nested.
inline std::string eighthInQuarterTripletXml(const std::string &inDuration, const std::string &inNotations)
{
    return scoreXml(30, noteXml("eighth", 0, inDuration, 3, 2, "quarter", 0, inNotations));
}

inline NoteData &firstNote(ScoreData &ioScore)
{
    return ioScore.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0);
}

inline TupletStart firstStart(const ScoreData &inScore)
{
    const auto &starts =
        inScore.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData.tupletStarts;
    REQUIRE(1 == starts.size());
    return starts.at(0);
}

inline void checkStart(const TupletStart &inStart, int inActualNumber, DurationName inActualName, int inActualDots,
                       int inNormalNumber, DurationName inNormalName, int inNormalDots)
{
    CHECK_EQUAL(inActualNumber, inStart.actualNumber);
    CHECK(inActualName == inStart.actualDurationName);
    CHECK_EQUAL(inActualDots, inStart.actualDots);
    CHECK_EQUAL(inNormalNumber, inStart.normalNumber);
    CHECK(inNormalName == inStart.normalDurationName);
    CHECK_EQUAL(inNormalDots, inStart.normalDots);
}

// The text of the written <tuplet-actual>, or an empty string when there is none.
inline std::string writtenActual(const std::string &inXml)
{
    const auto begin = inXml.find("<tuplet-actual>");
    if (begin == std::string::npos)
    {
        return {};
    }
    const auto end = inXml.find("</tuplet-actual>", begin);
    REQUIRE(end != std::string::npos);
    return inXml.substr(begin, end - begin);
}

// The specConflict diagnostics from reading the XML. The score read without a collection must be
// the same, so the check never changes what the reader produces.
inline std::vector<Diagnostic> readConflicts(const std::string &inXml)
{
    std::istringstream stream{inXml};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());
    Diagnostics diagnostics;
    const auto collected = getScore(document.value(), diagnostics);
    REQUIRE(collected.ok());
    const auto silent = getScore(document.value());
    REQUIRE(silent.ok());
    CHECK(silent.value() == collected.value());

    std::vector<Diagnostic> conflicts;
    for (const auto &diagnostic : diagnostics.all())
    {
        if (diagnostic.code == DiagnosticCode::specConflict)
        {
            conflicts.push_back(diagnostic);
        }
    }
    return conflicts;
}
} // namespace tupletActualGuessApiTest

// The quarter rest is 3 eighths in the time of 2 eighths; its 80 of 120 divisions are two thirds
// of a quarter.
TEST(quarterOpeningEighthTriplet, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto score = fromXml(issueXml());
    checkStart(firstStart(score), 3, DurationName::eighth, 0, 2, DurationName::eighth, 0);
}

T_END

TEST(quarterOpeningSixteenthQuintuplet, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto score = fromXml(scoreXml(20, noteXml("quarter", 0, "16", 5, 4, "16th", 0, startTuplet())));
    checkStart(firstStart(score), 5, DurationName::dur16th, 0, 4, DurationName::dur16th, 0);
}

T_END

// An eighth lasting a third of a quarter is an eighth inside a quarter-note triplet.
TEST(eighthInsideQuarterTriplet, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto score = fromXml(eighthInQuarterTripletXml("10", startTuplet()));
    checkStart(firstStart(score), 3, DurationName::quarter, 0, 2, DurationName::quarter, 0);
}

T_END

// A dotted quarter is three eighths, so in an eighth triplet it lasts exactly one quarter.
TEST(dottedQuarterOpeningEighthTriplet, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto score = fromXml(scoreXml(120, noteXml("quarter", 1, "120", 3, 2, "eighth", 0, startTuplet())));
    checkStart(firstStart(score), 3, DurationName::eighth, 0, 2, DurationName::eighth, 0);
}

T_END

TEST(dottedEighthTripletKeepsTheNormalDot, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto score = fromXml(scoreXml(120, noteXml("eighth", 1, "60", 3, 2, "eighth", 1, startTuplet())));
    checkStart(firstStart(score), 3, DurationName::eighth, 1, 2, DurationName::eighth, 1);
}

T_END

// Without a normal-type, the ratio counts in the note's own written value, dots included.
TEST(noNormalTypeCountsInTheNoteValue, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto plain = fromXml(scoreXml(120, noteXml("eighth", 0, "40", 3, 2, "", 0, startTuplet())));
    checkStart(firstStart(plain), 3, DurationName::eighth, 0, 2, DurationName::eighth, 0);

    const auto dotted = fromXml(scoreXml(120, noteXml("eighth", 1, "60", 3, 2, "", 0, startTuplet())));
    checkStart(firstStart(dotted), 3, DurationName::eighth, 1, 2, DurationName::eighth, 1);
}

T_END

// A guess that repeats the time modification is not written, and asking for it writes the guessed
// note value, not the note's own.
TEST(guessedActualIsNotWrittenUnlessAskedFor, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;

    auto issue = fromXml(issueXml());
    CHECK(writtenActual(toXml(issue)).empty());
    firstNote(issue).noteAttachmentData.tupletStarts.at(0).writeActual = Bool::yes;
    const auto issueActual = writtenActual(toXml(issue));
    CHECK(issueActual.find("<tuplet-number>3</tuplet-number>") != std::string::npos);
    CHECK(issueActual.find("<tuplet-type>eighth</tuplet-type>") != std::string::npos);

    auto nested = fromXml(eighthInQuarterTripletXml("10", startTuplet()));
    CHECK(writtenActual(toXml(nested)).empty());
    firstNote(nested).noteAttachmentData.tupletStarts.at(0).writeActual = Bool::yes;
    const auto nestedActual = writtenActual(toXml(nested));
    CHECK(nestedActual.find("<tuplet-number>3</tuplet-number>") != std::string::npos);
    CHECK(nestedActual.find("<tuplet-type>quarter</tuplet-type>") != std::string::npos);
}

T_END

TEST(durationThatFitsTheGuessIsNotReported, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    CHECK(readConflicts(issueXml()).empty());
    CHECK(readConflicts(eighthInQuarterTripletXml("10", startTuplet())).empty());
    CHECK(readConflicts(scoreXml(120, noteXml("quarter", 1, "120", 3, 2, "eighth", 0, startTuplet()))).empty());
}

T_END

// 20 of 30 divisions is what the eighth would last as 3 eighths in the time of 2 quarters.
TEST(durationThatCountsInTheWrittenTypeIsReported, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto conflicts = readConflicts(eighthInQuarterTripletXml("20", startTuplet()));
    REQUIRE(1 == conflicts.size());
    CHECK(Severity::warning == conflicts.at(0).severity);
    CHECK_EQUAL(0, conflicts.at(0).location.partIndex);
    CHECK_EQUAL(0, conflicts.at(0).location.measureIndex);
    CHECK(conflicts.at(0).message.find("mx guessed 3 quarter notes,") != std::string::npos);
    CHECK(conflicts.at(0).message.find("duration is 20 divisions, but that reading expects 10") != std::string::npos);
    CHECK(conflicts.at(0).message.find("note's own written type (eighth) instead") != std::string::npos);

    // the guess itself does not change
    const auto score = fromXml(eighthInQuarterTripletXml("20", startTuplet()));
    checkStart(firstStart(score), 3, DurationName::quarter, 0, 2, DurationName::quarter, 0);
}

T_END

TEST(durationThatFitsNeitherReadingIsReported, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto conflicts = readConflicts(eighthInQuarterTripletXml("17", startTuplet()));
    REQUIRE(1 == conflicts.size());
    CHECK(Severity::warning == conflicts.at(0).severity);
    CHECK(conflicts.at(0).message.find("duration is 17 divisions, but that reading expects 10") != std::string::npos);
    CHECK(conflicts.at(0).message.find("would expect 20; the duration fits neither reading") != std::string::npos);
}

T_END

// Without a normal-type the note is written in the note value the ratio counts in, so there is only
// one reading of its duration to report against.
TEST(durationThatDoesNotFitTheTimeModificationIsReported, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto conflicts = readConflicts(scoreXml(30, noteXml("eighth", 0, "17", 3, 2, "", 0, startTuplet())));
    REQUIRE(1 == conflicts.size());
    CHECK(Severity::warning == conflicts.at(0).severity);
    CHECK(conflicts.at(0).message.find("mx guessed 3 eighth notes,") != std::string::npos);
    CHECK(conflicts.at(0).message.find("does not fit the time-modification") != std::string::npos);
    CHECK(conflicts.at(0).message.find("written type") == std::string::npos);
}

T_END

// The guessed note value names its dots.
TEST(reportNamesDottedNoteValues, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto oneDot = readConflicts(scoreXml(120, noteXml("quarter", 0, "17", 3, 2, "quarter", 1, startTuplet())));
    REQUIRE(1 == oneDot.size());
    CHECK(oneDot.at(0).message.find("mx guessed 3 dotted quarter notes,") != std::string::npos);

    const auto twoDots = readConflicts(scoreXml(120, noteXml("quarter", 0, "17", 3, 2, "quarter", 2, startTuplet())));
    REQUIRE(1 == twoDots.size());
    CHECK(twoDots.at(0).message.find("mx guessed 3 quarter notes with 2 dots,") != std::string::npos);
}

T_END

TEST(graceNoteIsNotChecked, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const auto grace = noteXml("eighth", 0, "", 3, 2, "quarter", 0, startTuplet());
    const auto main = noteXml("eighth", 0, "10", 3, 2, "quarter", 0, "");
    CHECK(readConflicts(scoreXml(30, grace + main)).empty());
}

T_END

// A stated tuplet-actual is the author's own word, so the duration is not checked against it.
TEST(statedActualIsNotChecked, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    const std::string stated = R"(<tuplet type="start" number="1"><tuplet-actual><tuplet-number>3</tuplet-number>)"
                               R"(<tuplet-type>eighth</tuplet-type></tuplet-actual></tuplet>)";
    CHECK(readConflicts(eighthInQuarterTripletXml("17", stated)).empty());
}

T_END

// Divisions too coarse for a triplet force each duration to round to a whole division.
TEST(roundedDurationIsNotReported, TupletActualGuess)
{
    using namespace tupletActualGuessApiTest;
    CHECK(readConflicts(scoreXml(1, noteXml("eighth", 0, "1", 3, 2, "", 0, startTuplet()))).empty());
    CHECK(readConflicts(scoreXml(2, noteXml("eighth", 0, "1", 3, 2, "", 0, startTuplet()))).empty());
    CHECK(readConflicts(scoreXml(2, noteXml("quarter", 0, "1", 3, 2, "", 0, startTuplet()))).empty());
}

T_END

#endif
