// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, StaffDetails )
{
	StaffDetails object;
	stringstream expected;
	streamLine( expected, 1, R"(<staff-details/>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( ! object.hasContents() )
}
TEST( Test02, StaffDetails )
{
	StaffDetails object;
    object.getAttributes()->hasPrintObject = true;
    object.getAttributes()->hasPrintSpacing = true;
    object.getAttributes()->hasNumber = true;
    object.getAttributes()->number = StaffNumber{ 5 };
    object.getAttributes()->hasShowFrets = true;
    object.setHasStaffLines( true );
    object.setHasStaffSize( true );
    object.getStaffSize()->setValue( NonNegativeDecimal( 5.5 ) );
    auto tuning = makeStaffTuning();
    tuning->setHasTuningAlter( true );
    tuning->setTuningStep( makeTuningStep( StepEnum::c ) );
    tuning->setTuningAlter( makeTuningAlter( Semitones( -1.1 ) ) );
    tuning->setTuningOctave( makeTuningOctave( OctaveValue( 6 ) ) );
    object.addStaffTuning( tuning );
    
    auto tuning2 = makeStaffTuning();
    tuning->setHasTuningAlter( true );
    tuning->setTuningStep( makeTuningStep( StepEnum::d ) );
    tuning->setTuningAlter( makeTuningAlter( Semitones( -0.1 ) ) );
    tuning->setTuningOctave( makeTuningOctave( OctaveValue( 3 ) ) );
    object.addStaffTuning( tuning2 );
    
	stringstream expected;
	streamLine( expected, 1, R"(<staff-details number="5" show-frets="numbers" print-object="no" print-spacing="no">)" );
	streamLine( expected, 2, R"(<staff-lines>5</staff-lines>)" );
	streamLine( expected, 2, R"(<staff-tuning>)" );
	streamLine( expected, 3, R"(<tuning-step>D</tuning-step>)" );
	streamLine( expected, 3, R"(<tuning-alter>-0.1</tuning-alter>)" );
	streamLine( expected, 3, R"(<tuning-octave>3</tuning-octave>)" );
	streamLine( expected, 2, R"(</staff-tuning>)" );
	streamLine( expected, 2, R"(<staff-tuning>)" );
	streamLine( expected, 3, R"(<tuning-step>A</tuning-step>)" );
	streamLine( expected, 3, R"(<tuning-octave>0</tuning-octave>)" );
	streamLine( expected, 2, R"(</staff-tuning>)" );
	streamLine( expected, 2, R"(<staff-size>5.5</staff-size>)" );
	streamLine( expected, 1, R"(</staff-details>)", false );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object.hasAttributes() )
	CHECK( object.hasContents() )
}

#endif
