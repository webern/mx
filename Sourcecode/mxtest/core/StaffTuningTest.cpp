// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, StaffTuning )
{
	StaffTuning object;
	stringstream expected;
	streamLine( expected, 1, R"(<staff-tuning>)" );
	streamLine( expected, 2, R"(<tuning-step>A</tuning-step>)" );
	streamLine( expected, 2, R"(<tuning-octave>0</tuning-octave>)" );
	streamLine( expected, 1, R"(</staff-tuning>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, StaffTuning )
{
	StaffTuning object;
    object.getAttributes()->hasLine = true;
    object.getAttributes()->line = StaffLine{ 3 };
    object.setHasTuningAlter( true );
    object.getTuningAlter()->setValue( Semitones{ -1 } );
    object.getTuningStep()->setValue( StepEnum::f );
    auto value = makeTuningOctave( OctaveValue( 5 ) );
    object.setTuningOctave( value );
	stringstream expected;
	streamLine( expected, 1, R"(<staff-tuning line="3">)" );
	streamLine( expected, 2, R"(<tuning-step>F</tuning-step>)" );
	streamLine( expected, 2, R"(<tuning-alter>-1</tuning-alter>)" );
	streamLine( expected, 2, R"(<tuning-octave>5</tuning-octave>)" );
	streamLine( expected, 1, R"(</staff-tuning>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
