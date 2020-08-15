// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/VirtualInstrumentTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, VirtualInstrument )
{
    TestMode v = TestMode::one;
	VirtualInstrumentPtr object = tgenVirtualInstrument( v );
	stringstream expected;
	tgenVirtualInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, VirtualInstrument )
{
    TestMode v = TestMode::two;
	VirtualInstrumentPtr object = tgenVirtualInstrument( v );
	stringstream expected;
	tgenVirtualInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, VirtualInstrument )
{
    TestMode v = TestMode::three;
	VirtualInstrumentPtr object = tgenVirtualInstrument( v );
	stringstream expected;
	tgenVirtualInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    VirtualInstrumentPtr tgenVirtualInstrument( TestMode v )
    {
        VirtualInstrumentPtr o = makeVirtualInstrument();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasVirtualName( true );
                o->getVirtualName()->setValue( XsString( "VI1" ) );
            }
                break;
            case TestMode::three:
            {
                o->setHasVirtualName( true );
                o->getVirtualName()->setValue( XsString( "VI2" ) );
                o->setHasVirtualLibrary( true );
                o->getVirtualLibrary()->setValue( XsString( "VL2" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenVirtualInstrumentExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<virtual-instrument/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<virtual-instrument>)" );
                streamLine( os, i+1, R"(<virtual-name>VI1</virtual-name>)" );
                streamLine( os, i, R"(</virtual-instrument>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<virtual-instrument>)" );
                streamLine( os, i+1, R"(<virtual-library>VL2</virtual-library>)" );
                streamLine( os, i+1, R"(<virtual-name>VI2</virtual-name>)" );
                streamLine( os, i, R"(</virtual-instrument>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
