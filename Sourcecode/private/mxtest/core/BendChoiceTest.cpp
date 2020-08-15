// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/BendChoiceTest.h"
#include "mxtest/core/ScalingTest.h"
#include "mxtest/core/LayoutGroupTest.h"
#include "mxtest/core/AppearanceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, BendChoice )
{
    TestMode v = TestMode::one;
	BendChoicePtr object = tgenBendChoice( v );
	stringstream expected;
	tgenBendChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, BendChoice )
{
    TestMode v = TestMode::two;
	BendChoicePtr object = tgenBendChoice( v );
	stringstream expected;
	tgenBendChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, BendChoice )
{
    TestMode v = TestMode::three;
	BendChoicePtr object = tgenBendChoice( v );
	stringstream expected;
	tgenBendChoiceExpected( expected, 1, v );
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
    BendChoicePtr tgenBendChoice( TestMode v )
    {
        BendChoicePtr o = makeBendChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( BendChoice::Choice::release );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( BendChoice::Choice::preBend );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenBendChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<pre-bend/>)", false );
            }
                break;
            case TestMode::two:
            {
                
                streamLine( os, i, R"(<release/>)", false );
            }
                break;
            case TestMode::three:
            {
                
                streamLine( os, i, R"(<pre-bend/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
