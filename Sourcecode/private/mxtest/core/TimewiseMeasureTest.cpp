// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TimewiseMeasureTest.h"
#include "mxtest/core/TimewisePartTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TimewiseMeasure )
{
    TestMode v = TestMode::one;
	TimewiseMeasurePtr object = tgenTimewiseMeasure( v );
	stringstream expected;
	tgenTimewiseMeasureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, TimewiseMeasure )
{
    TestMode v = TestMode::two;
	TimewiseMeasurePtr object = tgenTimewiseMeasure( v );
	stringstream expected;
	tgenTimewiseMeasureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TimewiseMeasure )
{
    TestMode v = TestMode::three;
	TimewiseMeasurePtr object = tgenTimewiseMeasure( v );
	stringstream expected;
	tgenTimewiseMeasureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TimewiseMeasurePtr tgenTimewiseMeasure( TestMode v )
    {
        TimewiseMeasurePtr o = makeTimewiseMeasure();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->number = XsToken( "215" );
                o->addTimewisePart( tgenTimewisePart( v ) );
                o->removeTimewisePart( o->getTimewisePartSet().cbegin() );
                o->addTimewisePart( tgenTimewisePart( TestMode::three ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->number = XsToken( "105" );
                o->getAttributes()->hasImplicit = true;
                o->getAttributes()->implicit = YesNo::no;
                o->addTimewisePart( tgenTimewisePart( v ) );
                o->removeTimewisePart( o->getTimewisePartSet().cbegin() );
                o->addTimewisePart( tgenTimewisePart( TestMode::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTimewiseMeasureExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<measure number="">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</measure>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<measure number="215">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewisePartExpected(os, i+1, TestMode::three );
                os << std::endl;
                streamLine( os, i, R"(</measure>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<measure number="105" implicit="no">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewisePartExpected(os, i+1, TestMode::two );
                os << std::endl;
                streamLine( os, i, R"(</measure>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
