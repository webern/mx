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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    UnpitchedPtr tgenUnpitched( variant v )
    {
        UnpitchedPtr o = makeUnpitched();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasDisplayStepOctaveGroup( true );
                o->setDisplayStepOctaveGroup( tgenDisplayStepOctaveGroup( v ) );
            }
                break;
            case variant::three:
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
    void tgenUnpitchedExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<unpitched/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<unpitched>)" );
                tgenDisplayStepOctaveGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</unpitched>)", false );
            }
                break;
            case variant::three:
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
