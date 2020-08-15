// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/SystemMarginsTest.h"
#include "mxtest/core/PlayTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, SystemMargins )
{
    TestMode v = TestMode::one;
	SystemMarginsPtr object = tgenSystemMargins( v );
	stringstream expected;
	tgenSystemMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, SystemMargins )
{
    TestMode v = TestMode::two;
	SystemMarginsPtr object = tgenSystemMargins( v );
	stringstream expected;
	tgenSystemMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, SystemMargins )
{
    TestMode v = TestMode::three;
	SystemMarginsPtr object = tgenSystemMargins( v );
	stringstream expected;
	tgenSystemMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    SystemMarginsPtr tgenSystemMargins( TestMode v )
    {
        SystemMarginsPtr o = makeSystemMargins();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getLeftMargin()->setValue( TenthsValue( 11.1 ) );
                o->getRightMargin()->setValue( TenthsValue( 12.2 ) );
            }
                break;
            case TestMode::three:
            {
                o->getLeftMargin()->setValue( TenthsValue( 110.1 ) );
                o->getRightMargin()->setValue( TenthsValue( 120.2 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenSystemMarginsExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<system-margins>)" );
                streamLine( os, i+1, R"(<left-margin>0</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>0</right-margin>)" );
                streamLine( os, i, R"(</system-margins>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<system-margins>)" );
                streamLine( os, i+1, R"(<left-margin>11.1</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>12.2</right-margin>)" );;
                streamLine( os, i, R"(</system-margins>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<system-margins>)" );
                streamLine( os, i+1, R"(<left-margin>110.1</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>120.2</right-margin>)" );
                streamLine( os, i, R"(</system-margins>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
