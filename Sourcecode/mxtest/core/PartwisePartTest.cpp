// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    PartwisePartPtr tgenPartwisePart( variant v )
    {
        PartwisePartPtr o = makePartwisePart();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->id = XsIDREF( "IDTWO" );
                o->addPartwiseMeasure( tgenPartwiseMeasure( v ) );
                o->removePartwiseMeasure( o->getPartwiseMeasureSet().cbegin() );
                o->addPartwiseMeasure( tgenPartwiseMeasure( variant::three ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->id = XsIDREF( "ID3" );
                o->addPartwiseMeasure( tgenPartwiseMeasure( v ) );
                o->removePartwiseMeasure( o->getPartwiseMeasureSet().cbegin() );
                o->addPartwiseMeasure( tgenPartwiseMeasure( variant::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartwisePartExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<part id="ID">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<part id="IDTWO">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwiseMeasureExpected( os, i+1,  variant::three );
                os << std::endl;
                streamLine( os, i, R"(</part>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<part id="ID3">)" );
                tgenPartwiseMeasureExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwiseMeasureExpected( os, i+1,  variant::two );
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
