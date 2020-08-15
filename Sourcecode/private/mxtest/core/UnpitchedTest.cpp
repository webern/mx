// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/UnpitchedTest.h"
#include "mxtest/core/DisplayStepOctaveGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Unpitched )
{
    TestMode v = TestMode::one;
	UnpitchedPtr object = tgenUnpitched( v );
	stringstream expected;
	tgenUnpitchedExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Unpitched )
{
    TestMode v = TestMode::two;
	UnpitchedPtr object = tgenUnpitched( v );
	stringstream expected;
	tgenUnpitchedExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Unpitched )
{
    TestMode v = TestMode::three;
	UnpitchedPtr object = tgenUnpitched( v );
	stringstream expected;
	tgenUnpitchedExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    UnpitchedPtr tgenUnpitched( TestMode v )
    {
        UnpitchedPtr o = makeUnpitched();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasDisplayStepOctaveGroup( true );
                o->setDisplayStepOctaveGroup( tgenDisplayStepOctaveGroup( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setHasDisplayStepOctaveGroup( true );
                o->setDisplayStepOctaveGroup( tgenDisplayStepOctaveGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenUnpitchedExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<unpitched/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<unpitched>)" );
                tgenDisplayStepOctaveGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</unpitched>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<unpitched>)" );
                tgenDisplayStepOctaveGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</unpitched>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
