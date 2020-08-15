// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/HarmonicInfoChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, HarmonicInfoChoice )
{
    TestMode v = TestMode::one;
	HarmonicInfoChoicePtr object = tgenHarmonicInfoChoice( v );
	stringstream expected;
	tgenHarmonicInfoChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, HarmonicInfoChoice )
{
    TestMode v = TestMode::two;
	HarmonicInfoChoicePtr object = tgenHarmonicInfoChoice( v );
	stringstream expected;
	tgenHarmonicInfoChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, HarmonicInfoChoice )
{
    TestMode v = TestMode::three;
	HarmonicInfoChoicePtr object = tgenHarmonicInfoChoice( v );
	stringstream expected;
	tgenHarmonicInfoChoiceExpected( expected, 1, v );
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
    HarmonicInfoChoicePtr tgenHarmonicInfoChoice( TestMode v )
    {
        HarmonicInfoChoicePtr o = makeHarmonicInfoChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( HarmonicInfoChoice::Choice::touchingPitch );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( HarmonicInfoChoice::Choice::soundingPitch );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHarmonicInfoChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<base-pitch/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<touching-pitch/>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<sounding-pitch/>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
