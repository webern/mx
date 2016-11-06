// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "mx/impl/MeasureWriter.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Line.h"
#include "cpul/cpulTestHarness.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

// TODO - make real tests

namespace mxtest
{
    struct TestItems
    {
        std::unique_ptr<api::ScoreData> scoreData;
        std::unique_ptr<ScoreWriter> scoreWriter;
        api::MeasureData* measureData;
        std::unique_ptr<MeasureCursor> measureCursor;
        std::unique_ptr<MeasureWriter> measureWriter;
    };
    
    struct TestParameters
    {
        int ticksPerQuarter;
        int measureIndex;
        int partIndex;
        int numStaves;
    };
    
    TestItems setupTestItems( TestParameters params )
    {
        TestItems t;
        t.scoreData = std::make_unique<api::ScoreData>();
        t.scoreData->ticksPerQuarter = params.ticksPerQuarter;
        t.scoreData->parts.resize( params.partIndex + 1 );
        int pindex = 0;
        for( auto& p : t.scoreData->parts )
        {
            p.measures.resize( params.measureIndex + 1 );
            int mindex = 0;
            for( auto& m : p.measures )
            {
                m.staves.resize( params.numStaves );
                if( pindex == params.partIndex && mindex == params.measureIndex )
                {
                    t.measureData = &m;
                }
                ++mindex;
            }
            ++pindex;
        }
        t.scoreWriter = std::make_unique<ScoreWriter>( *t.scoreData );
        t.measureCursor = std::make_unique<MeasureCursor>( static_cast<int>( t.measureData->staves.size() ), params.ticksPerQuarter );
        t.measureCursor->partIndex = params.partIndex;
        t.measureCursor->measureIndex = params.measureIndex;
        t.measureWriter = std::make_unique<MeasureWriter>( *t.measureData, *t.measureCursor, *t.scoreWriter );
        return t;
    }
}


TEST( measureNumber, MeasureWriter )
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 100;
    params.measureIndex = 3;
    params.partIndex = 4;
    params.numStaves = 2;
    mxtest::TestItems t = mxtest::setupTestItems( params );
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    const auto expected = "4";
    const auto actual = partwiseMeasure->getAttributes()->number.getValue();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( measureNumberString, MeasureWriter )
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems( params );
    t.measureData->number = "hello";
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    const auto expected = "hello";
    const auto actual = partwiseMeasure->getAttributes()->number.getValue();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( leftBarlineFirstNoteRequiresForward, MeasureWriter )
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems( params );
    t.measureData->barlines.emplace_back( api::BarlineData{} );
    auto& barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::heavyLight;
    auto& staff = t.measureData->staves.at( 0 );
    auto& voice = staff.voices[0];
    voice.notes.emplace_back( api::NoteData{} );
    auto& note = voice.notes.back();
    note.tickTimePosition = 101;
    
    
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto mdcIter = partwiseMeasure->getMusicDataGroup()->getMusicDataChoiceSet().cbegin();
    auto mdcEnd = partwiseMeasure->getMusicDataGroup()->getMusicDataChoiceSet().cend();

    CHECK( mdcIter != mdcEnd );
    
    auto choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::barline );

    CHECK( ++mdcIter != mdcEnd );
    choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::properties );

    CHECK( ++mdcIter != mdcEnd );
    choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::forward );

    CHECK( ++mdcIter != mdcEnd );
    choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::note );
    
    CHECK( ++mdcIter == mdcEnd );

}
T_END


TEST( PropertiesButNoNotes, MeasureWriter )
{
    mxtest::TestParameters params;
    params.ticksPerQuarter = 101;
    params.measureIndex = 0;
    params.partIndex = 0;
    params.numStaves = 1;
    mxtest::TestItems t = mxtest::setupTestItems( params );
    t.measureData->barlines.emplace_back( api::BarlineData{} );
    auto& barline = t.measureData->barlines.front();
    barline.barlineType = api::BarlineType::heavyLight;
    auto& staff = t.measureData->staves.at( 0 );
    staff.clefs.emplace_back( api::ClefData{} );
    auto& clef = staff.clefs.back();
    clef.symbol = api::ClefSymbol::percussion;
    clef.line = 2;
    clef.tickTimePosition = 101;
    
    const auto partwiseMeasure = t.measureWriter->getPartwiseMeasure();
    auto mdcIter = partwiseMeasure->getMusicDataGroup()->getMusicDataChoiceSet().cbegin();
    auto mdcEnd = partwiseMeasure->getMusicDataGroup()->getMusicDataChoiceSet().cend();

    CHECK( mdcIter != mdcEnd );
    
    auto choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::barline );
    
    CHECK( ++mdcIter != mdcEnd );
    choice = (*mdcIter)->getChoice();
    CHECK( choice == core::MusicDataChoice::Choice::properties );
    auto props = (*mdcIter)->getProperties();
    CHECK( props->getHasDivisions() );
    CHECK_DOUBLES_EQUAL( 101.0, props->getDivisions()->getValue().getValue(), 0.01 );
    
    CHECK_EQUAL( 1, props->getClefSet().size() );
    CHECK( props->getClefSet().back()->getAttributes()->hasNumber );
    CHECK( 1 == props->getClefSet().back()->getAttributes()->number.getValue() );
    
    CHECK( ++mdcIter == mdcEnd );
}
T_END

#endif
