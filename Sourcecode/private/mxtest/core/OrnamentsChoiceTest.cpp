// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/OrnamentsChoiceTest.h"
#include "mxtest/core/ScalingTest.h"
#include "mxtest/core/LayoutGroupTest.h"
#include "mxtest/core/AppearanceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, OrnamentsChoice )
{
    TestMode v = TestMode::one;
	OrnamentsChoicePtr object = tgenOrnamentsChoice( v );
	stringstream expected;
	tgenOrnamentsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, OrnamentsChoice )
{
    TestMode v = TestMode::two;
	OrnamentsChoicePtr object = tgenOrnamentsChoice( v );
	stringstream expected;
	tgenOrnamentsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, OrnamentsChoice )
{
    TestMode v = TestMode::three;
	OrnamentsChoicePtr object = tgenOrnamentsChoice( v );
	stringstream expected;
	tgenOrnamentsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    OrnamentsChoicePtr tgenOrnamentsChoice( TestMode v )
    {
        OrnamentsChoicePtr o = makeOrnamentsChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                o->getTrillMark()->getAttributes()->hasFontStyle = true;
                o->getTrillMark()->getAttributes()->fontStyle = FontStyle::italic;
            }
                break;
            case TestMode::two:
            {
                o->setChoice( OrnamentsChoice::Choice::otherOrnament );
                o->getOtherOrnament()->setValue( XsString( "Mordiarty" ) );
                o->getOtherOrnament()->getAttributes()->hasPlacement = true;
            }
                break;
            case TestMode::three:
            {
                o->setChoice( OrnamentsChoice::Choice::shake );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenOrnamentsChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<trill-mark font-style="italic"/>)", false );
            }
                break;
            case TestMode::two:
            {

                streamLine( os, i, R"(<other-ornament placement="above">Mordiarty</other-ornament>)", false );
            }
                break;
            case TestMode::three:
            {

                streamLine( os, i, R"(<shake/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
