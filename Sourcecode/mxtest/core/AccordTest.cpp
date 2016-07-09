// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/AccordTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Accord )
{
    variant v = variant::one;
	AccordPtr object = tgenAccord( v );
	stringstream expected;
	tgenAccordExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Accord )
{
    variant v = variant::two;
	AccordPtr object = tgenAccord( v );
	stringstream expected;
	tgenAccordExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Accord )
{
    variant v = variant::three;
	AccordPtr object = tgenAccord( v );
	stringstream expected;
	tgenAccordExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
namespace MxTestHelpers
{
    AccordPtr tgenAccord( variant v )
    {
        AccordPtr o = makeAccord();
        switch ( v )
        {
            case variant::one:
            {
                ;
            }
                break;
            case variant::two:
            {
                o->getTuningStep()->setValue( StepEnum::c );
                o->setHasTuningAlter( true );
                o->getTuningAlter()->setValue( Semitones( -1 ) );
                o->getTuningOctave()->setValue( OctaveValue( 4 ) );
            }
                break;
            case variant::three:
            {
                o->getTuningStep()->setValue( StepEnum::f );
                o->setHasTuningAlter( true );
                o->getTuningAlter()->setValue( Semitones( 1 ) );
                o->getTuningOctave()->setValue( OctaveValue( 5 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenAccordExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<accord>)" );
                streamLine( os, i+1, R"(<tuning-step>A</tuning-step>)" );
                streamLine( os, i+1, R"(<tuning-octave>0</tuning-octave>)" );
                streamLine( os, i, R"(</accord>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<accord>)" );
                streamLine( os, i+1, R"(<tuning-step>C</tuning-step>)" );
                streamLine( os, i+1, R"(<tuning-alter>-1</tuning-alter>)" );
                streamLine( os, i+1, R"(<tuning-octave>4</tuning-octave>)" );
                streamLine( os, i, R"(</accord>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<accord>)" );
                streamLine( os, i+1, R"(<tuning-step>F</tuning-step>)" );
                streamLine( os, i+1, R"(<tuning-alter>1</tuning-alter>)" );
                streamLine( os, i+1, R"(<tuning-octave>5</tuning-octave>)" );
                streamLine( os, i, R"(</accord>)", false );
            }
                break;
            default:
                break;
        }
    }
}
#endif