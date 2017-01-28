// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/BassTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Bass )
{
    variant v = variant::one;
	BassPtr object = tgenBass( v );
	stringstream expected;
	tgenBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Bass )
{
    variant v = variant::two;
	BassPtr object = tgenBass( v );
	stringstream expected;
	tgenBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Bass )
{
    variant v = variant::three;
	BassPtr object = tgenBass( v );
	stringstream expected;
	tgenBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    BassPtr tgenBass( variant v )
    {
        BassPtr o = makeBass();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasBassAlter( true );
                o->getBassAlter()->setValue( Semitones( -2 ) );
                o->getBassStep()->setValue( StepEnum::f );
            }
                break;
            case variant::three:
            {
                o->setHasBassAlter( true );
                o->getBassAlter()->setValue( Semitones( 1.1 ) );
                o->getBassStep()->setValue( StepEnum::c );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenBassExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<bass>)" );
                streamLine( os, i+1, R"(<bass-step>A</bass-step>)" );
                streamLine( os, i, R"(</bass>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<bass>)" );
                streamLine( os, i+1, R"(<bass-step>F</bass-step>)" );
                streamLine( os, i+1, R"(<bass-alter>-2</bass-alter>)" );
                streamLine( os, i, R"(</bass>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<bass>)" );
                streamLine( os, i+1, R"(<bass-step>C</bass-step>)" );
                streamLine( os, i+1, R"(<bass-alter>1.1</bass-alter>)" );
                streamLine( os, i, R"(</bass>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
