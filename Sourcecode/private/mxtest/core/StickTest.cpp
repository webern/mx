// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/StickTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Stick )
{
	Stick object;
	stringstream expected;
	tgenStickExpected(expected, 1, TestMode::one );
	stringstream actual;
	// object.toStream( std::cout, 1 );
	object.toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object.hasAttributes() )
	CHECK( object.hasContents() )
}
TEST( Test02, Stick )
{
	StickPtr object = tgenStick( TestMode::two );
	stringstream expected;
	tgenStickExpected(expected, 1, TestMode::two );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Stick )
{
	StickPtr object = tgenStick( TestMode::three );
	stringstream expected;
	tgenStickExpected(expected, 1, TestMode::three );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    StickPtr tgenStick( TestMode v )
    {
        StickPtr o = makeStick();
        switch ( v )
        {
            case TestMode::one:
            {
                ;
            }
                break;
            case TestMode::two:
            {
                o->getStickMaterial()->setValue( StickMaterialEnum::x );
                o->getStickType()->setValue( StickTypeEnum::xylophone );
                o->getAttributes()->hasTip = true;
                o->getAttributes()->tip = TipDirection::southwest;

            }
                break;
            case TestMode::three:
            {
                o->getStickMaterial()->setValue( StickMaterialEnum::shaded );
                o->getStickType()->setValue( StickTypeEnum::doubleBassDrum );
                o->getAttributes()->hasTip = true;
                o->getAttributes()->tip = TipDirection::up;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenStickExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<stick>)" );
                streamLine( os, i+1, R"(<stick-type>yarn</stick-type>)" );
                streamLine( os, i+1, R"(<stick-material>medium</stick-material>)" );
                streamLine( os, i, R"(</stick>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<stick tip="southwest">)" );
                streamLine( os, i+1, R"(<stick-type>xylophone</stick-type>)" );
                streamLine( os, i+1, R"(<stick-material>x</stick-material>)" );
                streamLine( os, i, R"(</stick>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<stick tip="up">)" );
                streamLine( os, i+1, R"(<stick-type>double bass drum</stick-type>)" );
                streamLine( os, i+1, R"(<stick-material>shaded</stick-material>)" );
                streamLine( os, i, R"(</stick>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
