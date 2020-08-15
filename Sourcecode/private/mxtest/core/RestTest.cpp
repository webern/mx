// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/RestTest.h"
#include "mxtest/core/DisplayStepOctaveGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Rest )
{
    TestMode v = TestMode::one;
	RestPtr object = tgenRest( v );
	stringstream expected;
	tgenRestExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Rest )
{
    TestMode v = TestMode::two;
	RestPtr object = tgenRest( v );
	stringstream expected;
	tgenRestExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Rest )
{
    TestMode v = TestMode::three;
	RestPtr object = tgenRest( v );
	stringstream expected;
	tgenRestExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    RestPtr tgenRest( TestMode v )
    {
        RestPtr o = makeRest();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasMeasure = true;
                o->getAttributes()->measure = YesNo::yes;
                o->setHasDisplayStepOctaveGroup( true );
                o->setDisplayStepOctaveGroup( tgenDisplayStepOctaveGroup( v ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasMeasure = true;
                o->getAttributes()->measure = YesNo::no;
                o->setHasDisplayStepOctaveGroup( true );
                o->setDisplayStepOctaveGroup( tgenDisplayStepOctaveGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenRestExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<rest/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<rest measure="yes">)" );
                tgenDisplayStepOctaveGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</rest>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<rest measure="no">)" );
                tgenDisplayStepOctaveGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</rest>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
