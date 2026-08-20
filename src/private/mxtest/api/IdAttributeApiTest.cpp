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

#include <map>
#include <unordered_map>

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

// The text of an id, or an empty string when there is none.
static std::string idAttributeText(const std::optional<Id> &id)
{
    return id.has_value() ? id->value() : std::string{};
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
    CHECK_EQUAL("m1", idAttributeText(idAttributeMeasure(out).id));

    const auto &onote = idAttributeNote(out);
    CHECK_EQUAL("n1", idAttributeText(onote.id));
    REQUIRE(onote.lyrics.size() == 1);
    CHECK_EQUAL("ly1", idAttributeText(onote.lyrics.front().id));
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

    CHECK_EQUAL("t1", idAttributeText(omeasure.timeSignature.id));
    REQUIRE(omeasure.keys.size() == 1);
    CHECK_EQUAL("k1", idAttributeText(omeasure.keys.front().id));
    REQUIRE(omeasure.staves.back().clefs.size() == 1);
    CHECK_EQUAL("c1", idAttributeText(omeasure.staves.back().clefs.front().id));
    REQUIRE(omeasure.transpositions.size() == 1);
    CHECK_EQUAL("x1", idAttributeText(omeasure.transpositions.front().id));
    REQUIRE(omeasure.barlines.size() == 1);
    CHECK_EQUAL("b1", idAttributeText(omeasure.barlines.front().id));
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
    CHECK_EQUAL("t1", idAttributeText(out.parts.back().measures.front().timeSignature.id));
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

    CHECK_EQUAL("d1", idAttributeText(odirection.id));
    CHECK_EQUAL("so1", idAttributeText(odirection.soundData.id));

    std::optional<Id> foundWords;
    std::optional<Id> foundSymbol;
    std::optional<Id> foundRehearsal;
    std::optional<Id> foundWedge;
    std::optional<Id> foundBracket;
    std::optional<Id> foundDashes;
    std::optional<Id> foundOttava;
    std::optional<Id> foundPedal;

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

    CHECK_EQUAL("w1", idAttributeText(foundWords));
    CHECK_EQUAL("sy1", idAttributeText(foundSymbol));
    CHECK_EQUAL("r1", idAttributeText(foundRehearsal));
    CHECK_EQUAL("we1", idAttributeText(foundWedge));
    CHECK_EQUAL("br1", idAttributeText(foundBracket));
    CHECK_EQUAL("da1", idAttributeText(foundDashes));
    CHECK_EQUAL("o1", idAttributeText(foundOttava));
    CHECK_EQUAL("p1", idAttributeText(foundPedal));
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
    CHECK_EQUAL("s1", idAttributeText(onotes.front().noteAttachmentData.curveStarts.front().id));
    REQUIRE(onotes.back().noteAttachmentData.curveStops.size() == 1);
    CHECK_EQUAL("s2", idAttributeText(onotes.back().noteAttachmentData.curveStops.front().id));

    REQUIRE(onotes.front().noteAttachmentData.tupletStarts.size() == 1);
    CHECK_EQUAL("tu1", idAttributeText(onotes.front().noteAttachmentData.tupletStarts.front().id));
    REQUIRE(onotes.back().noteAttachmentData.tupletStops.size() == 1);
    CHECK_EQUAL("tu2", idAttributeText(onotes.back().noteAttachmentData.tupletStops.front().id));

    REQUIRE(onotes.back().tieLetRing.has_value());
    CHECK_EQUAL("lr1", idAttributeText(onotes.back().tieLetRing->id));
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
    CHECK_EQUAL("fb1", idAttributeText(odirections.front().figuredBasses.front().id));
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

    CHECK_EQUAL("m1", idAttributeText(measure.id));
    CHECK_EQUAL("t1", idAttributeText(measure.timeSignature.id));
    REQUIRE(measure.keys.size() == 1);
    CHECK_EQUAL("k1", idAttributeText(measure.keys.front().id));
    REQUIRE(measure.staves.front().clefs.size() == 1);
    CHECK_EQUAL("c1", idAttributeText(measure.staves.front().clefs.front().id));
    REQUIRE(measure.barlines.size() == 1);
    CHECK_EQUAL("b1", idAttributeText(measure.barlines.front().id));

    const auto &notes = measure.staves.front().voices.at(0).notes;
    REQUIRE(notes.size() == 1);
    CHECK_EQUAL("n1", idAttributeText(notes.front().id));
}

T_END;

// An id that breaks the XML name rules is scrubbed as the Id is built, so the api and the file
// always hold the same text.
TEST(idsAreScrubbedWhenBuilt, IdAttribute)
{
    CHECK_EQUAL("n1", Id{"n1"}.value());
    CHECK_EQUAL("measure-1.a_b", Id{"measure-1.a_b"}.value());

    // Characters the rules do not allow are dropped.
    CHECK_EQUAL("blindmice", Id{"3 blind mice"}.value());

    // An id cannot start with a digit, a hyphen, or a dot.
    CHECK_EQUAL("abc", Id{"1abc"}.value());
    CHECK_EQUAL("abc", Id{"-abc"}.value());

    // Nothing usable left, so the id becomes "X".
    CHECK_EQUAL("X", Id{""}.value());
    CHECK_EQUAL("X", Id{"!!!"}.value());
}

T_END;

// Compare the id you built with the text you gave it to find out whether the text was legal.
TEST(scrubbingIsVisibleToTheCaller, IdAttribute)
{
    const std::string wanted = "3 blind mice";
    const auto id = Id{wanted};
    CHECK(id.value() != wanted);

    const std::string legal = "n1";
    CHECK(Id{legal}.value() == legal);
}

T_END;

// A scrubbed id reaches the file scrubbed, and reads back the same way.
TEST(scrubbedIdRoundTrips, IdAttribute)
{
    auto score = idAttributeMakeScore();
    idAttributeNote(score).id = "3 blind mice";

    const auto xml = toXml(score);
    CHECK(xml.find("id=\"blindmice\"") != std::string::npos);

    const auto out = roundTrip(score);
    CHECK_EQUAL("blindmice", idAttributeText(idAttributeNote(out).id));
}

T_END;

// Id behaves like any other value: copy it, assign it, compare it, sort it, hash it.
TEST(idBehavesLikeAValue, IdAttribute)
{
    const Id first{"n1"};
    const Id copy = first;
    CHECK(first == copy);
    CHECK(!(first != copy));

    Id assigned{"other"};
    assigned = first;
    CHECK(first == assigned);
    CHECK_EQUAL("n1", assigned.value());

    // Assigning does not tie the two together.
    assigned = Id{"n2"};
    CHECK_EQUAL("n1", first.value());
    CHECK(first != assigned);

    // Equality is by text, not by where the id came from.
    CHECK(Id{"n1"} == Id{"n1"});
    CHECK(Id{"n1"} != Id{"n2"});

    // Ordering is by text.
    CHECK(Id{"a"} < Id{"b"});
    CHECK(Id{"b"} > Id{"a"});
    CHECK(Id{"a"} <= Id{"a"});
    CHECK(Id{"a"} >= Id{"a"});

    // Equal ids hash the same.
    const std::hash<Id> hasher;
    CHECK(hasher(Id{"n1"}) == hasher(copy));

    // So an Id works as a key in either kind of map.
    std::map<Id, int> ordered;
    ordered[Id{"n2"}] = 2;
    ordered[Id{"n1"}] = 1;
    REQUIRE(ordered.size() == 2);
    CHECK_EQUAL("n1", ordered.begin()->first.value());
    CHECK_EQUAL(1, ordered.at(Id{"n1"}));

    std::unordered_map<Id, int> hashed;
    hashed[Id{"n1"}] = 1;
    CHECK_EQUAL(1, hashed.at(Id{"n1"}));

    // A moved-from Id is still readable.
    Id source{"n3"};
    const Id moved = std::move(source);
    CHECK_EQUAL("n3", moved.value());
    CHECK(!source.value().empty());
}

T_END;

#endif
