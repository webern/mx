// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <sstream>
#include <string>

using namespace std;
using namespace mx::api;
using namespace mxtest;

// The four combinations of NoteData::isGrace / isCue map to MusicXML's four
// <note> flavors: normal, <cue/>, <grace/>, and <grace/><cue/>. Issue #288.
TEST(allFourCombinationsSurviveRoundTrip, GraceCue)
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

    // a normal quarter note
    voice.notes.emplace_back();
    voice.notes.back().tickTimePosition = 0;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    // a cue quarter note
    voice.notes.emplace_back();
    voice.notes.back().isCue = true;
    voice.notes.back().tickTimePosition = 4;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    // a grace note (no wire duration)
    voice.notes.emplace_back();
    voice.notes.back().isGrace = true;
    voice.notes.back().tickTimePosition = 8;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    // a grace note inside a cue passage (no wire duration)
    voice.notes.emplace_back();
    voice.notes.back().isGrace = true;
    voice.notes.back().isCue = true;
    voice.notes.back().tickTimePosition = 8;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    // a final normal quarter note that the grace notes attach to
    voice.notes.emplace_back();
    voice.notes.back().tickTimePosition = 8;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 5);

    CHECK(!outNotes.at(0).isGrace);
    CHECK(!outNotes.at(0).isCue);

    CHECK(!outNotes.at(1).isGrace);
    CHECK(outNotes.at(1).isCue);

    CHECK(outNotes.at(2).isGrace);
    CHECK(!outNotes.at(2).isCue);

    CHECK(outNotes.at(3).isGrace);
    CHECK(outNotes.at(3).isCue);

    CHECK(!outNotes.at(4).isGrace);
    CHECK(!outNotes.at(4).isCue);
}

T_END;

TEST(readAllFourCombinations, GraceCue)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>x</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
      </attributes>
      <note>
        <pitch><step>C</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
      <note>
        <cue/>
        <pitch><step>D</step><octave>4</octave></pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
      <note>
        <grace/>
        <pitch><step>E</step><octave>4</octave></pitch>
        <type>eighth</type>
      </note>
      <note>
        <grace/>
        <cue/>
        <pitch><step>F</step><octave>4</octave></pitch>
        <type>eighth</type>
      </note>
      <note>
        <pitch><step>G</step><octave>4</octave></pitch>
        <duration>2</duration>
        <type>half</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = fromXml(xml);
    const auto &notes = score.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(notes.size() == 5);

    CHECK(!notes.at(0).isGrace);
    CHECK(!notes.at(0).isCue);

    CHECK(!notes.at(1).isGrace);
    CHECK(notes.at(1).isCue);

    CHECK(notes.at(2).isGrace);
    CHECK(!notes.at(2).isCue);
    CHECK_EQUAL(0, notes.at(2).durationData.durationTimeTicks);

    CHECK(notes.at(3).isGrace);
    CHECK(notes.at(3).isCue);
    CHECK_EQUAL(0, notes.at(3).durationData.durationTimeTicks);

    CHECK(!notes.at(4).isGrace);
    CHECK(!notes.at(4).isCue);
}

T_END;

TEST(writeGraceCueElements, GraceCue)
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
    voice.notes.back().isGrace = true;
    voice.notes.back().isCue = true;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    voice.notes.emplace_back();
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto xml = toXml(score);

    // the grace-cue note emits both empty elements and no <duration>
    const auto gracePos = xml.find("<grace");
    const auto cuePos = xml.find("<cue");
    CHECK(gracePos != std::string::npos);
    CHECK(cuePos != std::string::npos);
    CHECK(gracePos < cuePos);

    // exactly one <duration> in the measure: the normal note's
    const auto firstDuration = xml.find("<duration>");
    REQUIRE(firstDuration != std::string::npos);
    CHECK(xml.find("<duration>", firstDuration + 1) == std::string::npos);
}

T_END;

// cue and grace-cue notes are silent and the schema gives them no <tie>, so a
// tie on one is notation only: the <tied> is written, the <tie> is not
TEST(cueNoteTiesAreNotationOnly, GraceCue)
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
    voice.notes.back().isCue = true;
    voice.notes.back().isTieStart = true;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    voice.notes.emplace_back();
    voice.notes.back().isGrace = true;
    voice.notes.back().isCue = true;
    voice.notes.back().isTieStop = true;
    voice.notes.back().tickTimePosition = 4;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    const auto xml = toXml(score);
    CHECK(xml.find("<tie ") == std::string::npos);
    CHECK(xml.find("<tied type=\"start\"") != std::string::npos);
    CHECK(xml.find("<tied type=\"stop\"") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 2);
    CHECK(outNotes.at(0).isCue);
    CHECK(outNotes.at(0).isTieStart);
    CHECK(outNotes.at(1).isCue);
    CHECK(outNotes.at(1).isGrace);
    CHECK(outNotes.at(1).isTieStop);
}

T_END;

// the <tied> notation is written exactly once, whether it comes from the
// isTieStart / isTieStop flags or from an attribute-bearing tie curve
TEST(tiedNotationIsNotDuplicated, GraceCue)
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
    voice.notes.back().isTieStart = true;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;
    voice.notes.back().noteAttachmentData.curveStarts.emplace_back(CurveType::tie);

    const auto xml = toXml(score);
    const auto first = xml.find("<tied ");
    REQUIRE(first != std::string::npos);
    CHECK(xml.find("<tied ", first + 1) == std::string::npos);
    CHECK(xml.find("<tie ") != std::string::npos);
}

T_END;

// grace-normal notes keep their ties (only cue and grace-cue drop them)
TEST(graceNormalNoteTiesAreKept, GraceCue)
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
    voice.notes.back().isGrace = true;
    voice.notes.back().isTieStart = true;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    voice.notes.emplace_back();
    voice.notes.back().isTieStop = true;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 2);
    CHECK(outNotes.at(0).isGrace);
    CHECK(!outNotes.at(0).isCue);
    CHECK(outNotes.at(0).isTieStart);
    CHECK(outNotes.at(1).isTieStop);
}

T_END;

// <grace>'s slash attribute (slashed grace notes, e.g. acciaccatura).
TEST(graceSlashRoundTrip, GraceCue)
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
    voice.notes.back().isGrace = true;
    voice.notes.back().graceSlash = Bool::yes;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    voice.notes.emplace_back();
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find(R"(<grace slash="yes" />)") != std::string::npos);

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 2);
    CHECK(outNotes.at(0).graceSlash == Bool::yes);
}

T_END;

// A grace note followed by the quarter note it leads into, at 4 ticks per quarter.
inline ScoreData graceTimeScore()
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    auto &part = score.parts.emplace_back();
    part.uniqueId = "P1";
    auto &voice = part.measures.emplace_back().staves.emplace_back().voices[0];

    auto &grace = voice.notes.emplace_back();
    grace.isGrace = true;
    grace.durationData.durationName = DurationName::eighth;

    auto &main = voice.notes.emplace_back();
    main.durationData.durationName = DurationName::quarter;
    main.durationData.durationTimeTicks = 4;
    return score;
}

inline NoteData &graceTimeNote(ScoreData &score, size_t partIndex = 0)
{
    return score.parts.at(partIndex).measures.front().staves.front().voices.at(0).notes.front();
}

inline Diagnostics graceTimeWriteDiagnostics(const ScoreData &score)
{
    Diagnostics diagnostics;
    REQUIRE(fromScore(score, diagnostics).ok());
    return diagnostics;
}

inline ScoreData graceTimeRead(const std::string &xml, Diagnostics &diagnostics)
{
    std::istringstream stream{xml};
    auto document = MusicXml::fromStream(stream);
    REQUIRE(document.ok());
    const auto score = getScore(document.value(), diagnostics);
    REQUIRE(score.ok());
    return score.value();
}

// <grace>'s steal-time-previous, steal-time-following and make-time attributes.
TEST(graceTimeRoundTrip, GraceCue)
{
    auto score = graceTimeScore();
    auto &grace = graceTimeNote(score);
    grace.graceSlash = Bool::yes;
    grace.graceStealTimePrevious = 20.0;
    grace.graceStealTimeFollowing = 33.5;
    grace.graceMakeTimeTicks = 3;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find(R"(steal-time-previous="20")") != std::string::npos);
    CHECK(xml.find(R"(steal-time-following="33.5")") != std::string::npos);
    CHECK(xml.find(R"(make-time="3")") != std::string::npos);
    CHECK(xml.find(R"(slash="yes")") != std::string::npos);
    CHECK(graceTimeWriteDiagnostics(score).all().empty());

    auto out = roundTrip(score);
    const auto &outGrace = graceTimeNote(out);
    CHECK(outGrace.graceSlash == Bool::yes);
    REQUIRE(outGrace.graceStealTimePrevious.has_value());
    CHECK_DOUBLES_EQUAL(20.0, *outGrace.graceStealTimePrevious, 1e-9);
    REQUIRE(outGrace.graceStealTimeFollowing.has_value());
    CHECK_DOUBLES_EQUAL(33.5, *outGrace.graceStealTimeFollowing, 1e-9);
    REQUIRE(outGrace.graceMakeTimeTicks.has_value());
    CHECK_EQUAL(3, *outGrace.graceMakeTimeTicks);
}

T_END;

// Each attribute is written on its own when only its field is set.
TEST(graceTimeFieldsAreIndependent, GraceCue)
{
    auto score = graceTimeScore();
    graceTimeNote(score).graceStealTimeFollowing = 50.0;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find(R"(<grace steal-time-following="50" />)") != std::string::npos);

    auto out = roundTrip(score);
    const auto &outGrace = graceTimeNote(out);
    CHECK(!outGrace.graceStealTimePrevious.has_value());
    CHECK(outGrace.graceStealTimeFollowing.has_value());
    CHECK(!outGrace.graceMakeTimeTicks.has_value());
}

T_END;

// Empty fields (the default) write a bare <grace/> and read back empty.
TEST(graceTimeDefaultsOmitAttributes, GraceCue)
{
    auto score = graceTimeScore();

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<grace />") != std::string::npos);
    CHECK(xml.find("steal-time") == std::string::npos);
    CHECK(xml.find("make-time") == std::string::npos);

    auto out = roundTrip(score);
    const auto &outGrace = graceTimeNote(out);
    CHECK(outGrace.isGrace);
    CHECK(!outGrace.graceStealTimePrevious.has_value());
    CHECK(!outGrace.graceStealTimeFollowing.has_value());
    CHECK(!outGrace.graceMakeTimeTicks.has_value());
}

T_END;

// make-time is in the measure's divisions on the page and in ticks in the api. With parts at 2 and 3
// divisions the score has 6 ticks per quarter, so make-time 1 in the second part is 2 ticks, and it
// is written back as 2 against the single <divisions> of 6.
TEST(graceMakeTimeScalesToTicks, GraceCue)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1"><part-name>One</part-name></score-part>
    <score-part id="P2"><part-name>Two</part-name></score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes><divisions>2</divisions></attributes>
      <note><pitch><step>C</step><octave>4</octave></pitch><duration>2</duration><type>quarter</type></note>
    </measure>
  </part>
  <part id="P2">
    <measure number="1">
      <attributes><divisions>3</divisions></attributes>
      <note><grace make-time="1"/><pitch><step>D</step><octave>4</octave></pitch><type>eighth</type></note>
      <note><pitch><step>C</step><octave>4</octave></pitch><duration>3</duration><type>quarter</type></note>
    </measure>
  </part>
</score-partwise>)";

    Diagnostics diagnostics;
    auto score = graceTimeRead(xml, diagnostics);
    CHECK(diagnostics.all().empty());
    CHECK_EQUAL(6, score.ticksPerQuarter);
    const auto &grace = graceTimeNote(score, 1);
    REQUIRE(grace.isGrace);
    REQUIRE(grace.graceMakeTimeTicks.has_value());
    CHECK_EQUAL(2, *grace.graceMakeTimeTicks);

    const auto written = mxtest::toXml(score);
    CHECK(written.find("<divisions>6</divisions>") != std::string::npos);
    CHECK(written.find(R"(make-time="2")") != std::string::npos);

    auto out = roundTrip(score);
    REQUIRE(graceTimeNote(out, 1).graceMakeTimeTicks.has_value());
    CHECK_EQUAL(2, *graceTimeNote(out, 1).graceMakeTimeTicks);
}

T_END;

// A make-time that is not a whole number of ticks is rounded, as a duration is, and reported.
TEST(fractionalGraceMakeTimeIsRoundedAndReported, GraceCue)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<score-partwise version="4.0">
  <part-list><score-part id="P1"><part-name>One</part-name></score-part></part-list>
  <part id="P1">
    <measure number="1">
      <attributes><divisions>4</divisions></attributes>
      <note><grace make-time="1.25"/><pitch><step>D</step><octave>4</octave></pitch><type>eighth</type></note>
      <note><pitch><step>C</step><octave>4</octave></pitch><duration>4</duration><type>quarter</type></note>
    </measure>
  </part>
</score-partwise>)";

    Diagnostics diagnostics;
    auto score = graceTimeRead(xml, diagnostics);
    REQUIRE(graceTimeNote(score).graceMakeTimeTicks.has_value());
    CHECK_EQUAL(1, *graceTimeNote(score).graceMakeTimeTicks);
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    CHECK(Severity::warning == diagnostic.severity);
    CHECK(DiagnosticCode::valueAdjusted == diagnostic.code);
    CHECK_EQUAL(std::string{"grace make-time duration 1.25 is 1.25 ticks; using 1"}, diagnostic.message);
}

T_END;

// A steal time outside 0 to 100 is written as the nearer end of that range, and reported.
TEST(outOfRangeGraceStealTimeIsClampedAndReported, GraceCue)
{
    auto score = graceTimeScore();
    graceTimeNote(score).graceStealTimePrevious = 150.0;
    graceTimeNote(score).graceStealTimeFollowing = -5.0;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find(R"(steal-time-previous="100")") != std::string::npos);
    CHECK(xml.find(R"(steal-time-following="0")") != std::string::npos);

    const auto diagnostics = graceTimeWriteDiagnostics(score);
    REQUIRE(diagnostics.all().size() == 2);
    CHECK(DiagnosticCode::valueAdjusted == diagnostics.all()[0].code);
    CHECK_EQUAL(std::string{"grace steal-time-previous 150 is out of range; using 100"}, diagnostics.all()[0].message);
    CHECK_EQUAL(0, diagnostics.all()[0].location.partIndex);
    CHECK_EQUAL(0, diagnostics.all()[0].location.measureIndex);
    CHECK(DiagnosticCode::valueAdjusted == diagnostics.all()[1].code);
    CHECK_EQUAL(std::string{"grace steal-time-following -5 is out of range; using 0"}, diagnostics.all()[1].message);
}

T_END;

// The schema puts no lower bound on make-time, so a negative value is written as it is.
TEST(negativeGraceMakeTimeIsWrittenUnchanged, GraceCue)
{
    auto score = graceTimeScore();
    graceTimeNote(score).graceMakeTimeTicks = -2;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find(R"(make-time="-2")") != std::string::npos);
    CHECK(graceTimeWriteDiagnostics(score).all().empty());

    auto out = roundTrip(score);
    REQUIRE(graceTimeNote(out).graceMakeTimeTicks.has_value());
    CHECK_EQUAL(-2, *graceTimeNote(out).graceMakeTimeTicks);
}

T_END;

// On a note that is not a grace note the three fields are ignored.
TEST(graceTimeOnNonGraceNoteIsIgnored, GraceCue)
{
    auto score = graceTimeScore();
    auto &note = graceTimeNote(score);
    note.isGrace = false;
    note.durationData.durationTimeTicks = 2;
    note.graceStealTimePrevious = 20.0;
    note.graceStealTimeFollowing = 20.0;
    note.graceMakeTimeTicks = 1;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<grace") == std::string::npos);
    CHECK(graceTimeWriteDiagnostics(score).all().empty());
}

T_END;

// Each grace time field takes part in NoteData equality on its own.
TEST(graceTimeFieldsTakePartInEquality, GraceCue)
{
    const NoteData base;

    NoteData previous;
    previous.graceStealTimePrevious = 10.0;
    CHECK(!(base == previous));

    NoteData following;
    following.graceStealTimeFollowing = 10.0;
    CHECK(!(base == following));

    NoteData makeTime;
    makeTime.graceMakeTimeTicks = 1;
    CHECK(!(base == makeTime));
}

T_END;

// An unspecified graceSlash (the default) must not emit the attribute at all.
TEST(graceSlashUnspecifiedOmitsAttribute, GraceCue)
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
    voice.notes.back().isGrace = true;
    voice.notes.back().durationData.durationName = DurationName::eighth;
    voice.notes.back().durationData.durationTimeTicks = 0;

    voice.notes.emplace_back();
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("slash") == std::string::npos);

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 2);
    CHECK(outNotes.at(0).graceSlash == Bool::unspecified);
}

T_END;

// Issue #312: two consecutive grace notes sharing a tick, followed by the normal note they
// attach to, must not collapse on round trip.
TEST(consecutiveGraceNotesAtSameTickSurviveRoundTrip, GraceCue)
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
    voice.notes.back().isGrace = true;
    voice.notes.back().tickTimePosition = 0;
    voice.notes.back().durationData.durationName = DurationName::eighth;

    voice.notes.emplace_back();
    voice.notes.back().isGrace = true;
    voice.notes.back().tickTimePosition = 0;
    voice.notes.back().durationData.durationName = DurationName::eighth;

    voice.notes.emplace_back();
    voice.notes.back().tickTimePosition = 0;
    voice.notes.back().durationData.durationName = DurationName::quarter;
    voice.notes.back().durationData.durationTimeTicks = 4;

    const auto xml = mxtest::toXml(score);

    // no cursor-correcting <backup> should be needed: grace notes carry no wire duration
    CHECK(xml.find("<backup>") == std::string::npos);

    const auto out = roundTrip(score);
    const auto &outNotes = out.parts.back().measures.back().staves.back().voices.at(0).notes;
    REQUIRE(outNotes.size() == 3);
    CHECK(outNotes.at(0).isGrace);
    CHECK(outNotes.at(1).isGrace);
    CHECK(!outNotes.at(2).isGrace);
}

T_END;

#endif
