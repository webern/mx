// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mx/api/OttavaData.h"
#include "mx/api/RehearsalData.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"
#include "mxtest/file/Path.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST(OutOfOrderDoesntThrow, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];
    NoteData onote{};

    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    onote.tickTimePosition = 10;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    onote.tickTimePosition = 20;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    onote.tickTimePosition = 30;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    DirectionData directionData{};
    MarkData mark{StandardDynamic::f};

    int tickTime = 10;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.directionTypes.assign(1, DirectionChoice{mark});
    ostaff.directions.push_back(directionData);

    tickTime = 9;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.directionTypes.assign(1, DirectionChoice{mark});
    ostaff.directions.push_back(directionData);

    tickTime = 8;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.directionTypes.assign(1, DirectionChoice{mark});
    ostaff.directions.push_back(directionData);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rpart = rscore.parts.back();
    const auto &rmeasure = rpart.measures.back();
    const auto &rstaff = rmeasure.staves.back();
    const auto &rdirections = rstaff.directions;

    // tickTimePosition is the anchor and offset is the drawn-position shift, so the direction's
    // effective (drawn) location is their sum. A mark authored between note ticks anchors to the
    // preceding note and carries the remainder as an offset.
    const auto effectiveTick = [](const DirectionData &d) {
        return d.tickTimePosition + (d.offset.has_value() ? *d.offset : 0);
    };

    CHECK_EQUAL(3, rdirections.size());
    auto rdirection = rdirections.cbegin();
    CHECK_EQUAL(8, effectiveTick(*rdirection));
    ++rdirection;
    CHECK_EQUAL(9, effectiveTick(*rdirection));
    ++rdirection;
    CHECK_EQUAL(10, effectiveTick(*rdirection));
}

T_END;

TEST(Ottava22RoundTripXml, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &staff = measure.staves.emplace_back();
    auto &note = staff.voices[0].notes.emplace_back();
    note.durationData.durationTimeTicks = 10;
    note.durationData.durationName = DurationName::quarter;

    OttavaStart up;
    up.ottavaType = OttavaType::o22ma;
    OttavaStart down;
    down.ottavaType = OttavaType::o22mb;

    DirectionData direction;
    direction.directionTypes.emplace_back(DirectionChoice{up});
    direction.directionTypes.emplace_back(DirectionChoice{down});
    staff.directions.emplace_back(direction);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<octave-shift type=\"down\" size=\"22\"") != std::string::npos);
    CHECK(xml.find("<octave-shift type=\"up\" size=\"22\"") != std::string::npos);

    const auto roundTripped = mxtest::roundTrip(score);
    const auto &directionTypes =
        roundTripped.parts.front().measures.front().staves.front().directions.front().directionTypes;
    REQUIRE(directionTypes.size() == 2);
    REQUIRE(directionTypes.front().isOttavaStart());
    REQUIRE(directionTypes.back().isOttavaStart());
    CHECK(OttavaType::o22ma == directionTypes.front().ottavaStart().ottavaType);
    CHECK(OttavaType::o22mb == directionTypes.back().ottavaStart().ottavaType);
}

T_END;

TEST(OutOfOrderTorture, DirectionData)
{
    const std::vector<int> randomNumbers = {-31, 25,  -9,    21,      0, 77,  -100, 100,  0,  31, 12, 6,
                                            19,  109, 10000, -100000, 0, 231, 652,  2345, 11, 11, 1,  11};

    const auto vecSize = randomNumbers.size();

    for (int i = 0; i < 100; ++i)
    {
        const int tick0 = randomNumbers.at(static_cast<size_t>(i + 0) % vecSize);
        const int tick1 = randomNumbers.at(static_cast<size_t>(i + 1) % vecSize);
        const int tick2 = randomNumbers.at(static_cast<size_t>(i + 2) % vecSize);
        const int tick3 = randomNumbers.at(static_cast<size_t>(i + 3) % vecSize);
        const int dur0tick = randomNumbers.at(static_cast<size_t>(i + 4) % vecSize);
        const int dur1tick = randomNumbers.at(static_cast<size_t>(i + 5) % vecSize);
        const int dur2tick = randomNumbers.at(static_cast<size_t>(i + 6) % vecSize);

        ScoreData oscore;
        oscore.ticksPerQuarter = 10;
        oscore.parts.emplace_back();
        auto &opart = oscore.parts.back();
        opart.measures.emplace_back();
        auto &omeasure = opart.measures.back();
        omeasure.staves.emplace_back();
        auto &ostaff = omeasure.staves.back();
        auto &ovoice = ostaff.voices[0];
        NoteData onote{};

        onote.tickTimePosition = tick0;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back(onote);

        onote.tickTimePosition = tick1;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back(onote);

        onote.tickTimePosition = tick2;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back(onote);

        onote.tickTimePosition = tick3;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back(onote);

        DirectionData directionData{};
        MarkData mark{StandardDynamic::f};

        mark.tickTimePosition = dur0tick;
        directionData.tickTimePosition = dur0tick;
        directionData.directionTypes.assign(1, DirectionChoice{mark});
        ostaff.directions.push_back(directionData);

        mark.tickTimePosition = dur1tick;
        directionData.tickTimePosition = dur1tick;
        directionData.directionTypes.assign(1, DirectionChoice{mark});
        ostaff.directions.push_back(directionData);

        mark.tickTimePosition = dur2tick;
        directionData.tickTimePosition = dur2tick;
        directionData.directionTypes.assign(1, DirectionChoice{mark});
        ostaff.directions.push_back(directionData);

        const auto rscore = mxtest::roundTrip(oscore);
        const auto &rpart = rscore.parts.back();
        const auto &rmeasure = rpart.measures.back();
        const auto &rstaff = rmeasure.staves.back();
        const auto &rdirections = rstaff.directions;

        std::vector<int> tempTickSorter = {dur0tick, dur1tick, dur2tick};
        std::sort(std::begin(tempTickSorter), std::end(tempTickSorter));

        int expectedShift = 0;

        if (tick0 < 0)
        {
            expectedShift = (-1 * tick0);
        }

        // tickTimePosition is the anchor and offset is the drawn-position shift; their sum is the
        // effective (drawn) location, which is what was authored on each mark.
        const auto effectiveTick = [](const DirectionData &d) {
            return d.tickTimePosition + (d.offset.has_value() ? *d.offset : 0);
        };

        CHECK_EQUAL(3, rdirections.size());
        auto rdirection = rdirections.cbegin();
        CHECK_EQUAL(tempTickSorter.at(0), effectiveTick(*rdirection));
        ++rdirection;
        CHECK_EQUAL(tempTickSorter.at(1), effectiveTick(*rdirection));
        ++rdirection;
        CHECK_EQUAL(tempTickSorter.at(2), effectiveTick(*rdirection));
    }
}

T_END;

// Parse the synthetic rehearsal file and confirm that mx::api reads the text and enclosure.
// This pins the core -> api read path and would fail if parseRehearsal regresses.
TEST(RehearsalSyntheticFileRead, DirectionData)
{
    const std::string path = mxtest::getResourcesDirectoryPath() + "synthetic/rehearsal.3.1.xml";
    auto docIdResult = MusicXml::fromFile(path);
    REQUIRE(docIdResult.ok());
    const auto scoreResult = getScore(std::move(docIdResult).value());
    REQUIRE(scoreResult.ok());
    const auto &score = scoreResult.value();
    REQUIRE(score.parts.size() == 1);
    REQUIRE(score.parts.front().measures.size() == 1);
    REQUIRE(score.parts.front().measures.front().staves.size() == 1);
    const auto &directions = score.parts.front().measures.front().staves.front().directions;
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().directionTypes.size() == 1);
    REQUIRE(directions.front().directionTypes.front().isRehearsal());
    const auto rehearsal = directions.front().directionTypes.front().rehearsal();
    CHECK_EQUAL("x", rehearsal.text);
    CHECK(Enclosure::rectangle == rehearsal.enclosure);
}

T_END;

// Verify that rehearsal marks survive a full MusicXML serialization/deserialization round trip.
// This catches the bug where DirectionWriter had no rehearsal write path and rehearsals were
// silently dropped on output.
TEST(RehearsalRoundTripXml, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];

    NoteData onote{};
    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    RehearsalData rehearsal;
    rehearsal.text = "B";
    rehearsal.enclosure = Enclosure::rectangle;
    rehearsal.fontData.fontFamily = {"Times New Roman"};
    rehearsal.fontData.style = FontStyle::normal;
    rehearsal.fontData.weight = FontWeight::bold;
    rehearsal.fontData.sizeType = FontSizeType::point;
    rehearsal.fontData.sizePoint = 12.0;
    rehearsal.positionData.isDefaultXSpecified = true;
    rehearsal.positionData.defaultX = 5.0;

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.directionTypes.emplace_back(DirectionChoice{rehearsal});
    ostaff.directions.push_back(directionData);

    const auto rscore = mxtest::roundTrip(oscore);
    REQUIRE(rscore.parts.size() == 1);
    REQUIRE(rscore.parts.front().measures.size() == 1);
    REQUIRE(rscore.parts.front().measures.front().staves.size() == 1);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().directionTypes.size() == 1);
    REQUIRE(rdirections.front().directionTypes.front().isRehearsal());
    const auto outRehearsal = rdirections.front().directionTypes.front().rehearsal();
    CHECK_EQUAL("B", outRehearsal.text);
    CHECK(Enclosure::rectangle == outRehearsal.enclosure);
    CHECK(FontWeight::bold == outRehearsal.fontData.weight);
}

T_END;

// Verify that a rehearsal with no enclosure set (Enclosure::unspecified) does not emit
// an enclosure attribute in the serialized XML, and that the field round-trips as unspecified.
TEST(RehearsalUnspecifiedEnclosureNoPhantomAttribute, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];

    NoteData onote{};
    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    RehearsalData rehearsal;
    rehearsal.text = "C";
    // enclosure left at default (unspecified) — must not appear in XML or round-trip as rectangle

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.directionTypes.emplace_back(DirectionChoice{rehearsal});
    ostaff.directions.push_back(directionData);

    const auto xml = mxtest::toXml(oscore);
    CHECK(xml.find("enclosure") == std::string::npos);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().directionTypes.size() == 1);
    REQUIRE(rdirections.front().directionTypes.front().isRehearsal());
    CHECK(Enclosure::unspecified == rdirections.front().directionTypes.front().rehearsal().enclosure);
}

T_END;

// A rehearsal mark's justify attribute round-trips and stays independent of halign, which lives in
// positionData. An unspecified justify emits no attribute.
TEST(RehearsalJustify, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];

    NoteData onote{};
    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    RehearsalData rehearsal;
    rehearsal.text = "D";
    rehearsal.justify = HorizontalAlignment::center;
    rehearsal.positionData.horizontalAlignment = HorizontalAlignment::right;

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.directionTypes.emplace_back(DirectionChoice{rehearsal});
    ostaff.directions.push_back(directionData);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().directionTypes.size() == 1);
    REQUIRE(rdirections.front().directionTypes.front().isRehearsal());
    const auto outRehearsal = rdirections.front().directionTypes.front().rehearsal();
    CHECK(HorizontalAlignment::center == outRehearsal.justify);
    CHECK(HorizontalAlignment::right == outRehearsal.positionData.horizontalAlignment);
}

T_END;

TEST(RehearsalUnspecifiedJustifyNoPhantomAttribute, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];

    NoteData onote{};
    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    RehearsalData rehearsal;
    rehearsal.text = "E";

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.directionTypes.emplace_back(DirectionChoice{rehearsal});
    ostaff.directions.push_back(directionData);

    const auto xml = mxtest::toXml(oscore);
    CHECK(xml.find("justify") == std::string::npos);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().directionTypes.size() == 1);
    REQUIRE(rdirections.front().directionTypes.front().isRehearsal());
    CHECK(HorizontalAlignment::unspecified == rdirections.front().directionTypes.front().rehearsal().justify);
}

T_END;

// Verify that a wedge with no color set (isColorSpecified == false) does not emit a color
// attribute in the serialized XML, and that the field round-trips as unspecified.
TEST(WedgeUnspecifiedColorNoPhantomAttribute, DirectionData)
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto &opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto &omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto &ostaff = omeasure.staves.back();
    auto &ovoice = ostaff.voices[0];

    NoteData onote{};
    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back(onote);

    WedgeStart wedge;
    wedge.wedgeType = WedgeType::crescendo;
    // isColorSpecified left at default (false) -- must not appear in XML or round-trip as white

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.directionTypes.emplace_back(DirectionChoice{wedge});
    ostaff.directions.push_back(directionData);

    const auto xml = mxtest::toXml(oscore);
    CHECK(xml.find("color") == std::string::npos);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().directionTypes.size() == 1);
    REQUIRE(rdirections.front().directionTypes.front().isWedgeStart());
    CHECK(!rdirections.front().directionTypes.front().wedgeStart().isColorSpecified);
}

T_END;

// A note for the tests below, which use ticksPerQuarter 10.
inline NoteData directionDataTestNote(int inTick, int inTicks, DurationName inName)
{
    NoteData note{};
    note.tickTimePosition = inTick;
    note.durationData.durationTimeTicks = inTicks;
    note.durationData.durationName = inName;
    return note;
}

// A forte at inTick, drawn inOffset ticks to the right of it.
inline DirectionData directionDataTestOffsetForte(int inTick, int inOffset)
{
    MarkData mark{StandardDynamic::f};
    mark.tickTimePosition = inTick;
    DirectionData direction{};
    direction.tickTimePosition = inTick;
    direction.offset = inOffset;
    direction.directionTypes.assign(1, DirectionChoice{mark});
    return direction;
}

// A chord symbol after a <forward> in a measure without notes keeps its tick. Its <offset> only moves
// where it is drawn, so the writer must reach the tick with a <forward>.
TEST(OffsetHarmonyInMeasureWithoutNotesKeepsTick, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &staff = measure.staves.emplace_back();

    DirectionData direction{};
    direction.tickTimePosition = 10;
    direction.offset = 3;
    auto &chord = direction.chords.emplace_back();
    chord.root = Step::c;
    chord.chordKind = ChordKind::major;
    staff.directions.push_back(direction);

    const auto xml = mxtest::toXml(score);
    CHECK(xml.find("<forward>") != std::string::npos);

    const auto rscore = mxtest::roundTrip(score);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    CHECK_EQUAL(10, rdirections.front().tickTimePosition);
    REQUIRE(rdirections.front().offset.has_value());
    CHECK_EQUAL(3, *rdirections.front().offset);
    CHECK_EQUAL(1, rdirections.front().chords.size());
}

T_END;

TEST(OffsetDirectionAfterLastNoteKeepsTick, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &staff = measure.staves.emplace_back();
    staff.voices[0].notes.push_back(directionDataTestNote(0, 10, DurationName::quarter));
    staff.directions.push_back(directionDataTestOffsetForte(30, 2));

    const auto rscore = mxtest::roundTrip(score);
    const auto &rstaff = rscore.parts.front().measures.front().staves.front();
    REQUIRE(rstaff.directions.size() == 1);
    CHECK_EQUAL(30, rstaff.directions.front().tickTimePosition);
    REQUIRE(rstaff.directions.front().offset.has_value());
    CHECK_EQUAL(2, *rstaff.directions.front().offset);
    REQUIRE(rstaff.voices.size() == 1);
    REQUIRE(rstaff.voices.cbegin()->second.notes.size() == 1);
    CHECK_EQUAL(0, rstaff.voices.cbegin()->second.notes.front().tickTimePosition);
}

T_END;

TEST(OffsetDirectionBeforeFirstNoteKeepsTick, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &staff = measure.staves.emplace_back();
    auto &notes = staff.voices[0].notes;
    notes.push_back(directionDataTestNote(20, 10, DurationName::quarter));
    notes.push_back(directionDataTestNote(30, 10, DurationName::quarter));
    staff.directions.push_back(directionDataTestOffsetForte(10, 2));

    const auto rscore = mxtest::roundTrip(score);
    const auto &rstaff = rscore.parts.front().measures.front().staves.front();
    REQUIRE(rstaff.directions.size() == 1);
    CHECK_EQUAL(10, rstaff.directions.front().tickTimePosition);
    REQUIRE(rstaff.directions.front().offset.has_value());
    CHECK_EQUAL(2, *rstaff.directions.front().offset);
    REQUIRE(rstaff.voices.size() == 1);
    const auto &rnotes = rstaff.voices.cbegin()->second.notes;
    REQUIRE(rnotes.size() == 2);
    CHECK_EQUAL(20, rnotes.front().tickTimePosition);
    CHECK_EQUAL(30, rnotes.back().tickTimePosition);
}

T_END;

// The second staff has only a direction. The first staff's whole note leaves the write position at
// the end of the measure, so the writer backs up to reach the direction's tick.
TEST(OffsetDirectionOnStaffWithoutNotesKeepsTick, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &upper = measure.staves.emplace_back();
    upper.voices[0].notes.push_back(directionDataTestNote(0, 40, DurationName::whole));
    auto &lower = measure.staves.emplace_back();
    lower.directions.push_back(directionDataTestOffsetForte(0, 5));

    const auto rscore = mxtest::roundTrip(score);
    const auto &rstaves = rscore.parts.front().measures.front().staves;
    REQUIRE(rstaves.size() == 2);
    CHECK(rstaves.front().directions.empty());
    REQUIRE(rstaves.back().directions.size() == 1);
    CHECK_EQUAL(0, rstaves.back().directions.front().tickTimePosition);
    REQUIRE(rstaves.back().directions.front().offset.has_value());
    CHECK_EQUAL(5, *rstaves.back().directions.front().offset);
}

T_END;

// A middle barline between the last note and the direction is written where it stands, so the
// <forward> to the direction is split in two around it.
TEST(MiddleBarlineBeforeOffsetDirectionKeepsTick, DirectionData)
{
    ScoreData score;
    score.ticksPerQuarter = 10;
    score.parts.emplace_back();
    auto &measure = score.parts.back().measures.emplace_back();
    auto &staff = measure.staves.emplace_back();
    staff.voices[0].notes.push_back(directionDataTestNote(0, 10, DurationName::quarter));
    staff.directions.push_back(directionDataTestOffsetForte(30, 3));
    BarlineData barline{};
    barline.tickTimePosition = 20;
    barline.location = HorizontalAlignment::center;
    barline.barlineType = BarlineType::dashed;
    measure.barlines.push_back(barline);

    const auto rscore = mxtest::roundTrip(score);
    const auto &rmeasure = rscore.parts.front().measures.front();
    REQUIRE(rmeasure.barlines.size() == 1);
    CHECK_EQUAL(20, rmeasure.barlines.front().tickTimePosition);
    REQUIRE(rmeasure.staves.size() == 1);
    REQUIRE(rmeasure.staves.front().directions.size() == 1);
    CHECK_EQUAL(30, rmeasure.staves.front().directions.front().tickTimePosition);
    CHECK(rscore.parts.front().measures == score.parts.front().measures);
}

T_END;

#endif
