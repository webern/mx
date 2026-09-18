// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/api/TestHelpers.h"

#include <map>

using namespace std;
using namespace mx::api;
using namespace mxtest;

// issue #441: peeking ahead for a chord tag skipped over <backup>/<forward>, so a chord note
// past one of those wrongly marked the note before it as a chord and stalled its time advance,
// which then sent the backup negative and threw off every position that followed
TEST(chordAfterBackupDoesNotAttachToPriorNote, BackupChord)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>MusicXML Part</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>8</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
      <backup>
        <duration>4</duration>
      </backup>
      <note>
        <chord/>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <voice>2</voice>
        <type>eighth</type>
      </note>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 1);
    const auto &staff = measure.staves.at(0);
    REQUIRE(staff.voices.size() == 2);
    const auto &voiceOneNotes = staff.voices.at(0).notes;
    const auto &voiceTwoNotes = staff.voices.at(1).notes;
    REQUIRE(voiceOneNotes.size() == 1);
    REQUIRE(voiceTwoNotes.size() == 2);

    // the note before the backup does not join the chord on the far side of it
    CHECK(!voiceOneNotes.at(0).isChord);
    CHECK(voiceTwoNotes.at(0).isChord);
    CHECK(!voiceTwoNotes.at(1).isChord);

    // the backup rewinds from the first note's end, not from a cursor that never advanced
    CHECK_EQUAL(0, voiceOneNotes.at(0).tickTimePosition);
    CHECK_EQUAL(4, voiceTwoNotes.at(0).tickTimePosition);
    CHECK_EQUAL(8, voiceTwoNotes.at(1).tickTimePosition);
}

T_END;

// MusicXML requires a positive duration on <backup>, but core repairs a non-positive one to a
// value so small that it scales to zero ticks. Such a backup moves nothing, so the chord tone
// after it really is at the position of the note before it, and the peek must not stop there.
TEST(chordAfterAZeroLengthBackupStillAttaches, BackupChord)
{
    const std::string xml = R"(<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>MusicXML Part</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>8</divisions>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
      <backup>
        <duration>0</duration>
      </backup>
      <note>
        <chord/>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    const auto score = mxtest::fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 1);

    // the reader may bucket the notes into more than one voice, so find each one by its step
    std::map<Step, NoteData> notesByStep;
    for (const auto &voice : measure.staves.at(0).voices)
    {
        for (const auto &note : voice.second.notes)
        {
            notesByStep.emplace(note.pitchData.step, note);
        }
    }
    REQUIRE(notesByStep.size() == 3);

    // the zero-length backup does not separate the chord tone from the note before it
    CHECK(notesByStep.at(Step::c).isChord);
    CHECK(notesByStep.at(Step::e).isChord);
    CHECK(!notesByStep.at(Step::g).isChord);

    CHECK_EQUAL(0, notesByStep.at(Step::c).tickTimePosition);
    CHECK_EQUAL(0, notesByStep.at(Step::e).tickTimePosition);
    CHECK_EQUAL(8, notesByStep.at(Step::g).tickTimePosition);
}

T_END;

#endif
