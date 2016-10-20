// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    FullNoteTypeChoicePtr tgenFullNoteTypeChoice( variant v )
    {
        FullNoteTypeChoicePtr o = makeFullNoteTypeChoice();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( FullNoteTypeChoice::Choice::unpitched );
                o->setUnpitched( tgenUnpitched( v ) );
            }
                break;
            case variant::three:
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
    void tgenFullNoteTypeChoiceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenPitchExpected( os, i, v );
            }
                break;
            case variant::two:
            {
                tgenUnpitchedExpected( os, i, v );
            }
                break;
            case variant::three:
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
