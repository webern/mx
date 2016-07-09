// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
using namespace MxTestHelpers;

TEST( Test01, BendChoice )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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

namespace MxTestHelpers
{
    BendChoicePtr tgenBendChoice( variant v )
    {
        BendChoicePtr o = makeBendChoice();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( BendChoice::Choice::release );
            }
                break;
            case variant::three:
            {
                o->setChoice( BendChoice::Choice::preBend );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenBendChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<pre-bend/>)", false );
            }
                break;
            case variant::two:
            {
                
                streamLine( os, i, R"(<release/>)", false );
            }
                break;
            case variant::three:
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
