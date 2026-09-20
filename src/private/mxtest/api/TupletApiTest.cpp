// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// A <tuplet> may spell its ratio out in <tuplet-actual> and <tuplet-normal>, or leave both out
// and let the note's <time-modification> speak for it. mx reads either shape into the same
// fields and remembers which one the file used, so the ratio is stated again only where it says
// something the note does not already say -- which is what an inner tuplet of a nested pair
// needs. The coverage here is the reading, the writing, and the three <tuplet> attributes that
// had no home in mx::api before: show-type, line-shape, and bracket on a stop.

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace mx::api;
using namespace mxtest;

namespace tupletApiTest
{
// One quarter note carrying whatever time modification and notations a test needs.
inline std::string scoreXml(const std::string &inTimeModification, const std::string &inNotations)
{
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>MusicXML Part</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>6</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <voice>1</voice>
        <type>quarter</type>
)" + inTimeModification +
           R"(        <notations>
)" + inNotations +
           R"(
        </notations>
      </note>
    </measure>
  </part>
</score-partwise>
)";
}

inline const std::string &tripletTimeModification()
{
    static const std::string value = R"(        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
        </time-modification>
)";
    return value;
}

inline const NoteAttachmentData &attachments(const ScoreData &inScore)
{
    return inScore.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0).noteAttachmentData;
}

// A score of one voice whose notes are appended in order.
inline ScoreData makeScore()
{
    ScoreData score;
    score.parts.emplace_back();
    score.parts.back().uniqueId = "P1";
    score.parts.back().measures.emplace_back();
    score.parts.back().measures.back().staves.emplace_back();
    return score;
}

inline NoteData &appendNote(ScoreData &ioScore, DurationName inDurationName, int inActualNotes, int inNormalNotes)
{
    auto &voice = ioScore.parts.back().measures.back().staves.back().voices[0];
    auto &note = voice.notes.emplace_back();
    note.pitchData.step = Step::c;
    note.pitchData.octave = 4;
    note.durationData.durationName = inDurationName;
    note.durationData.durationTimeTicks = 1;
    note.durationData.timeModificationActualNotes = inActualNotes;
    note.durationData.timeModificationNormalNotes = inNormalNotes;
    note.tickTimePosition = static_cast<int>(voice.notes.size()) - 1;
    return note;
}

// A tuplet start stating a ratio of its own, for example 3 eighths in the time of 2 eighths.
inline TupletStart makeStart(int inActualNumber, int inNormalNumber, DurationName inDurationName)
{
    TupletStart start;
    start.actualNumber = inActualNumber;
    start.actualDurationName = inDurationName;
    start.actualDots = 0;
    start.normalNumber = inNormalNumber;
    start.normalDurationName = inDurationName;
    start.normalDots = 0;
    return start;
}
} // namespace tupletApiTest

// A <tuplet> that states no ratio of its own is written back the same way. The api still reports
// the ratio, read from the note's time modification.
TEST(bareStartWritesNoPortions, Tuplet)
{
    using namespace tupletApiTest;
    const auto score = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1"/>)"));

    const auto &starts = attachments(score).tupletStarts;
    REQUIRE(1 == starts.size());
    CHECK(Bool::unspecified == starts.at(0).writeActual);
    CHECK(Bool::unspecified == starts.at(0).writeNormal);
    CHECK_EQUAL(3, starts.at(0).actualNumber);
    CHECK_EQUAL(2, starts.at(0).normalNumber);
    CHECK(DurationName::quarter == starts.at(0).actualDurationName);

    const auto written = toXml(score);
    CHECK(written.find("<tuplet-actual>") == std::string::npos);
    CHECK(written.find("<tuplet-normal>") == std::string::npos);
}

T_END

// A file that spells the ratio out keeps it, even though the note's time modification says the
// same thing.
TEST(statedPortionsAreWrittenBack, Tuplet)
{
    using namespace tupletApiTest;
    const auto score = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1">
            <tuplet-actual>
              <tuplet-number>3</tuplet-number>
              <tuplet-type>quarter</tuplet-type>
            </tuplet-actual>
            <tuplet-normal>
              <tuplet-number>2</tuplet-number>
              <tuplet-type>quarter</tuplet-type>
            </tuplet-normal>
          </tuplet>)"));

    const auto &starts = attachments(score).tupletStarts;
    REQUIRE(1 == starts.size());
    CHECK(Bool::yes == starts.at(0).writeActual);
    CHECK(Bool::yes == starts.at(0).writeNormal);

    const auto written = toXml(score);
    CHECK(written.find("<tuplet-actual>") != std::string::npos);
    CHECK(written.find("<tuplet-normal>") != std::string::npos);
    CHECK(written.find("<tuplet-type>quarter</tuplet-type>") != std::string::npos);
}

T_END

// A portion that states only part of its figure keeps exactly the part it stated. Nothing is
// invented to fill the gap, and the portion the file left out stays out.
TEST(partialPortionIsWrittenBackExactly, Tuplet)
{
    using namespace tupletApiTest;
    const auto score = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1">
            <tuplet-actual>
              <tuplet-number>3</tuplet-number>
            </tuplet-actual>
          </tuplet>)"));

    const auto &starts = attachments(score).tupletStarts;
    REQUIRE(1 == starts.size());
    CHECK(Bool::yes == starts.at(0).writeActual);
    CHECK(Bool::unspecified == starts.at(0).writeNormal);
    CHECK(DurationName::unspecified == starts.at(0).actualDurationName);

    const auto written = toXml(score);
    const auto actualPos = written.find("<tuplet-actual>");
    REQUIRE(actualPos != std::string::npos);
    const auto actualEnd = written.find("</tuplet-actual>", actualPos);
    REQUIRE(actualEnd != std::string::npos);
    const auto actual = written.substr(actualPos, actualEnd - actualPos);
    CHECK(actual.find("<tuplet-number>3</tuplet-number>") != std::string::npos);
    CHECK(actual.find("<tuplet-type>") == std::string::npos);
    CHECK(written.find("<tuplet-normal>") == std::string::npos);
}

T_END

// show-type says whether the note values of the ratio are drawn, the way show-number says
// whether its numbers are.
TEST(showTypeRoundTrips, Tuplet)
{
    using namespace tupletApiTest;
    const auto both =
        fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1" show-type="both"/>)"));
    REQUIRE(1 == attachments(both).tupletStarts.size());
    CHECK(Bool::yes == attachments(both).tupletStarts.at(0).showActualType);
    CHECK(Bool::yes == attachments(both).tupletStarts.at(0).showNormalType);
    CHECK(toXml(both).find(R"(show-type="both")") != std::string::npos);

    const auto actual = fromXml(
        scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1" show-type="actual"/>)"));
    CHECK(Bool::yes == attachments(actual).tupletStarts.at(0).showActualType);
    CHECK(Bool::no == attachments(actual).tupletStarts.at(0).showNormalType);
    CHECK(toXml(actual).find(R"(show-type="actual")") != std::string::npos);

    const auto none =
        fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1" show-type="none"/>)"));
    CHECK(Bool::no == attachments(none).tupletStarts.at(0).showActualType);
    CHECK(Bool::no == attachments(none).tupletStarts.at(0).showNormalType);
    CHECK(toXml(none).find(R"(show-type="none")") != std::string::npos);

    // a file that says nothing about show-type gets no attribute back
    const auto silent = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1"/>)"));
    CHECK(Bool::unspecified == attachments(silent).tupletStarts.at(0).showActualType);
    CHECK(toXml(silent).find("show-type") == std::string::npos);
}

T_END

// line-shape chooses between a straight bracket and the older curved one.
TEST(lineShapeRoundTrips, Tuplet)
{
    using namespace tupletApiTest;
    const auto curved = fromXml(
        scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1" line-shape="curved"/>)"));
    REQUIRE(1 == attachments(curved).tupletStarts.size());
    CHECK(TupletLineShape::curved == attachments(curved).tupletStarts.at(0).lineShape);
    CHECK(toXml(curved).find(R"(line-shape="curved")") != std::string::npos);

    const auto straight = fromXml(
        scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1" line-shape="straight"/>)"));
    CHECK(TupletLineShape::straight == attachments(straight).tupletStarts.at(0).lineShape);
    CHECK(toXml(straight).find(R"(line-shape="straight")") != std::string::npos);

    const auto silent = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="start" number="1"/>)"));
    CHECK(TupletLineShape::unspecified == attachments(silent).tupletStarts.at(0).lineShape);
    CHECK(toXml(silent).find("line-shape") == std::string::npos);
}

T_END

// Some programs state the bracket on the closing end of a tuplet rather than the opening one.
TEST(stopBracketRoundTrips, Tuplet)
{
    using namespace tupletApiTest;
    const auto score =
        fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="stop" number="1" bracket="no"/>)"));

    const auto &stops = attachments(score).tupletStops;
    REQUIRE(1 == stops.size());
    CHECK(Bool::no == stops.at(0).bracket);
    CHECK(toXml(score).find(R"(bracket="no")") != std::string::npos);

    const auto silent = fromXml(scoreXml(tripletTimeModification(), R"(          <tuplet type="stop" number="1"/>)"));
    CHECK(Bool::unspecified == attachments(silent).tupletStops.at(0).bracket);
    CHECK(toXml(silent).find("bracket") == std::string::npos);
}

T_END

// The nested case this whole design serves. An inner triplet of eighths inside a triplet of
// quarters gives its notes a cumulative 9:4, so the inner tuplet's own 3:2 differs from the
// note and is written, while the outer tuplet repeats its note's 3:2 and is left out.
TEST(nestedTupletWritesTheInnerRatioOnly, Tuplet)
{
    using namespace tupletApiTest;
    auto score = makeScore();

    auto &outerNote = appendNote(score, DurationName::quarter, 3, 2);
    outerNote.noteAttachmentData.tupletStarts.push_back(makeStart(3, 2, DurationName::quarter));

    auto &innerNote = appendNote(score, DurationName::eighth, 9, 4);
    innerNote.noteAttachmentData.tupletStarts.push_back(makeStart(3, 2, DurationName::eighth));

    const auto written = toXml(score);
    CHECK(written.find("<tuplet-type>quarter</tuplet-type>") == std::string::npos);
    CHECK(written.find("<tuplet-number>3</tuplet-number>") != std::string::npos);
    CHECK(written.find("<tuplet-type>eighth</tuplet-type>") != std::string::npos);
}

T_END

// The fidelity fields override the rule in both directions.
TEST(fidelityFieldsForceAndSilencePortions, Tuplet)
{
    using namespace tupletApiTest;

    // a ratio that differs from the note is written, unless told not to
    auto silenced = makeScore();
    auto &silencedNote = appendNote(silenced, DurationName::eighth, 9, 4);
    auto silencedStart = makeStart(3, 2, DurationName::eighth);
    silencedStart.writeActual = Bool::no;
    silencedStart.writeNormal = Bool::no;
    silencedNote.noteAttachmentData.tupletStarts.push_back(silencedStart);
    const auto silencedXml = toXml(silenced);
    CHECK(silencedXml.find("<tuplet-actual>") == std::string::npos);
    CHECK(silencedXml.find("<tuplet-normal>") == std::string::npos);

    // a ratio that repeats the note is left out, unless told to state it
    auto forced = makeScore();
    auto &forcedNote = appendNote(forced, DurationName::quarter, 3, 2);
    auto forcedStart = makeStart(3, 2, DurationName::quarter);
    forcedStart.writeActual = Bool::yes;
    forcedStart.writeNormal = Bool::yes;
    forcedNote.noteAttachmentData.tupletStarts.push_back(forcedStart);
    const auto forcedXml = toXml(forced);
    CHECK(forcedXml.find("<tuplet-actual>") != std::string::npos);
    CHECK(forcedXml.find("<tuplet-normal>") != std::string::npos);
}

T_END

// A portion without a note value is written without a <tuplet-type>; the old writer turned the
// missing value into a maxima.
TEST(unspecifiedDurationNameWritesNoTupletType, Tuplet)
{
    using namespace tupletApiTest;
    auto score = makeScore();
    auto &note = appendNote(score, DurationName::quarter, 3, 2);
    TupletStart start;
    start.actualNumber = 5;
    start.writeActual = Bool::yes;
    note.noteAttachmentData.tupletStarts.push_back(start);

    const auto written = toXml(score);
    CHECK(written.find("<tuplet-number>5</tuplet-number>") != std::string::npos);
    CHECK(written.find("<tuplet-type>") == std::string::npos);
    CHECK(written.find("<tuplet-normal>") == std::string::npos);
}

T_END

#endif
