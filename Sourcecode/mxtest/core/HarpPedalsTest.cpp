// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, HarpPedals )
{
	HarpPedals object;
	stringstream expected;
	streamLine( expected, 1, R"(<harp-pedals>)" );
	streamLine( expected, 2, R"(<pedal-tuning>)" );
	streamLine( expected, 3, R"(<pedal-step>A</pedal-step>)" );
	streamLine( expected, 3, R"(<pedal-alter>0</pedal-alter>)" );
	streamLine( expected, 2, R"(</pedal-tuning>)" );
	streamLine( expected, 1, R"(</harp-pedals>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, HarpPedals )
{
	HarpPedals object;
    object.removePedalTuning( object.getPedalTuningSet().cbegin() ); // does nothing becuase minOccurs = 1
    object.getPedalTuning( object.getPedalTuningSet().cbegin() )->getPedalStep()->setValue( StepEnum::b );
    object.getPedalTuning( object.getPedalTuningSet().cbegin() )->getPedalAlter()->setValue( Semitones( -1 ) );
    auto pt1 = makePedalTuning();
    pt1->getPedalStep()->setValue( StepEnum::f );
    pt1->getPedalAlter()->setValue( Semitones( 1 ) );
    object.addPedalTuning( pt1 );
    stringstream expected;
	streamLine( expected, 1, R"(<harp-pedals>)" );
	streamLine( expected, 2, R"(<pedal-tuning>)" );
	streamLine( expected, 3, R"(<pedal-step>B</pedal-step>)" );
	streamLine( expected, 3, R"(<pedal-alter>-1</pedal-alter>)" );
	streamLine( expected, 2, R"(</pedal-tuning>)" );
	streamLine( expected, 2, R"(<pedal-tuning>)" );
	streamLine( expected, 3, R"(<pedal-step>F</pedal-step>)" );
	streamLine( expected, 3, R"(<pedal-alter>1</pedal-alter>)" );
	streamLine( expected, 2, R"(</pedal-tuning>)" );
	streamLine( expected, 1, R"(</harp-pedals>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test03, HarpPedals )
{
	HarpPedals object;
    object.removePedalTuning( object.getPedalTuningSet().cbegin() ); // does nothing becuase minOccurs = 1
    object.getPedalTuning( object.getPedalTuningSet().cbegin() )->getPedalStep()->setValue( StepEnum::b );
    object.getPedalTuning( object.getPedalTuningSet().cbegin() )->getPedalAlter()->setValue( Semitones( -1 ) );
    auto pt1 = makePedalTuning();
    pt1->getPedalStep()->setValue( StepEnum::f );
    pt1->getPedalAlter()->setValue( Semitones( 1 ) );
    object.addPedalTuning( pt1 );
    object.clearPedalTuningSet();
    stringstream expected;
	streamLine( expected, 1, R"(<harp-pedals>)" );
	streamLine( expected, 2, R"(<pedal-tuning>)" );
	streamLine( expected, 3, R"(<pedal-step>A</pedal-step>)" );
	streamLine( expected, 3, R"(<pedal-alter>0</pedal-alter>)" );
	streamLine( expected, 2, R"(</pedal-tuning>)" );
	streamLine( expected, 1, R"(</harp-pedals>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
