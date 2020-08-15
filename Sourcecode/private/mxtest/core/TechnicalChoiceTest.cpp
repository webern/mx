// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TechnicalChoiceTest.h"
#include "mxtest/core/HarmonicTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TechnicalChoice )
{
    TestMode v = TestMode::one;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, TechnicalChoice )
{
    TestMode v = TestMode::two;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TechnicalChoice )
{
    TestMode v = TestMode::three;
	TechnicalChoicePtr object = tgenTechnicalChoice( v );
	stringstream expected;
	tgenTechnicalChoiceExpected( expected, 1, v );
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
    TechnicalChoicePtr tgenTechnicalChoice( TestMode v )
    {
        TechnicalChoicePtr o = makeTechnicalChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( TechnicalChoice::Choice::string_ );
                o->getString()->setValue( StringNumber( 4 ) );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( TechnicalChoice::Choice::harmonic );
                o->setHarmonic( tgenHarmonic( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTechnicalChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<up-bow/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<string>4</string>)", false );
            }
                break;
            case TestMode::three:
            {
                tgenHarmonicExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
