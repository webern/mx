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

#endif
