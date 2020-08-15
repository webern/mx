// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/HoleTest.h"
#include "mxtest/core/MidiInstrumentTest.h"
/* #include "MidiDeviceTest.cpp" */


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Hole )
{
    TestMode v = TestMode::one;
	HolePtr object = tgenHole( v );
	stringstream expected;
	tgenHoleExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Hole )
{
    TestMode v = TestMode::two;
	HolePtr object = tgenHole( v );
	stringstream expected;
	tgenHoleExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Hole )
{
    TestMode v = TestMode::three;
	HolePtr object = tgenHole( v );
	stringstream expected;
	tgenHoleExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    HolePtr tgenHole( TestMode v )
    {
        HolePtr o = makeHole();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getHoleClosed()->setValue( HoleClosedValue::half );
                o->setHasHoleShape( true );
                o->getHoleShape()->setValue( XsString( "torus" ) );
                o->getAttributes()->hasColor = true;
                o->getAttributes()->color = Color( 1, 2, 3 );
            }
                break;
            case TestMode::three:
            {
                o->getHoleClosed()->setValue( HoleClosedValue::yes );
                o->setHasHoleShape( true );
                o->getHoleShape()->setValue( XsString( "smokes" ) );
                o->setHasHoleType( true );
                o->getHoleType()->setValue( XsString( "foobar" ) );
                o->getAttributes()->hasPlacement = true;
                o->getAttributes()->placement = AboveBelow::above;
                o->getAttributes()->hasDefaultY = true;
                o->getAttributes()->defaultY = TenthsValue( -1.1 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHoleExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<hole>)" );
                streamLine( os, i+1, R"(<hole-closed>no</hole-closed>)" );
                streamLine( os, i, R"(</hole>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<hole color="#010203">)" );
                streamLine( os, i+1, R"(<hole-closed>half</hole-closed>)" );
                streamLine( os, i+1, R"(<hole-shape>torus</hole-shape>)" );
                streamLine( os, i, R"(</hole>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<hole default-y="-1.1" placement="above">)" );
                streamLine( os, i+1, R"(<hole-type>foobar</hole-type>)" );
                streamLine( os, i+1, R"(<hole-closed>yes</hole-closed>)" );
                streamLine( os, i+1, R"(<hole-shape>smokes</hole-shape>)" );
                streamLine( os, i, R"(</hole>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
