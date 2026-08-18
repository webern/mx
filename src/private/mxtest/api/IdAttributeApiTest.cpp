// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

// A one-part, one-measure score holding a single quarter note, ready for a test to hang ids on.
static ScoreData idAttributeMakeScore()
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.name = "P1";
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();

    NoteData note{};
    note.pitchData.step = Step::c;
    note.pitchData.octave = 4;
    note.durationData.durationTimeTicks = 4;
    note.durationData.durationName = DurationName::quarter;
    note.tickTimePosition = 0;
    staff.voices[0].notes.push_back(note);

    return score;
}

static MeasureData &idAttributeMeasure(ScoreData &score)
{
    return score.parts.back().measures.back();
}

static const MeasureData &idAttributeMeasure(const ScoreData &score)
{
    return score.parts.back().measures.back();
}

static NoteData &idAttributeNote(ScoreData &score)
{
    return idAttributeMeasure(score).staves.back().voices.at(0).notes.back();
}

static const NoteData &idAttributeNote(const ScoreData &score)
{
    return idAttributeMeasure(score).staves.back().voices.at(0).notes.back();
}

// The <note> id, the reason for issue 399, plus the <measure> and <lyric> ids around it.
TEST(noteMeasureAndLyricIds, IdAttribute)
{
    auto score = idAttributeMakeScore();
    idAttributeMeasure(score).id = "m1";

    auto &note = idAttributeNote(score);
    note.id = "n1";

    LyricData lyric;
    lyric.text = "la";
    lyric.id = "ly1";
    note.lyrics.push_back(lyric);

    const auto xml = toXml(score);
    CHECK(xml.find("id=\"n1\"") != std::string::npos);

    const auto out = roundTrip(score);
    CHECK(std::optional<std::string>{"m1"} == idAttributeMeasure(out).id);

    const auto &onote = idAttributeNote(out);
    CHECK(std::optional<std::string>{"n1"} == onote.id);
    REQUIRE(onote.lyrics.size() == 1);
    CHECK(std::optional<std::string>{"ly1"} == onote.lyrics.front().id);
}

T_END;

// The ids on the elements that live in <attributes>, plus the <barline> id.
TEST(attributesAndBarlineIds, IdAttribute)
{
    auto score = idAttributeMakeScore();
    auto &measure = idAttributeMeasure(score);

    measure.timeSignature = TimeChoice{TimeSignatureData{}};
    measure.timeSignature.isImplicit = false;
    measure.timeSignature.id = "t1";

    KeyData key;
    key.fifths = 2;
    key.mode = KeyMode::major;
    key.id = "k1";
    measure.keys.push_back(key);

    ClefData clef;
    clef.setTreble();
    clef.id = "c1";
    measure.staves.back().clefs.push_back(clef);

    TransposeData transpose{-2, -1};
    transpose.id = "x1";
    measure.transpositions.push_back(transpose);

    BarlineData barline;
    barline.barlineType = BarlineType::lightHeavy;
    barline.location = HorizontalAlignment::right;
    barline.id = "b1";
    measure.barlines.push_back(barline);

    const auto out = roundTrip(score);
    const auto &omeasure = idAttributeMeasure(out);

    CHECK(std::optional<std::string>{"t1"} == omeasure.timeSignature.id);
    REQUIRE(omeasure.keys.size() == 1);
    CHECK(std::optional<std::string>{"k1"} == omeasure.keys.front().id);
    REQUIRE(omeasure.staves.back().clefs.size() == 1);
    CHECK(std::optional<std::string>{"c1"} == omeasure.staves.back().clefs.front().id);
    REQUIRE(omeasure.transpositions.size() == 1);
    CHECK(std::optional<std::string>{"x1"} == omeasure.transpositions.front().id);
    REQUIRE(omeasure.barlines.size() == 1);
    CHECK(std::optional<std::string>{"b1"} == omeasure.barlines.front().id);
}

T_END;

// A time signature that a later measure inherits carries no id: only the measure that states the
// <time> element owns it.
TEST(inheritedTimeSignatureHasNoId, IdAttribute)
{
    auto score = idAttributeMakeScore();
    auto &measure = idAttributeMeasure(score);
    measure.timeSignature = TimeChoice{TimeSignatureData{}};
    measure.timeSignature.isImplicit = false;
    measure.timeSignature.id = "t1";

    auto &part = score.parts.back();
    part.measures.push_back(part.measures.back());
    part.measures.back().timeSignature.isImplicit = true;
    part.measures.back().id = std::nullopt;

    const auto out = roundTrip(score);
    REQUIRE(out.parts.back().measures.size() == 2);
    CHECK(std::optional<std::string>{"t1"} == out.parts.back().measures.front().timeSignature.id);
    CHECK(!out.parts.back().measures.back().timeSignature.id.has_value());
}

T_END;

// The <direction> id and the ids of the direction-type elements mx::api models as their own types.
TEST(directionIds, IdAttribute)
{
    auto score = idAttributeMakeScore();
    auto &staff = idAttributeMeasure(score).staves.back();

    DirectionData direction;
    direction.tickTimePosition = 0;
    direction.id = "d1";

    WordsData words;
    words.text = "dolce";
    words.id = "w1";

    SymbolData symbol;
    symbol.smufl = "arrowBlackUp";
    symbol.id = "sy1";

    direction.directionTypes.emplace_back(
        DirectionChoice{std::vector<WordsChoice>{WordsChoice{words}, WordsChoice{symbol}}});

    RehearsalData rehearsal;
    rehearsal.text = "A";
    rehearsal.id = "r1";
    direction.directionTypes.emplace_back(DirectionChoice{rehearsal});

    WedgeStart wedgeStart;
    wedgeStart.wedgeType = WedgeType::crescendo;
    wedgeStart.id = "we1";
    direction.directionTypes.emplace_back(DirectionChoice{wedgeStart});

    SpannerStart bracketStart;
    bracketStart.id = "br1";
    direction.directionTypes.emplace_back(DirectionChoice::bracketStart(bracketStart));

    SpannerStart dashesStart;
    dashesStart.id = "da1";
    direction.directionTypes.emplace_back(DirectionChoice::dashesStart(dashesStart));

    OttavaStart ottavaStart;
    ottavaStart.ottavaType = OttavaType::o8va;
    ottavaStart.spannerStart.id = "o1";
    direction.directionTypes.emplace_back(DirectionChoice{ottavaStart});

    PedalLineData pedal{PedalLineKind::start};
    pedal.id = "p1";
    direction.directionTypes.emplace_back(DirectionChoice{pedal});

    direction.isSoundDataSpecified = true;
    direction.soundData.tempo = 120.0;
    direction.soundData.id = "so1";

    staff.directions.push_back(direction);

    const auto out = roundTrip(score);
    const auto &odirections = idAttributeMeasure(out).staves.back().directions;
    REQUIRE(odirections.size() == 1);
    const auto &odirection = odirections.front();

    CHECK(std::optional<std::string>{"d1"} == odirection.id);
    CHECK(std::optional<std::string>{"so1"} == odirection.soundData.id);

    std::optional<std::string> foundWords;
    std::optional<std::string> foundSymbol;
    std::optional<std::string> foundRehearsal;
    std::optional<std::string> foundWedge;
    std::optional<std::string> foundBracket;
    std::optional<std::string> foundDashes;
    std::optional<std::string> foundOttava;
    std::optional<std::string> foundPedal;

    for (const auto &choice : odirection.directionTypes)
    {
        switch (choice.kind())
        {
        case DirectionChoice::Kind::wordsRun:
            for (const auto &item : choice.wordsRun())
            {
                if (item.isWords())
                {
                    foundWords = item.words().id;
                }
                else if (item.isSymbol())
                {
                    foundSymbol = item.symbol().id;
                }
            }
            break;
        case DirectionChoice::Kind::rehearsal:
            foundRehearsal = choice.rehearsal().id;
            break;
        case DirectionChoice::Kind::wedgeStart:
            foundWedge = choice.wedgeStart().id;
            break;
        case DirectionChoice::Kind::bracketStart:
            foundBracket = choice.bracketStart().id;
            break;
        case DirectionChoice::Kind::dashesStart:
            foundDashes = choice.dashesStart().id;
            break;
        case DirectionChoice::Kind::ottavaStart:
            foundOttava = choice.ottavaStart().spannerStart.id;
            break;
        case DirectionChoice::Kind::pedal:
            foundPedal = choice.pedal().id;
            break;
        default:
            break;
        }
    }

    CHECK(std::optional<std::string>{"w1"} == foundWords);
    CHECK(std::optional<std::string>{"sy1"} == foundSymbol);
    CHECK(std::optional<std::string>{"r1"} == foundRehearsal);
    CHECK(std::optional<std::string>{"we1"} == foundWedge);
    CHECK(std::optional<std::string>{"br1"} == foundBracket);
    CHECK(std::optional<std::string>{"da1"} == foundDashes);
    CHECK(std::optional<std::string>{"o1"} == foundOttava);
    CHECK(std::optional<std::string>{"p1"} == foundPedal);
}

T_END;

// The ids on the note-attached elements: <slur>, <tuplet> and a let-ring <tied>.
TEST(notationIds, IdAttribute)
{
    auto score = idAttributeMakeScore();
    auto &staff = idAttributeMeasure(score).staves.back();
    auto &notes = staff.voices.at(0).notes;

    NoteData second = notes.back();
    second.tickTimePosition = 4;
    notes.push_back(second);

    CurveStart slurStart{CurveType::slur};
    slurStart.id = "s1";
    notes.front().noteAttachmentData.curveStarts.push_back(slurStart);

    CurveStop slurStop{CurveType::slur};
    slurStop.id = "s2";
    notes.back().noteAttachmentData.curveStops.push_back(slurStop);

    TupletStart tupletStart;
    tupletStart.actualNumber = 3;
    tupletStart.actualDurationName = DurationName::quarter;
    tupletStart.normalNumber = 2;
    tupletStart.normalDurationName = DurationName::quarter;
    tupletStart.id = "tu1";
    notes.front().noteAttachmentData.tupletStarts.push_back(tupletStart);

    TupletStop tupletStop;
    tupletStop.id = "tu2";
    notes.back().noteAttachmentData.tupletStops.push_back(tupletStop);

    TieLetRing letRing;
    letRing.id = "lr1";
    notes.back().tieLetRing = letRing;

    const auto out = roundTrip(score);
    const auto &onotes = idAttributeMeasure(out).staves.back().voices.at(0).notes;
    REQUIRE(onotes.size() == 2);

    REQUIRE(onotes.front().noteAttachmentData.curveStarts.size() == 1);
    CHECK(std::optional<std::string>{"s1"} == onotes.front().noteAttachmentData.curveStarts.front().id);
    REQUIRE(onotes.back().noteAttachmentData.curveStops.size() == 1);
    CHECK(std::optional<std::string>{"s2"} == onotes.back().noteAttachmentData.curveStops.front().id);

    REQUIRE(onotes.front().noteAttachmentData.tupletStarts.size() == 1);
    CHECK(std::optional<std::string>{"tu1"} == onotes.front().noteAttachmentData.tupletStarts.front().id);
    REQUIRE(onotes.back().noteAttachmentData.tupletStops.size() == 1);
    CHECK(std::optional<std::string>{"tu2"} == onotes.back().noteAttachmentData.tupletStops.front().id);

    REQUIRE(onotes.back().tieLetRing.has_value());
    CHECK(std::optional<std::string>{"lr1"} == onotes.back().tieLetRing->id);
}

T_END;

// The <figured-bass> id.
TEST(figuredBassId, IdAttribute)
{
    auto score = idAttributeMakeScore();
    auto &staff = idAttributeMeasure(score).staves.back();

    FigureData figure;
    figure.figureNumber = "6";

    FiguredBassData figuredBass;
    figuredBass.figures.push_back(figure);
    figuredBass.id = "fb1";

    DirectionData direction;
    direction.tickTimePosition = 0;
    direction.placement = Placement::below;
    direction.figuredBasses.push_back(figuredBass);
    staff.directions.push_back(direction);

    const auto out = roundTrip(score);
    const auto &odirections = idAttributeMeasure(out).staves.back().directions;
    REQUIRE(odirections.size() == 1);
    REQUIRE(odirections.front().figuredBasses.size() == 1);
    CHECK(std::optional<std::string>{"fb1"} == odirections.front().figuredBasses.front().id);
}

T_END;

// Reading ids straight off a MusicXML source.
TEST(idsAreReadFromSourceXml, IdAttribute)
{
    const std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 4.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="4.0">
  <part-list>
    <score-part id="P1">
      <part-name>Music</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1" id="m1">
      <attributes>
        <divisions>1</divisions>
        <key id="k1">
          <fifths>0</fifths>
        </key>
        <time id="t1">
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <clef id="c1">
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note id="n1">
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <type>whole</type>
      </note>
      <barline location="right" id="b1">
        <bar-style>light-heavy</bar-style>
      </barline>
    </measure>
  </part>
</score-partwise>)";

    const auto score = fromXml(xml);
    REQUIRE(score.parts.size() == 1);
    const auto &measure = score.parts.front().measures.front();

    CHECK(std::optional<std::string>{"m1"} == measure.id);
    CHECK(std::optional<std::string>{"t1"} == measure.timeSignature.id);
    REQUIRE(measure.keys.size() == 1);
    CHECK(std::optional<std::string>{"k1"} == measure.keys.front().id);
    REQUIRE(measure.staves.front().clefs.size() == 1);
    CHECK(std::optional<std::string>{"c1"} == measure.staves.front().clefs.front().id);
    REQUIRE(measure.barlines.size() == 1);
    CHECK(std::optional<std::string>{"b1"} == measure.barlines.front().id);

    const auto &notes = measure.staves.front().voices.at(0).notes;
    REQUIRE(notes.size() == 1);
    CHECK(std::optional<std::string>{"n1"} == notes.front().id);
}

T_END;

#endif
