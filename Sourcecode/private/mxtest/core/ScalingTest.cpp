// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ScalingTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Scaling )
{
    TestMode v = TestMode::one;
	ScalingPtr object = tgenScaling( v );
	stringstream expected;
	tgenScalingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Scaling )
{
    TestMode v = TestMode::two;
	ScalingPtr object = tgenScaling( v );
	stringstream expected;
	tgenScalingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Scaling )
{
    TestMode v = TestMode::three;
	ScalingPtr object = tgenScaling( v );
	stringstream expected;
	tgenScalingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ScalingPtr tgenScaling( TestMode v )
    {
        ScalingPtr o = makeScaling();
        switch ( v )
        {
            case TestMode::one:
            {

            }
                break;
            case TestMode::two:
            {
                o->getMillimeters()->setValue( MillimetersValue( 9 ) );
                o->getTenths()->setValue( TenthsValue( 42 ) );
            }
                break;
            case TestMode::three:
            {
                o->getMillimeters()->setValue( MillimetersValue( 11 ) );
                o->getTenths()->setValue( TenthsValue( 44 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScalingExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<scaling>)" );
                streamLine( os, i+1, R"(<millimeters>7</millimeters>)" );
                streamLine( os, i+1, R"(<tenths>40</tenths>)" );
                streamLine( os, i, R"(</scaling>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<scaling>)" );
                streamLine( os, i+1, R"(<millimeters>9</millimeters>)" );
                streamLine( os, i+1, R"(<tenths>42</tenths>)" );
                streamLine( os, i, R"(</scaling>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<scaling>)" );
                streamLine( os, i+1, R"(<millimeters>11</millimeters>)" );
                streamLine( os, i+1, R"(<tenths>44</tenths>)" );
                streamLine( os, i, R"(</scaling>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
