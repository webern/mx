// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ForwardTest.h"
#include "mxtest/core/EditorialVoiceGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Forward )
{
    variant v = variant::one;
	ForwardPtr object = tgenForward( v );
	stringstream expected;
	tgenForwardExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Forward )
{
    variant v = variant::two;
	ForwardPtr object = tgenForward( v );
	stringstream expected;
	tgenForwardExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Forward )
{
    variant v = variant::three;
	ForwardPtr object = tgenForward( v );
	stringstream expected;
	tgenForwardExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ForwardPtr tgenForward( variant v )
    {
        ForwardPtr o = makeForward();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getDuration()->setValue( PositiveDivisionsValue( 31 ) );
                o->setEditorialVoiceGroup( tgenEditorialVoiceGroup( v ) );
            }
                break;
            case variant::three:
            {
                o->getDuration()->setValue( PositiveDivisionsValue( 32 ) );
                o->setEditorialVoiceGroup( tgenEditorialVoiceGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenForwardExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<forward>)" );
                streamLine( os, i+1, R"(<duration>1</duration>)" );
                streamLine( os, i, R"(</forward>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<forward>)" );
                streamLine( os, i+1, R"(<duration>31</duration>)" );
                tgenEditorialVoiceGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</forward>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<forward>)" );
                streamLine( os, i+1, R"(<duration>32</duration>)" );
                tgenEditorialVoiceGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</forward>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
