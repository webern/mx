// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Diagnostics.h"
#include "mx/api/MusicXml.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

#include <sstream>
#include <string>

using namespace std;
using namespace mx::api;
using namespace mxtest;

// A one-part score with one measure. partListExtra goes into <part-list> before the <score-part>
// and partListTail after it; measureBody follows the measure's <attributes>.
static std::string editorialScoreXml(const std::string &measureBody, const std::string &partListExtra = "",
                                     const std::string &partListTail = "")
{
    return std::string{R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<score-partwise version="4.0">
  <part-list>
)"} + partListExtra +
           R"(    <score-part id="P1">
      <part-name>Music</part-name>
    </score-part>
)" + partListTail +
           R"(  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <time>
          <beats>1</beats>
          <beat-type>4</beat-type>
        </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
)" + measureBody +
           R"(    </measure>
  </part>
</score-partwise>
)";
}

// A <footnote> that sets every attribute FootnoteData models, each to a value no other one shares.
static const std::string kEditorialFootnoteXml =
    R"(<footnote default-x="11" default-y="-22" relative-x="33" relative-y="-44" font-family="Times" )"
    R"(font-style="italic" font-size="9" font-weight="bold" color="#102030" halign="right" valign="top" )"
    R"(underline="1" overline="2" line-through="3" justify="center" enclosure="oval" xml:space="preserve">)"
    R"(m. 12: slur absent in the autograph</footnote>)";

// The FootnoteData that kEditorialFootnoteXml reads as.
static FootnoteData editorialExpectedFootnote()
{
    FootnoteData footnote;
    footnote.text = "m. 12: slur absent in the autograph";
    footnote.positionData.isDefaultXSpecified = true;
    footnote.positionData.defaultX = 11.0;
    footnote.positionData.isDefaultYSpecified = true;
    footnote.positionData.defaultY = -22.0;
    footnote.positionData.isRelativeXSpecified = true;
    footnote.positionData.relativeX = 33.0;
    footnote.positionData.isRelativeYSpecified = true;
    footnote.positionData.relativeY = -44.0;
    footnote.positionData.horizontalAlignment = HorizontalAlignment::right;
    footnote.positionData.verticalAlignment = VerticalAlignment::top;
    footnote.fontData.fontFamily = {"Times"};
    footnote.fontData.style = FontStyle::italic;
    footnote.fontData.sizeType = FontSizeType::point;
    footnote.fontData.sizePoint = 9.0;
    footnote.fontData.weight = FontWeight::bold;
    footnote.fontData.underline = 1;
    footnote.fontData.overline = 2;
    footnote.fontData.lineThrough = 3;
    ColorData color;
    color.red = 0x10;
    color.green = 0x20;
    color.blue = 0x30;
    footnote.color = color;
    footnote.justify = HorizontalAlignment::center;
    footnote.enclosure = Enclosure::oval;
    footnote.xmlSpace = XmlSpace::preserve;
    return footnote;
}

static LevelData editorialLevel(const std::string &text, Bool reference, LevelType type, Bool parentheses, Bool bracket,
                                SymbolSize size)
{
    LevelData level;
    level.text = text;
    level.reference = reference;
    level.type = type;
    level.parentheses = parentheses;
    level.bracket = bracket;
    level.size = size;
    return level;
}

static const MeasureData &editorialMeasure(const ScoreData &score)
{
    return score.parts.at(0).measures.at(0);
}

// Writes the score and reads it back, returning the XML written along the way.
static ScoreData editorialRoundTrip(const ScoreData &score, std::string &outXml)
{
    outXml = toXml(score);
    return fromXml(outXml);
}

// A rest with <level parentheses="yes"/>: the way MusicXML parenthesizes a rest, which does not
// have a notehead.
TEST(parenthesizedRest, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <note>
        <rest/>
        <duration>1</duration>
        <level parentheses="yes"/>
        <voice>1</voice>
        <type>quarter</type>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &note = editorialMeasure(score).staves.at(0).voices.at(0).notes.at(0);
    CHECK(note.isRest);
    REQUIRE(note.level.has_value());
    CHECK(*note.level == editorialLevel("", Bool::unspecified, LevelType::unspecified, Bool::yes, Bool::unspecified,
                                        SymbolSize::unspecified));
    CHECK(!note.footnote.has_value());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    CHECK(written.find("<level parentheses=\"yes\"") != std::string::npos);
    CHECK(out.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0) == note);
}

T_END;

// A note carrying a footnote, a level and a voice: all three share one MusicXML group, and writing
// one must keep the others.
TEST(noteLevelFootnoteAndVoice, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>1</duration>
        )" + kEditorialFootnoteXml + R"(
        <level reference="yes" type="start" parentheses="no" size="cue">2 (pencil)</level>
        <voice>1</voice>
        <type>quarter</type>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &note = editorialMeasure(score).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.level.has_value());
    CHECK(*note.level ==
          editorialLevel("2 (pencil)", Bool::yes, LevelType::start, Bool::no, Bool::unspecified, SymbolSize::cue));
    REQUIRE(note.footnote.has_value());
    CHECK(*note.footnote == editorialExpectedFootnote());
    CHECK_EQUAL(1, note.userRequestedVoiceNumber);

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    const auto footnotePos = written.find("<footnote");
    const auto levelPos = written.find("<level");
    const auto voicePos = written.find("<voice>1</voice>");
    REQUIRE(footnotePos != std::string::npos);
    REQUIRE(levelPos != std::string::npos);
    REQUIRE(voicePos != std::string::npos);
    CHECK(footnotePos < levelPos);
    CHECK(levelPos < voicePos);
    CHECK(out.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.at(0) == note);
}

T_END;

// The same three on a note authored through the api, in the second voice, so the writer has to
// state the voice.
TEST(authoredNoteLevelFootnoteAndVoice, EditorialApi)
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.name = "Music";
    part.measures.emplace_back();
    auto &staff = part.measures.back().staves.emplace_back();
    for (int voiceIndex = 0; voiceIndex < 2; ++voiceIndex)
    {
        NoteData note;
        note.pitchData.step = voiceIndex == 0 ? Step::e : Step::c;
        note.pitchData.octave = 4;
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 1;
        staff.voices[voiceIndex].notes.push_back(note);
    }
    auto &note = staff.voices[1].notes.back();
    note.level =
        editorialLevel("editor", Bool::no, LevelType::stop, Bool::unspecified, Bool::yes, SymbolSize::graceCue);
    note.footnote = editorialExpectedFootnote();

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    CHECK(written.find("<voice>2</voice>") != std::string::npos);
    const auto &onote = editorialMeasure(out).staves.at(0).voices.at(1).notes.at(0);
    REQUIRE(onote.level.has_value());
    CHECK(*onote.level == *note.level);
    REQUIRE(onote.footnote.has_value());
    CHECK(*onote.footnote == *note.footnote);
    CHECK(!editorialMeasure(out).staves.at(0).voices.at(0).notes.at(0).level.has_value());
}

T_END;

TEST(lyric, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <note>
        <pitch>
          <step>D</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
        <lyric number="1">
          <syllabic>single</syllabic>
          <text>la</text>
          )" + kEditorialFootnoteXml + R"(
          <level reference="no" type="single" bracket="yes" size="large">lyric layer</level>
        </lyric>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &note = editorialMeasure(score).staves.at(0).voices.at(0).notes.at(0);
    REQUIRE(note.lyrics.size() == 1);
    const auto &lyric = note.lyrics.front();
    REQUIRE(lyric.level.has_value());
    CHECK(*lyric.level ==
          editorialLevel("lyric layer", Bool::no, LevelType::single, Bool::unspecified, Bool::yes, SymbolSize::large));
    REQUIRE(lyric.footnote.has_value());
    CHECK(*lyric.footnote == editorialExpectedFootnote());
    CHECK(!note.level.has_value());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    CHECK(editorialMeasure(out).staves.at(0).voices.at(0).notes.at(0).lyrics == note.lyrics);
}

T_END;

TEST(barline, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <note>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
      <barline location="right">
        <bar-style>light-heavy</bar-style>
        )" + kEditorialFootnoteXml + R"(
        <level type="stop" parentheses="yes" bracket="no" size="full">barline layer</level>
        <repeat direction="backward"/>
      </barline>
)");
    const auto score = fromXml(xml);
    const auto &measure = editorialMeasure(score);
    REQUIRE(measure.barlines.size() == 1);
    const auto &barline = measure.barlines.front();
    REQUIRE(barline.level.has_value());
    CHECK(*barline.level ==
          editorialLevel("barline layer", Bool::unspecified, LevelType::stop, Bool::yes, Bool::no, SymbolSize::full));
    REQUIRE(barline.footnote.has_value());
    CHECK(*barline.footnote == editorialExpectedFootnote());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    CHECK(editorialMeasure(out).barlines == measure.barlines);
}

T_END;

// A level and footnote on the whole <figured-bass>, and different ones on one of its <figure>s.
TEST(figuredBassAndFigure, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <figured-bass>
        <figure>
          <figure-number>6</figure-number>
          <level reference="yes" type="single" bracket="no">figure layer</level>
        </figure>
        <figure>
          <figure-number>4</figure-number>
        </figure>
        )" + kEditorialFootnoteXml + R"(
        <level type="start" parentheses="no" bracket="yes" size="cue">bass layer</level>
      </figured-bass>
      <note>
        <pitch>
          <step>F</step>
          <octave>3</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &directions = editorialMeasure(score).staves.at(0).directions;
    REQUIRE(directions.size() == 1);
    const auto &direction = directions.front();
    CHECK(!direction.level.has_value());
    CHECK(!direction.footnote.has_value());
    REQUIRE(direction.figuredBasses.size() == 1);
    const auto &figuredBass = direction.figuredBasses.front();
    REQUIRE(figuredBass.level.has_value());
    CHECK(*figuredBass.level ==
          editorialLevel("bass layer", Bool::unspecified, LevelType::start, Bool::no, Bool::yes, SymbolSize::cue));
    REQUIRE(figuredBass.footnote.has_value());
    CHECK(*figuredBass.footnote == editorialExpectedFootnote());
    REQUIRE(figuredBass.figures.size() == 2);
    const auto &figure = figuredBass.figures.at(0);
    REQUIRE(figure.level.has_value());
    CHECK(*figure.level == editorialLevel("figure layer", Bool::yes, LevelType::single, Bool::unspecified, Bool::no,
                                          SymbolSize::unspecified));
    CHECK(!figure.footnote.has_value());
    CHECK(!figuredBass.figures.at(1).level.has_value());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    const auto &odirections = editorialMeasure(out).staves.at(0).directions;
    REQUIRE(odirections.size() == 1);
    CHECK(odirections.front().figuredBasses == direction.figuredBasses);
}

T_END;

// A <direction> carrying a footnote, a level and a voice.
TEST(directionLevelFootnoteAndVoice, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <direction placement="below">
        <direction-type>
          <dynamics>
            <f/>
          </dynamics>
        </direction-type>
        )" + kEditorialFootnoteXml + R"(
        <level reference="no" type="stop" parentheses="yes" size="large">direction layer</level>
        <voice>1</voice>
        <staff>1</staff>
      </direction>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>quarter</type>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &directions = editorialMeasure(score).staves.at(0).directions;
    REQUIRE(directions.size() == 1);
    const auto &direction = directions.front();
    REQUIRE(direction.level.has_value());
    CHECK(*direction.level == editorialLevel("direction layer", Bool::no, LevelType::stop, Bool::yes, Bool::unspecified,
                                             SymbolSize::large));
    REQUIRE(direction.footnote.has_value());
    CHECK(*direction.footnote == editorialExpectedFootnote());
    CHECK_EQUAL(1, direction.voice);

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    const auto footnotePos = written.find("<footnote");
    const auto levelPos = written.find("<level");
    const auto voicePos = written.find("<voice>1</voice>", levelPos);
    REQUIRE(footnotePos != std::string::npos);
    REQUIRE(levelPos != std::string::npos);
    REQUIRE(voicePos != std::string::npos);
    CHECK(footnotePos < levelPos);
    CHECK(voicePos < written.find("<staff>1</staff>", levelPos));
    const auto &odirections = editorialMeasure(out).staves.at(0).directions;
    REQUIRE(odirections.size() == 1);
    CHECK(odirections.front() == direction);
}

T_END;

TEST(harmony, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <harmony>
        <root>
          <root-step>C</root-step>
        </root>
        <kind>major</kind>
        )" + kEditorialFootnoteXml + R"(
        <level reference="yes" type="stop" bracket="no" size="grace-cue">chord layer</level>
      </harmony>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
)");
    const auto score = fromXml(xml);
    const auto &directions = editorialMeasure(score).staves.at(0).directions;
    REQUIRE(directions.size() == 1);
    const auto &direction = directions.front();
    CHECK(!direction.level.has_value());
    REQUIRE(direction.chords.size() == 1);
    const auto &chord = direction.chords.front();
    REQUIRE(chord.level.has_value());
    CHECK(*chord.level ==
          editorialLevel("chord layer", Bool::yes, LevelType::stop, Bool::unspecified, Bool::no, SymbolSize::graceCue));
    REQUIRE(chord.footnote.has_value());
    CHECK(*chord.footnote == editorialExpectedFootnote());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    const auto &odirections = editorialMeasure(out).staves.at(0).directions;
    REQUIRE(odirections.size() == 1);
    CHECK(odirections.front().chords == direction.chords);
}

T_END;

// The chords of one direction become one <harmony>, which has room for one level and one footnote.
// Each comes from the first chord that has one, and reads back onto every chord.
TEST(harmonyFirstChordRule, EditorialApi)
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.name = "Music";
    part.measures.emplace_back();
    auto &staff = part.measures.back().staves.emplace_back();
    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    staff.voices[0].notes.push_back(note);

    DirectionData direction;
    direction.chords.resize(3);
    direction.chords.at(0).root = Step::c;
    direction.chords.at(0).chordKind = ChordKind::major;
    direction.chords.at(0).footnote = editorialExpectedFootnote();
    direction.chords.at(1).root = Step::f;
    direction.chords.at(1).chordKind = ChordKind::major;
    direction.chords.at(1).level =
        editorialLevel("second", Bool::yes, LevelType::start, Bool::no, Bool::unspecified, SymbolSize::cue);
    direction.chords.at(2).root = Step::g;
    direction.chords.at(2).chordKind = ChordKind::dominant;
    direction.chords.at(2).level =
        editorialLevel("third", Bool::no, LevelType::stop, Bool::yes, Bool::yes, SymbolSize::large);
    FootnoteData thirdFootnote;
    thirdFootnote.text = "third";
    direction.chords.at(2).footnote = thirdFootnote;
    staff.directions.push_back(direction);

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    CHECK(written.find("second") != std::string::npos);
    CHECK(written.find(">third<") == std::string::npos);
    const auto &odirections = editorialMeasure(out).staves.at(0).directions;
    REQUIRE(odirections.size() == 1);
    REQUIRE(odirections.front().chords.size() == 3);
    for (const auto &chord : odirections.front().chords)
    {
        REQUIRE(chord.level.has_value());
        CHECK(*chord.level == *direction.chords.at(1).level);
        REQUIRE(chord.footnote.has_value());
        CHECK(*chord.footnote == editorialExpectedFootnote());
    }
}

T_END;

// The level and footnote of a part group are on its start element.
TEST(partGroup, EditorialApi)
{
    const auto xml = editorialScoreXml(R"(      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>1</duration>
        <type>quarter</type>
      </note>
)",
                                       R"(    <part-group type="start" number="1">
      <group-name>Winds</group-name>
      <group-symbol>bracket</group-symbol>
      )" + kEditorialFootnoteXml + R"(
      <level type="start" parentheses="yes" bracket="no">group layer</level>
    </part-group>
)",
                                       R"(    <part-group type="stop" number="1"/>
)");
    const auto score = fromXml(xml);
    REQUIRE(score.partGroups.size() == 1);
    const auto &group = score.partGroups.front();
    REQUIRE(group.level.has_value());
    CHECK(*group.level == editorialLevel("group layer", Bool::unspecified, LevelType::start, Bool::yes, Bool::no,
                                         SymbolSize::unspecified));
    REQUIRE(group.footnote.has_value());
    CHECK(*group.footnote == editorialExpectedFootnote());

    std::string written;
    const auto out = editorialRoundTrip(score, written);
    REQUIRE(out.partGroups.size() == 1);
    CHECK(out.partGroups.front() == group);
    const auto stopPos = written.find("<part-group type=\"stop\"");
    REQUIRE(stopPos != std::string::npos);
    CHECK(written.find("<level", stopPos) == std::string::npos);
    CHECK(written.find("<footnote", stopPos) == std::string::npos);
}

T_END;

// A direction whose only content is a level has no <direction> element to hold it.
TEST(directionWithoutDirectionTypesDropsLevel, EditorialApi)
{
    ScoreData score;
    score.ticksPerQuarter = 1;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.uniqueId = "P1";
    part.name = "Music";
    part.measures.emplace_back();
    auto &staff = part.measures.back().staves.emplace_back();
    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 1;
    staff.voices[0].notes.push_back(note);
    DirectionData direction;
    direction.level = editorialLevel("lost", Bool::yes, LevelType::single, Bool::no, Bool::yes, SymbolSize::full);
    staff.directions.push_back(direction);

    Diagnostics diagnostics;
    const auto result = fromScore(score, diagnostics);
    REQUIRE(result.ok());
    REQUIRE(diagnostics.all().size() == 1);
    const auto &diagnostic = diagnostics.all().front();
    CHECK(DiagnosticCode::droppedData == diagnostic.code);
    CHECK(Severity::error == diagnostic.severity);
    CHECK_EQUAL("a direction level or footnote is not written; the direction does not have any direction types",
                diagnostic.message);

    std::ostringstream xml;
    REQUIRE(result.value().writeToStream(xml).ok());
    CHECK(xml.str().find("<level") == std::string::npos);
}

T_END;

#endif
