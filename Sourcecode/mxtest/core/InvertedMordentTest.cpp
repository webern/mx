// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/InvertedMordentTest.h"
#include "mxtest/core/MidiInstrumentTest.h"
/* #include "MidiDeviceTest.cpp" */


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, InvertedMordent )
{
    variant v = variant::one;
	InvertedMordentPtr object = tgenInvertedMordent( v );
	stringstream expected;
	tgenInvertedMordentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, InvertedMordent )
{
    variant v = variant::two;
	InvertedMordentPtr object = tgenInvertedMordent( v );
	stringstream expected;
	tgenInvertedMordentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test03, InvertedMordent )
{
    variant v = variant::three;
	InvertedMordentPtr object = tgenInvertedMordent( v );
	stringstream expected;
	tgenInvertedMordentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}

namespace MxTestHelpers
{
    InvertedMordentPtr tgenInvertedMordent( variant v )
    {
        InvertedMordentPtr o = makeInvertedMordent();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasBeats = true;
                o->getAttributes()->beats = TrillBeats( 1.2 );
                o->getAttributes()->hasApproach = true;
                o->getAttributes()->approach = AboveBelow::above;
                o->getAttributes()->hasAccelerate = true;
                o->getAttributes()->accelerate = YesNo::yes;
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasDeparture = true;
                o->getAttributes()->departure = AboveBelow::below;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenInvertedMordentExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<inverted-mordent/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<inverted-mordent accelerate="yes" beats="2" approach="above"/>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<inverted-mordent departure="below"/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
