#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TimewiseMeasureTest.h"
#include "mxtest/core/TimewisePartTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, TimewiseMeasure )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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

namespace MxTestHelpers
{
    TimewiseMeasurePtr tgenTimewiseMeasure( variant v )
    {
        TimewiseMeasurePtr o = makeTimewiseMeasure();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->number = XsToken( "215" );
                o->addTimewisePart( tgenTimewisePart( v ) );
                o->removeTimewisePart( o->getTimewisePartSet().cbegin() );
                o->addTimewisePart( tgenTimewisePart( variant::three ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->number = XsToken( "105" );
                o->getAttributes()->hasImplicit = true;
                o->getAttributes()->implicit = YesNo::no;
                o->addTimewisePart( tgenTimewisePart( v ) );
                o->removeTimewisePart( o->getTimewisePartSet().cbegin() );
                o->addTimewisePart( tgenTimewisePart( variant::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTimewiseMeasureExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<measure number="">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                streamLine( os, i, R"(</measure>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<measure number="215">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewisePartExpected( os, i+1,  variant::three );
                os << std::endl;
                streamLine( os, i, R"(</measure>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<measure number="105" implicit="no">)" );
                tgenTimewisePartExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewisePartExpected( os, i+1,  variant::two );
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
