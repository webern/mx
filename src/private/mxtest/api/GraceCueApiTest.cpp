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
