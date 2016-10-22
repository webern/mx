#include <iostream>
#include "cpul/cpulRun.h"
#include "cpul/cpulTestTimer.h"
#include "mxtest/api/RoundTrip.h"

void doStuff();

int main(int argc, const char * argv[])
{
    //MxRtiTest::rtiMain();
    UNUSED_PARAMETER( argc )
    UNUSED_PARAMETER( argv )
    cpul::TestTimer timer;
    //cpulRun( true );
    mxtest::roundTrip();
    doStuff();
    timer.report( "Total runtime for all tests" );
    return 0;
}


#include "mx/api/ScoreData.h"

void doStuff()
{
    using namespace mx::api;
    ScoreData score;
    score.parts.emplace_back( PartData{} );
    score.ticksPerQuarter = 8;
    auto& part = score.parts.front();
    part.uniqueId = "P1";
    part.name = "MusicXML Part";

    // measure 1 - setup
    part.measures.emplace_back( MeasureData{} );
    auto measureP = &part.measures.front();
    measureP->keys.emplace_back( KeyData{} );
    auto keyP = &measureP->keys.back();
    keyP->mode = KeyMode::major;
    measureP->timeSignature.isImplicit = false;
    measureP->timeSignature.symbol = TimeSignatureSymbol::common;
    measureP->staves.emplace_back( StaffData{} );
    measureP->staves.emplace_back( StaffData{} );
    auto staff1P = &measureP->staves.at( 0 );
    staff1P->clefs.emplace_back( ClefData{} );
    auto staff2P = &measureP->staves.at( 1 );
    staff2P->clefs.emplace_back( ClefData{} );
    auto clefP = &staff2P->clefs.back();
    clefP->symbol = ClefSymbol::f;
    clefP->line = 4;
    staff1P->directions.emplace_back( DirectionData{} );
    auto directionP = &staff1P->directions.back();
    directionP->placement = Placement::below;
    directionP->marks.emplace_back( DirectionMark{} );
    auto directionMarkP = &directionP->marks.back();
    directionMarkP->markType = MarkType::ffff;
    
    // measure 1 - music
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    auto noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->directions.emplace_back( DirectionData{} );
    directionP = &staff1P->directions.back();
    directionP->tickTimePosition = 24;
    directionP->isOffsetSpecified = true;
    directionP->offset = 1;
    directionP->placement = Placement::below;
    directionP->isStaffValueSpecified = true;
    directionP->marks.emplace_back( DirectionMark{} );
    directionMarkP = &directionP->marks.back();
    directionMarkP->markType = MarkType::p;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::g;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 2;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->directions.emplace_back( DirectionData{} );
    directionP = &staff2P->directions.back();
    directionP->tickTimePosition = 0;
    directionP->isOffsetSpecified = false;
    directionP->offset = 0;
    directionP->placement = Placement::below;
    directionP->isStaffValueSpecified = true;
    directionP->wedgeStarts.emplace_back( WedgeStart{} );
    auto wedgeStartP = &directionP->wedgeStarts.back();
    wedgeStartP->wedgeType = WedgeType::crescendo;
    wedgeStartP->isSpreadSpecified = true;
    wedgeStartP->spread = 0.0;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 2;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->directions.emplace_back( DirectionData{} );
    directionP = &staff2P->directions.back();
    directionP->tickTimePosition = 16;
    directionP->isOffsetSpecified = false;
    directionP->offset = 0;
    directionP->placement = Placement::unspecified;
    directionP->isStaffValueSpecified = true;
    directionP->wedgeStops.emplace_back( WedgeStop{} );
    auto wedgeStopP = &directionP->wedgeStops.back();
    wedgeStopP->isSpreadSpecified = true;
    wedgeStopP->spread = 15.0;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 3;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 3;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    // measure 2 - setup
    part.measures.emplace_back( MeasureData{} );
    measureP = &part.measures.back();
    measureP->keys.emplace_back( KeyData{} );
    keyP = &measureP->keys.back();
    keyP->fifths = 2;
    keyP->mode = KeyMode::major;
    measureP->staves.emplace_back( StaffData{} );
    measureP->staves.emplace_back( StaffData{} );
    staff1P = &measureP->staves.at( 0 );
    staff2P = &measureP->staves.at( 1 );
    staff2P->clefs.emplace_back( ClefData{} );
    clefP = &staff2P->clefs.back();
    clefP->symbol = ClefSymbol::g;
    clefP->line = 2;

    // measure 2 - music
    staff1P = &measureP->staves.at( 0 );
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P = &measureP->staves.at( 1 );
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::f;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::g;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    // measure 3 - setup
    part.measures.emplace_back( MeasureData{} );
    measureP = &part.measures.back();
    measureP->staves.emplace_back( StaffData{} );
    measureP->staves.emplace_back( StaffData{} );
    staff1P = &measureP->staves.at( 0 );
    staff2P = &measureP->staves.at( 1 );
    staff1P->clefs.emplace_back( ClefData{} );
    clefP = &staff1P->clefs.back();
    clefP->symbol = ClefSymbol::c;
    clefP->line = 2;
    
    // measure 3 - music
    staff1P = &measureP->staves.at( 0 );
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;

    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::a;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 8;
    noteP->pitchData.step = Step::b;
    noteP->pitchData.octave = 4;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 16;
    noteP->pitchData.step = Step::c;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 1;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 24;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::quarter;
    noteP->durationData.durationTimeTicks = 8;
    
    // measure 4 - setup
    part.measures.emplace_back( MeasureData{} );
    measureP = &part.measures.back();
    measureP->staves.emplace_back( StaffData{} );
    measureP->staves.emplace_back( StaffData{} );
    staff1P = &measureP->staves.at( 0 );
    staff2P = &measureP->staves.at( 1 );
    
    // measure 4 - music
    staff1P = &measureP->staves.at( 0 );
    staff1P->voices[0].notes.emplace_back( NoteData{} );
    noteP = &staff1P->voices[0].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::whole;
    noteP->isRest = true;
    noteP->durationData.durationTimeTicks = 32;
    
    staff2P->voices[1].notes.emplace_back( NoteData{} );
    noteP = &staff2P->voices[1].notes.back();
    noteP->tickTimePosition = 0;
    noteP->pitchData.step = Step::d;
    noteP->pitchData.octave = 5;
    noteP->pitchData.alter = 0;
    noteP->pitchData.accidental = Accidental::none;
    noteP->durationData.durationName = DurationName::whole;
    noteP->isRest = true;
    noteP->durationData.durationTimeTicks = 32;
    
    measureP->barlines.emplace_back( BarlineData{} );
    
    // save the file
    auto docId = DocumentManager::getInstance().createFromScore( score );
    DocumentManager::getInstance().writeToFile( docId, "./ly23e_manually.xml" );
    DocumentManager::getInstance().destroyDocument( docId );
}
