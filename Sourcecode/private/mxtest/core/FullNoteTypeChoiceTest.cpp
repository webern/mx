// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/FullNoteTypeChoiceTest.h"
#include "mxtest/core/PitchTest.h"
#include "mxtest/core/RestTest.h"
#include "mxtest/core/UnpitchedTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, FullNoteTypeChoice )
{
    TestMode v = TestMode::one;
	FullNoteTypeChoicePtr object = tgenFullNoteTypeChoice( v );
	stringstream expected;
	tgenFullNoteTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, FullNoteTypeChoice )
{
    TestMode v = TestMode::two;
	FullNoteTypeChoicePtr object = tgenFullNoteTypeChoice( v );
	stringstream expected;
	tgenFullNoteTypeChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, FullNoteTypeChoice )
{
    TestMode v = TestMode::three;
	FullNoteTypeChoicePtr object = tgenFullNoteTypeChoice( v );
	stringstream expected;
	tgenFullNoteTypeChoiceExpected( expected, 1, v );
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
    FullNoteTypeChoicePtr tgenFullNoteTypeChoice( TestMode v )
    {
        FullNoteTypeChoicePtr o = makeFullNoteTypeChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( FullNoteTypeChoice::Choice::unpitched );
                o->setUnpitched( tgenUnpitched( v ) );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( FullNoteTypeChoice::Choice::rest );
                o->setRest( tgenRest( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenFullNoteTypeChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenPitchExpected( os, i, v );
            }
                break;
            case TestMode::two:
            {
                tgenUnpitchedExpected( os, i, v );
            }
                break;
            case TestMode::three:
            {
                tgenRestExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
