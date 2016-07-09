// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ToeTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Toe )
{
    variant v = variant::one;
	ToePtr object = tgenToe( v );
	stringstream expected;
	tgenToeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Toe )
{
    variant v = variant::two;
	ToePtr object = tgenToe( v );
	stringstream expected;
	tgenToeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test03, Toe )
{
    variant v = variant::three;
	ToePtr object = tgenToe( v );
	stringstream expected;
	tgenToeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}

namespace MxTestHelpers
{
    ToePtr tgenToe( variant v )
    {
        ToePtr o = makeToe();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasSubstitution = true;
                o->getAttributes()->substitution = YesNo::yes;
                o->getAttributes()->hasFontStyle = true;
                o->getAttributes()->fontStyle = FontStyle::italic;
                o->getAttributes()->hasColor = true;
                o->getAttributes()->color = Color( 83, 102, 30, 22 );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasFontFamily = true;
                o->getAttributes()->fontFamily = CommaSeparatedText( "Bish,and,Bones" );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenToeExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<toe/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<toe font-style="italic" color="#53661E16" substitution="yes"/>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<toe font-family="Bish,and,Bones"/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
