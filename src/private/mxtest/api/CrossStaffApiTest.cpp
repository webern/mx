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
#include <string>
#include <vector>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace crossStaffApiTest
{
inline size_t countOccurrences(const std::string &haystack, const std::string &needle)
{
    size_t count = 0;
    for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1))
    {
        ++count;
    }
    return count;
}

// two staves, one voice on staff 1 whose beamed run dips onto staff 2 for its middle two
// notes, plus a whole rest on staff 2
inline ScoreData makeCrossStaffScore()
{
    ScoreData score;
    score.ticksPerQuarter = 2;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    measure.staves.emplace_back();

    auto &notes = measure.staves.at(0).voices[0].notes;

    for (int i = 0; i < 4; ++i)
    {
        NoteData note;
        note.pitchData.step = Step::c;
        note.pitchData.octave = 5;
        note.tickTimePosition = i;
        note.durationData.durationName = DurationName::eighth;
        note.durationData.durationTimeTicks = 1;
        notes.push_back(note);
    }

    notes.at(0).beams = {Beam::begin};
    notes.at(1).beams = {Beam::extend};
    notes.at(2).beams = {Beam::extend};
    notes.at(3).beams = {Beam::end};
    notes.at(1).crossStaffIndex = 1;
    notes.at(2).crossStaffIndex = 1;

    NoteData rest;
    rest.isRest = true;
    rest.tickTimePosition = 0;
    rest.durationData.durationName = DurationName::whole;
    rest.durationData.durationTimeTicks = 8;
    measure.staves.at(1).voices[0].notes.push_back(rest);

    return score;
}
} // namespace crossStaffApiTest

TEST(writeCrossStaffOverride, CrossStaff)
{
    const auto score = crossStaffApiTest::makeCrossStaffScore();
    const auto xml = mxtest::toXml(score);

    // the two overridden notes plus the staff-2 rest
    CHECK_EQUAL(3, crossStaffApiTest::countOccurrences(xml, "<staff>2</staff>"));

    // the two non-overridden beamed notes
    CHECK_EQUAL(2, crossStaffApiTest::countOccurrences(xml, "<staff>1</staff>"));

    // the beamed run stays adjacent in the stream: no forward, and only the single backup
    // that rewinds to write the staff-2 rest
    CHECK_EQUAL(0, crossStaffApiTest::countOccurrences(xml, "<forward>"));
    CHECK_EQUAL(1, crossStaffApiTest::countOccurrences(xml, "<backup>"));
}

T_END;

TEST(roundTripCrossStaffOverride, CrossStaff)
{
    const auto score = crossStaffApiTest::makeCrossStaffScore();
    const auto out = mxtest::roundTrip(score);

    REQUIRE(out.parts.size() == 1);
    const auto &measure = out.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);
    REQUIRE(measure.staves.at(0).voices.size() == 1);
    const auto &notes = measure.staves.at(0).voices.at(0).notes;
    REQUIRE(notes.size() == 4);

    CHECK(!notes.at(0).crossStaffIndex.has_value());
    REQUIRE(notes.at(1).crossStaffIndex.has_value());
    CHECK_EQUAL(1, *notes.at(1).crossStaffIndex);
    REQUIRE(notes.at(2).crossStaffIndex.has_value());
    CHECK_EQUAL(1, *notes.at(2).crossStaffIndex);
    CHECK(!notes.at(3).crossStaffIndex.has_value());

    CHECK_EQUAL(0, notes.at(0).tickTimePosition);
    CHECK_EQUAL(1, notes.at(1).tickTimePosition);
    CHECK_EQUAL(2, notes.at(2).tickTimePosition);
    CHECK_EQUAL(3, notes.at(3).tickTimePosition);

    REQUIRE(measure.staves.at(1).voices.size() == 1);
    const auto &staffTwoNotes = measure.staves.at(1).voices.at(0).notes;
    REQUIRE(staffTwoNotes.size() == 1);
    CHECK(staffTwoNotes.at(0).isRest);
    CHECK(!staffTwoNotes.at(0).crossStaffIndex.has_value());
}

T_END;

namespace crossStaffApiTest
{
constexpr const char *const xmlHeader = R"(<score-partwise version="3.0">
  <part-list>
    <score-part id="P1">
      <part-name>MusicXML Part</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>2</divisions>
        <staves>2</staves>
        <clef number="1">
          <sign>G</sign>
          <line>2</line>
        </clef>
        <clef number="2">
          <sign>F</sign>
          <line>4</line>
        </clef>
      </attributes>
)";

constexpr const char *const xmlFooter = R"(    </measure>
  </part>
</score-partwise>
)";

inline std::string makeNoteXml(const char *step, int octave, const char *staff, const char *beam)
{
    std::string xml;
    xml += "      <note>\n        <pitch>\n          <step>";
    xml += step;
    xml += "</step>\n          <octave>";
    xml += std::to_string(octave);
    xml += "</octave>\n        </pitch>\n        <duration>1</duration>\n        <voice>1</voice>\n";
    xml += "        <type>eighth</type>\n        <staff>";
    xml += staff;
    xml += "</staff>\n";
    if (beam)
    {
        xml += "        <beam number=\"1\">";
        xml += beam;
        xml += "</beam>\n";
    }
    xml += "      </note>\n";
    return xml;
}
} // namespace crossStaffApiTest

// the ly43d measure 1 shape: a voice living on staff 2 with two beam groups that cross to
// staff 1; the second group begins on the foreign staff, which exercises the sticky home
TEST(readStickyHomeCrossStaffBeams, CrossStaff)
{
    using namespace crossStaffApiTest;
    std::string xml = xmlHeader;
    xml += makeNoteXml("A", 3, "2", "begin");
    xml += makeNoteXml("E", 4, "1", "continue");
    xml += makeNoteXml("A", 3, "2", "continue");
    xml += makeNoteXml("E", 4, "1", "end");
    xml += makeNoteXml("C", 5, "1", "begin");
    xml += makeNoteXml("E", 4, "1", "continue");
    xml += makeNoteXml("A", 3, "2", "continue");
    xml += makeNoteXml("B", 4, "2", "end");
    xml += xmlFooter;

    const auto score = mxtest::fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);

    // the whole voice stays on its home staff (staff 2)
    CHECK(measure.staves.at(0).voices.empty());
    REQUIRE(measure.staves.at(1).voices.size() == 1);
    const auto &notes = measure.staves.at(1).voices.at(0).notes;
    REQUIRE(notes.size() == 8);

    // every note whose <staff> named staff 1 carries the override; the rest do not
    const std::vector<bool> expectCross{false, true, false, true, true, true, false, false};
    for (size_t i = 0; i < notes.size(); ++i)
    {
        if (expectCross.at(i))
        {
            REQUIRE(notes.at(i).crossStaffIndex.has_value());
            CHECK_EQUAL(0, *notes.at(i).crossStaffIndex);
        }
        else
        {
            CHECK(!notes.at(i).crossStaffIndex.has_value());
        }
        CHECK_EQUAL(static_cast<int>(i), notes.at(i).tickTimePosition);
    }
}

T_END;

TEST(readCrossStaffChord, CrossStaff)
{
    using namespace crossStaffApiTest;
    std::string xml = xmlHeader;
    xml += R"(      <note>
        <pitch>
          <step>C</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <chord/>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
)";
    xml += xmlFooter;

    const auto score = mxtest::fromXml(xml);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);

    // the chord member follows its chord's first note into staff 2 with the override set
    CHECK(measure.staves.at(0).voices.empty());
    REQUIRE(measure.staves.at(1).voices.size() == 1);
    const auto &notes = measure.staves.at(1).voices.at(0).notes;
    REQUIRE(notes.size() == 2);
    CHECK(notes.at(0).isChord);
    CHECK(notes.at(1).isChord);
    CHECK(!notes.at(0).crossStaffIndex.has_value());
    REQUIRE(notes.at(1).crossStaffIndex.has_value());
    CHECK_EQUAL(0, *notes.at(1).crossStaffIndex);
    CHECK_EQUAL(notes.at(0).tickTimePosition, notes.at(1).tickTimePosition);
}

T_END;

TEST(readUnbeamedStaffChangeIsStructural, CrossStaff)
{
    using namespace crossStaffApiTest;
    std::string xml = xmlHeader;
    xml += R"(      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
)";
    xml += xmlFooter;

    const auto score = mxtest::fromXml(xml);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);

    // no beam, no chord: the notes bucket to the staves their <staff> values name
    REQUIRE(measure.staves.at(0).voices.size() == 1);
    REQUIRE(measure.staves.at(1).voices.size() == 1);
    const auto &staffOneNotes = measure.staves.at(0).voices.at(0).notes;
    const auto &staffTwoNotes = measure.staves.at(1).voices.at(0).notes;
    REQUIRE(staffOneNotes.size() == 1);
    REQUIRE(staffTwoNotes.size() == 1);
    CHECK(!staffOneNotes.at(0).crossStaffIndex.has_value());
    CHECK(!staffTwoNotes.at(0).crossStaffIndex.has_value());
}

T_END;

TEST(readPureForeignStaffGroupNotRelocated, CrossStaff)
{
    using namespace crossStaffApiTest;
    std::string xml = xmlHeader;
    xml += R"(      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>2</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
)";
    xml += makeNoteXml("C", 3, "2", "begin");
    xml += makeNoteXml("D", 3, "2", "continue");
    xml += makeNoteXml("E", 3, "2", "continue");
    xml += makeNoteXml("F", 3, "2", "end");
    xml += xmlFooter;

    const auto score = mxtest::fromXml(xml);
    const auto &measure = score.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);

    // a beam group entirely on one staff never relocates, even though the voice started
    // on the other staff
    REQUIRE(measure.staves.at(0).voices.size() == 1);
    REQUIRE(measure.staves.at(1).voices.size() == 1);
    CHECK_EQUAL(1, measure.staves.at(0).voices.at(0).notes.size());
    const auto &groupNotes = measure.staves.at(1).voices.at(0).notes;
    REQUIRE(groupNotes.size() == 4);

    for (const auto &note : groupNotes)
    {
        CHECK(!note.crossStaffIndex.has_value());
    }
}

T_END;

namespace crossStaffApiTest
{
// Voice 1 beams four eighths that cross down to staff 2 for the third one, and leaves the second
// eighth as a rest. Voice 2 plays in that rest, on the staff {otherVoiceStaff} names. There is no
// <backup>: the <voice> text alone tells the two voices apart.
constexpr const char *const kOtherVoiceInBeamGroupXml = R"(      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>eighth</type>
        <staff>1</staff>
        <beam number="1">begin</beam>
      </note>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <voice>2</voice>
        <type>eighth</type>
        <staff>{otherVoiceStaff}</staff>
      </note>
      <note>
        <pitch>
          <step>A</step>
          <octave>3</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>eighth</type>
        <staff>2</staff>
        <beam number="1">continue</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>eighth</type>
        <staff>1</staff>
        <beam number="1">end</beam>
      </note>
)";

// The score of kOtherVoiceInBeamGroupXml with voice 2 on staff inOtherVoiceStaff (1 or 2).
inline std::string makeOtherVoiceInBeamGroupXml(int inOtherVoiceStaff)
{
    std::string body = kOtherVoiceInBeamGroupXml;
    const std::string placeholder = "{otherVoiceStaff}";
    body.replace(body.find(placeholder), placeholder.size(), std::to_string(inOtherVoiceStaff));
    return std::string{xmlHeader} + body + xmlFooter;
}

// Checks that the voice 1 beam group of kOtherVoiceInBeamGroupXml stays together on staff 1, with
// its third note marked as drawn on staff 2, and that voice 2 is a voice of its own on staff
// inOtherVoiceStaff.
inline void checkOtherVoiceInBeamGroup(const ScoreData &inScore, int inOtherVoiceStaff)
{
    REQUIRE(inScore.parts.size() == 1);
    const auto &measure = inScore.parts.at(0).measures.at(0);
    REQUIRE(measure.staves.size() == 2);

    const auto &groupVoices = measure.staves.at(0).voices;
    const auto &otherVoices = measure.staves.at(inOtherVoiceStaff - 1).voices;
    const bool isSameStaff = inOtherVoiceStaff == 1;
    REQUIRE(groupVoices.size() == (isSameStaff ? 2 : 1));
    REQUIRE(measure.staves.at(1).voices.size() == (isSameStaff ? 0 : 1));
    const auto &groupNotes = groupVoices.begin()->second.notes;
    const auto &otherNotes = std::prev(otherVoices.end())->second.notes;

    REQUIRE(groupNotes.size() == 3);
    CHECK(groupNotes.at(0).pitchData.step == Step::c);
    CHECK(groupNotes.at(1).pitchData.step == Step::a);
    CHECK(groupNotes.at(2).pitchData.step == Step::e);
    CHECK_EQUAL(0, groupNotes.at(0).tickTimePosition);
    CHECK_EQUAL(2, groupNotes.at(1).tickTimePosition);
    CHECK_EQUAL(3, groupNotes.at(2).tickTimePosition);
    CHECK(!groupNotes.at(0).crossStaffIndex.has_value());
    REQUIRE(groupNotes.at(1).crossStaffIndex.has_value());
    CHECK_EQUAL(1, *groupNotes.at(1).crossStaffIndex);
    CHECK(!groupNotes.at(2).crossStaffIndex.has_value());
    CHECK(groupNotes.at(0).beams == std::vector<Beam>{Beam::begin});
    CHECK(groupNotes.at(1).beams == std::vector<Beam>{Beam::extend});
    CHECK(groupNotes.at(2).beams == std::vector<Beam>{Beam::end});

    REQUIRE(otherNotes.size() == 1);
    CHECK(otherNotes.at(0).pitchData.step == Step::g);
    CHECK_EQUAL(1, otherNotes.at(0).tickTimePosition);
    CHECK(!otherNotes.at(0).crossStaffIndex.has_value());
}
} // namespace crossStaffApiTest

// a note of another voice inside a cross-staff beam group, with no <backup>, does not break the
// group
TEST(readCrossStaffBeamGroupAroundOtherVoice, CrossStaff)
{
    using namespace crossStaffApiTest;
    const auto score = mxtest::fromXml(makeOtherVoiceInBeamGroupXml(1));
    checkOtherVoiceInBeamGroup(score, 1);
    checkOtherVoiceInBeamGroup(mxtest::fromXml(mxtest::toXml(score)), 1);
}

T_END;

// the same, with the other voice on the staff the group crosses to
TEST(writeCrossStaffBeamGroupAroundOtherStaffVoice, CrossStaff)
{
    using namespace crossStaffApiTest;
    const auto score = mxtest::fromXml(makeOtherVoiceInBeamGroupXml(2));
    checkOtherVoiceInBeamGroup(score, 2);
    checkOtherVoiceInBeamGroup(mxtest::fromXml(mxtest::toXml(score)), 2);
}

T_END;

// a beam group over a rest written as a <forward> stays one group, so a group that crosses staves
// keeps its notes on one staff
TEST(readCrossStaffBeamGroupOverForwardRest, CrossStaff)
{
    using namespace crossStaffApiTest;
    std::string xml = xmlHeader;
    xml += makeNoteXml("C", 5, "1", "begin");
    xml += R"(      <forward>
        <duration>1</duration>
      </forward>
)";
    xml += makeNoteXml("A", 3, "2", "continue");
    xml += makeNoteXml("E", 5, "1", "end");
    xml += xmlFooter;

    const auto check = [](const ScoreData &inScore) {
        REQUIRE(inScore.parts.size() == 1);
        const auto &measure = inScore.parts.at(0).measures.at(0);
        REQUIRE(measure.staves.size() == 2);
        CHECK(measure.staves.at(1).voices.empty());
        REQUIRE(measure.staves.at(0).voices.size() == 1);
        const auto &notes = measure.staves.at(0).voices.begin()->second.notes;
        REQUIRE(notes.size() == 3);
        CHECK_EQUAL(0, notes.at(0).tickTimePosition);
        CHECK_EQUAL(2, notes.at(1).tickTimePosition);
        CHECK_EQUAL(3, notes.at(2).tickTimePosition);
        CHECK(!notes.at(0).crossStaffIndex.has_value());
        REQUIRE(notes.at(1).crossStaffIndex.has_value());
        CHECK_EQUAL(1, *notes.at(1).crossStaffIndex);
        CHECK(!notes.at(2).crossStaffIndex.has_value());
        CHECK(notes.at(0).beams == std::vector<Beam>{Beam::begin});
        CHECK(notes.at(1).beams == std::vector<Beam>{Beam::extend});
        CHECK(notes.at(2).beams == std::vector<Beam>{Beam::end});
    };

    const auto score = mxtest::fromXml(xml);
    check(score);
    check(mxtest::fromXml(mxtest::toXml(score)));
}

T_END;

#endif
