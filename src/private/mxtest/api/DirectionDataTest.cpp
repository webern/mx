// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
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
    MarkData mark{MarkType::f};

    int tickTime = 10;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back(mark);
    ostaff.directions.push_back(directionData);

    tickTime = 9;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back(mark);
    ostaff.directions.push_back(directionData);

    tickTime = 8;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back(mark);
    ostaff.directions.push_back(directionData);

    const auto rscore = mxtest::roundTrip(oscore);
    const auto &rpart = rscore.parts.back();
    const auto &rmeasure = rpart.measures.back();
    const auto &rstaff = rmeasure.staves.back();
    const auto &rdirections = rstaff.directions;

    CHECK_EQUAL(3, rdirections.size());
    auto rdirection = rdirections.cbegin();
    CHECK_EQUAL(8, rdirection->tickTimePosition);
    ++rdirection;
    CHECK_EQUAL(9, rdirection->tickTimePosition);
    ++rdirection;
    CHECK_EQUAL(10, rdirection->tickTimePosition);
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
        MarkData mark{MarkType::f};

        mark.tickTimePosition = dur0tick;
        directionData.tickTimePosition = dur0tick;
        directionData.marks.clear();
        directionData.marks.push_back(mark);
        ostaff.directions.push_back(directionData);

        mark.tickTimePosition = dur1tick;
        directionData.tickTimePosition = dur1tick;
        directionData.marks.clear();
        directionData.marks.push_back(mark);
        ostaff.directions.push_back(directionData);

        mark.tickTimePosition = dur2tick;
        directionData.tickTimePosition = dur2tick;
        directionData.marks.clear();
        directionData.marks.push_back(mark);
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

        CHECK_EQUAL(3, rdirections.size());
        auto rdirection = rdirections.cbegin();
        CHECK_EQUAL(tempTickSorter.at(0), rdirection->tickTimePosition);
        ++rdirection;
        CHECK_EQUAL(tempTickSorter.at(1), rdirection->tickTimePosition);
        ++rdirection;
        CHECK_EQUAL(tempTickSorter.at(2), rdirection->tickTimePosition);
    }
}

T_END;

// Parse the synthetic rehearsal file and confirm that mx::api reads the text and enclosure.
// This pins the core -> api read path and would fail if parseRehearsal regresses.
TEST(RehearsalSyntheticFileRead, DirectionData)
{
    const std::string path = mxtest::getResourcesDirectoryPath() + "synthetic/rehearsal.3.1.xml";
    auto &docMgr = DocumentManager::getInstance();
    const auto docIdResult = docMgr.createFromFile(path);
    REQUIRE(docIdResult.ok());
    const int docId = docIdResult.value();
    const auto scoreResult = docMgr.getData(docId);
    docMgr.destroyDocument(docId);
    REQUIRE(scoreResult.ok());
    const auto &score = scoreResult.value();
    REQUIRE(score.parts.size() == 1);
    REQUIRE(score.parts.front().measures.size() == 1);
    REQUIRE(score.parts.front().measures.front().staves.size() == 1);
    const auto &directions = score.parts.front().measures.front().staves.front().directions;
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.front().rehearsals.size() == 1);
    const auto &rehearsal = directions.front().rehearsals.front();
    CHECK_EQUAL("x", rehearsal.text);
    CHECK(RehearsalEnclosure::rectangle == rehearsal.enclosure);
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
    rehearsal.enclosure = RehearsalEnclosure::rectangle;
    rehearsal.fontData.fontFamily = {"Times New Roman"};
    rehearsal.fontData.style = FontStyle::normal;
    rehearsal.fontData.weight = FontWeight::bold;
    rehearsal.fontData.sizeType = FontSizeType::point;
    rehearsal.fontData.sizePoint = 12.0;
    rehearsal.positionData.isDefaultXSpecified = true;
    rehearsal.positionData.defaultX = 5.0;

    DirectionData directionData;
    directionData.tickTimePosition = 0;
    directionData.rehearsals.push_back(rehearsal);
    ostaff.directions.push_back(directionData);

    const auto rscore = mxtest::roundTrip(oscore);
    REQUIRE(rscore.parts.size() == 1);
    REQUIRE(rscore.parts.front().measures.size() == 1);
    REQUIRE(rscore.parts.front().measures.front().staves.size() == 1);
    const auto &rdirections = rscore.parts.front().measures.front().staves.front().directions;
    REQUIRE(rdirections.size() == 1);
    REQUIRE(rdirections.front().rehearsals.size() == 1);
    CHECK_EQUAL("B", rdirections.front().rehearsals.front().text);
    CHECK(RehearsalEnclosure::rectangle == rdirections.front().rehearsals.front().enclosure);
    CHECK(FontWeight::bold == rdirections.front().rehearsals.front().fontData.weight);
}

T_END;

// Verify that a rehearsal with no enclosure set (RehearsalEnclosure::unspecified) does not emit
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
    directionData.rehearsals.push_back(rehearsal);
    ostaff.directions.push_back(directionData);

    const auto xml = mxtest::toXml(oscore);
    CHECK(xml.find("enclosure") == std::string::npos);

    const auto rscore = mxtest::roundTrip(oscore);
    REQUIRE(rscore.parts.front().measures.front().staves.front().directions.size() == 1);
    REQUIRE(rscore.parts.front().measures.front().staves.front().directions.front().rehearsals.size() == 1);
    CHECK(RehearsalEnclosure::unspecified ==
          rscore.parts.front().measures.front().staves.front().directions.front().rehearsals.front().enclosure);
}

T_END;

#endif
