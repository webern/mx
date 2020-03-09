// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"
#include "mx/core/Document.h"
#include "mx/core/Document.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/Tied.h"
#include "mxtest/api/RoundTrip.h"
#include "mxtest/api/TestHelpers.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST( OutOfOrderDoesntThrow, DirectionData )
{
    ScoreData oscore;
    oscore.ticksPerQuarter = 10;
    oscore.parts.emplace_back();
    auto& opart = oscore.parts.back();
    opart.measures.emplace_back();
    auto& omeasure = opart.measures.back();
    omeasure.staves.emplace_back();
    auto& ostaff = omeasure.staves.back();
    auto& ovoice = ostaff.voices[0];
    NoteData onote{};

    onote.tickTimePosition = 0;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back( onote );

    onote.tickTimePosition = 10;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back( onote );

    onote.tickTimePosition = 20;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back( onote );

    onote.tickTimePosition = 30;
    onote.durationData.durationTimeTicks = 10;
    onote.durationData.durationName = DurationName::quarter;
    onote.durationData.durationName = DurationName::quarter;
    ovoice.notes.push_back( onote );

    DirectionData directionData{};
    MarkData mark{ MarkType::f };

    int tickTime = 10;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back( mark );
    ostaff.directions.push_back( directionData );

    tickTime = 9;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back( mark );
    ostaff.directions.push_back( directionData );

    tickTime = 8;
    mark.tickTimePosition = tickTime;
    directionData.tickTimePosition = tickTime;
    directionData.marks.clear();
    directionData.marks.push_back( mark );
    ostaff.directions.push_back( directionData );

    const auto rscore = mxtest::roundTrip( oscore );
    const auto& rpart = rscore.parts.back();
    const auto& rmeasure = rpart.measures.back();
    const auto& rstaff = rmeasure.staves.back();
    const auto& rdirections = rstaff.directions;

    CHECK_EQUAL( 3, rdirections.size() );
    auto rdirection = rdirections.cbegin();
    CHECK_EQUAL( 8, rdirection->tickTimePosition );
    ++rdirection;
    CHECK_EQUAL( 9, rdirection->tickTimePosition );
    ++rdirection;
    CHECK_EQUAL( 10, rdirection->tickTimePosition );
}
T_END;

TEST( OutOfOrderTorture, DirectionData )
{
    const std::vector<int> randomNumbers =
    { -31, 25, -9, 21, 0, 77, -100, 100, 0,
       31, 12, 6, 19, 109, 10000, -100000,
       0, 231, 652, 2345, 11, 11, 1, 11
    };

    const auto vecSize = randomNumbers.size();

    for( int i = 0; i < 100; ++i )
    {
        const int tick0 = randomNumbers.at( static_cast<size_t>( i + 0 ) % vecSize );
        const int tick1 = randomNumbers.at( static_cast<size_t>( i + 1 ) % vecSize );
        const int tick2 = randomNumbers.at( static_cast<size_t>( i + 2 ) % vecSize );
        const int tick3 = randomNumbers.at( static_cast<size_t>( i + 3 ) % vecSize );
        const int dur0tick = randomNumbers.at( static_cast<size_t>( i + 4 ) % vecSize );
        const int dur1tick = randomNumbers.at( static_cast<size_t>( i + 5 ) % vecSize );
        const int dur2tick = randomNumbers.at( static_cast<size_t>( i + 6 ) % vecSize );

        ScoreData oscore;
        oscore.ticksPerQuarter = 10;
        oscore.parts.emplace_back();
        auto& opart = oscore.parts.back();
        opart.measures.emplace_back();
        auto& omeasure = opart.measures.back();
        omeasure.staves.emplace_back();
        auto& ostaff = omeasure.staves.back();
        auto& ovoice = ostaff.voices[0];
        NoteData onote{};

        onote.tickTimePosition = tick0;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back( onote );

        onote.tickTimePosition = tick1;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back( onote );

        onote.tickTimePosition = tick2;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back( onote );

        onote.tickTimePosition = tick3;
        onote.durationData.durationTimeTicks = 10;
        onote.durationData.durationName = DurationName::quarter;
        onote.durationData.durationName = DurationName::quarter;
        ovoice.notes.push_back( onote );

        DirectionData directionData{};
        MarkData mark{ MarkType::f };

        mark.tickTimePosition = dur0tick;
        directionData.tickTimePosition = dur0tick;
        directionData.marks.clear();
        directionData.marks.push_back( mark );
        ostaff.directions.push_back( directionData );

        mark.tickTimePosition = dur1tick;
        directionData.tickTimePosition = dur1tick;
        directionData.marks.clear();
        directionData.marks.push_back( mark );
        ostaff.directions.push_back( directionData );

        mark.tickTimePosition = dur2tick;
        directionData.tickTimePosition = dur2tick;
        directionData.marks.clear();
        directionData.marks.push_back( mark );
        ostaff.directions.push_back( directionData );

        const auto rscore = mxtest::roundTrip( oscore );
        const auto& rpart = rscore.parts.back();
        const auto& rmeasure = rpart.measures.back();
        const auto& rstaff = rmeasure.staves.back();
        const auto& rdirections = rstaff.directions;

        std::vector<int> tempTickSorter = { dur0tick, dur1tick, dur2tick };
        std::sort( std::begin( tempTickSorter ), std::end( tempTickSorter ) );

        int expectedShift = 0;

        if( tick0 < 0 )
        {
            expectedShift = ( -1 * tick0 );
        }

        CHECK_EQUAL( 3, rdirections.size() );
        auto rdirection = rdirections.cbegin();
        CHECK_EQUAL( tempTickSorter.at( 0 ), rdirection->tickTimePosition );
        ++rdirection;
        CHECK_EQUAL( tempTickSorter.at( 1 ), rdirection->tickTimePosition );
        ++rdirection;
        CHECK_EQUAL( tempTickSorter.at( 2 ), rdirection->tickTimePosition );
    }
}
T_END;

#endif
