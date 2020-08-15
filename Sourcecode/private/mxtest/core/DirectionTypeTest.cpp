// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/DirectionTypeTest.h"
#include "mxtest/core/PercussionTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, DirectionType )
{
    TestMode v = TestMode::one;
	DirectionTypePtr object = tgenDirectionType( v );
	stringstream expected;
	tgenDirectionTypeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, DirectionType )
{
    TestMode v = TestMode::two;
	DirectionTypePtr object = tgenDirectionType( v );
	stringstream expected;
	tgenDirectionTypeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, DirectionType )
{
    TestMode v = TestMode::three;
	DirectionTypePtr object = tgenDirectionType( v );
	stringstream expected;
	tgenDirectionTypeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    DirectionTypePtr tgenDirectionType( TestMode v )
    {
        DirectionTypePtr o = makeDirectionType();
        switch ( v )
        {
            case TestMode::one:
            {
                (*o->getRehearsalSet().cbegin())->setValue( XsString( "A" ) );
            }
                break;
            case TestMode::two:
            {
                o->setChoice( DirectionType::Choice::pedal );
                o->getPedal()->getAttributes()->hasLine = true;
                o->getPedal()->getAttributes()->line = YesNo::yes;
            }
                break;
            case TestMode::three:
            {
                PercussionPtr p1 = tgenPercussion( TestMode::two );
                PercussionPtr p2 = tgenPercussion( TestMode::three );
                o->setChoice( DirectionType::Choice::percussion );
                o->addPercussion( p1 );
                o->removePercussion( o->getPercussionSet().cbegin() );
                o->addPercussion( p2 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDirectionTypeExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<direction-type>)" );
                streamLine( os, i+1, R"(<rehearsal>A</rehearsal>)" );
                streamLine( os, i, R"(</direction-type>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<direction-type>)" );
                streamLine( os, i+1, R"(<pedal type="start" line="yes"/>)" );
                streamLine( os, i, R"(</direction-type>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<direction-type>)" );
                tgenPercussionExpected(os, i+1, TestMode::two );
                os << std::endl;
                tgenPercussionExpected(os, i+1, TestMode::three );
                os << std::endl;
                streamLine( os, i, R"(</direction-type>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
