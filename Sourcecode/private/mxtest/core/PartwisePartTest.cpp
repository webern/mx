// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PartwisePartTest.h"
#include "mxtest/core/PartwiseMeasureTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PartwisePart )
{
    TestMode v = TestMode::one;
	PartwisePartPtr object = tgenPartwisePart( v );
	stringstream expected;
	tgenPartwisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, PartwisePart )
{
    TestMode v = TestMode::two;
	PartwisePartPtr object = tgenPartwisePart( v );
	stringstream expected;
	tgenPartwisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PartwisePart )
{
    TestMode v = TestMode::three;
	PartwisePartPtr object = tgenPartwisePart( v );
	stringstream expected;
	tgenPartwisePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PartwisePartPtr tgenPartwisePart( TestMode v )
    {
        PartwisePartPtr o = makePartwisePart();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->id = XsIDREF( "IDTWO" );
                o->addPartwiseMeasure( tgenPartwiseMeasure( v ) );
                o->removePartwiseMeasure( o->getPartwiseMeasureSet().cbegin() );
                o->addPartwiseMeasure( tgenPartwiseMeasure( TestMode::three ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->id = XsIDREF( "ID3" );
                o->addPartwiseMeasure( tgenPartwiseMeasure( v ) );
                o->removePartwiseMeasure( o->getPartwiseMeasureSet().cbegin() );
                o->addPartwiseMeasure( tgenPartwiseMeasure( TestMode::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartwisePartExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<part id="ID">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<part id="IDTWO">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwiseMeasureExpected(os, i+1, TestMode::three );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<part id="ID3">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwiseMeasureExpected(os, i+1, TestMode::two );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
