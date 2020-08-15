// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/GroupingTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Grouping )
{
    TestMode v = TestMode::one;
	GroupingPtr object = tgenGrouping( v );
	stringstream expected;
	tgenGroupingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Grouping )
{
    TestMode v = TestMode::two;
	GroupingPtr object = tgenGrouping( v );
	stringstream expected;
	tgenGroupingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Grouping )
{
    TestMode v = TestMode::three;
	GroupingPtr object = tgenGrouping( v );
	stringstream expected;
	tgenGroupingExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    GroupingPtr tgenGrouping( TestMode v )
    {
        GroupingPtr o = makeGrouping();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasNumber = true;
                auto f1 = makeFeature();
                f1->setValue( XsString( "Schenker" ) );
                f1->getAttributes()->hasType = true;
                f1->getAttributes()->type = XsToken( "Type Two" );
                o->addFeature( f1 );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasMemberOf = true;
                o->getAttributes()->type = StartStopSingle::stop;
                o->getAttributes()->memberOf = XsToken( "Some Member" );
                auto f1 = makeFeature();
                f1->setValue( XsString( "Strauss" ) );
                f1->getAttributes()->hasType = true;
                f1->getAttributes()->type = XsToken( "Type Three" );
                o->addFeature( f1 );
                auto f2 = makeFeature();
                f2->setValue( XsString( "Bones" ) );
                o->addFeature( f2 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenGroupingExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<grouping type="single"/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<grouping type="single" number="1">)" );
                streamLine( os, i+1, R"(<feature type="Type Two">Schenker</feature>)" );
                streamLine( os, i, R"(</grouping>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<grouping type="stop" member-of="Some Member">)" );
                streamLine( os, i+1, R"(<feature type="Type Three">Strauss</feature>)" );
                streamLine( os, i+1, R"(<feature>Bones</feature>)" );
                streamLine( os, i, R"(</grouping>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
