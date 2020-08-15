// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/StrongAccentTest.h"
#include "mxtest/core/PlayTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, StrongAccent )
{
    TestMode v = TestMode::one;
	StrongAccentPtr object = tgenStrongAccent( v );
	stringstream expected;
	tgenStrongAccentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, StrongAccent )
{
    TestMode v = TestMode::two;
	StrongAccentPtr object = tgenStrongAccent( v );
	stringstream expected;
	tgenStrongAccentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test03, StrongAccent )
{
    TestMode v = TestMode::three;
	StrongAccentPtr object = tgenStrongAccent( v );
	stringstream expected;
	tgenStrongAccentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}

namespace mxtest
{
    StrongAccentPtr tgenStrongAccent( TestMode v )
    {
        StrongAccentPtr o = makeStrongAccent();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasFontStyle = true;
                o->getAttributes()->hasRelativeX = true;
                o->getAttributes()->relativeX = TenthsValue( -1.1 );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasDefaultY = true;
                o->getAttributes()->defaultY = TenthsValue( 2.2 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenStrongAccentExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<strong-accent/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<strong-accent relative-x="-1.1" font-style="normal"/>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<strong-accent default-y="2.2"/>)", false );

            }
                break;
            default:
                break;
        }
    }
}

#endif
