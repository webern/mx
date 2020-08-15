// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/HarmonicTypeChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, HarmonicTypeChoice )
{
    TestMode v = TestMode::one;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, HarmonicTypeChoice )
{
    TestMode v = TestMode::two;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, HarmonicTypeChoice )
{
    TestMode v = TestMode::three;
	HarmonicTypeChoicePtr object = tgenHarmonicTypeChoice( v );
	stringstream expected;
	tgenHarmonicTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    HarmonicTypeChoicePtr tgenHarmonicTypeChoice( TestMode v )
    {
        HarmonicTypeChoicePtr o = makeHarmonicTypeChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( HarmonicTypeChoice::Choice::artificial );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( HarmonicTypeChoice::Choice::natural );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHarmonicTypeChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<natural/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<artificial/>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<natural/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
