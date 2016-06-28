#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/StaffLayoutTest.h"
#include "mxtest/core/SystemMarginsTest.h"
#include "mxtest/core/SystemDividersTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, StaffLayout )
{
    variant v = variant::one;
	StaffLayoutPtr object = tgenStaffLayout( v );
	stringstream expected;
	tgenStaffLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, StaffLayout )
{
    variant v = variant::two;
	StaffLayoutPtr object = tgenStaffLayout( v );
	stringstream expected;
	tgenStaffLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, StaffLayout )
{
    variant v = variant::three;
	StaffLayoutPtr object = tgenStaffLayout( v );
	stringstream expected;
	tgenStaffLayoutExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    StaffLayoutPtr tgenStaffLayout( variant v )
    {
        StaffLayoutPtr o = makeStaffLayout();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasStaffDistance( true );
                o->getStaffDistance()->setValue( TenthsValue( 12.1 ) );
                o->getAttributes()->hasNumber = true;
                o->getAttributes()->number = StaffNumber( 5 );
            }
                break;
            case variant::three:
            {
                o->setHasStaffDistance( true );
                o->getStaffDistance()->setValue( TenthsValue( 13.3 ) );
                o->getAttributes()->hasNumber = true;
                o->getAttributes()->number = StaffNumber( 7 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenStaffLayoutExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<staff-layout/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<staff-layout number="5">)" );
                streamLine( os, i+1, R"(<staff-distance>12.1</staff-distance>)" );
                streamLine( os, i, R"(</staff-layout>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<staff-layout number="7">)" );
                streamLine( os, i+1, R"(<staff-distance>13.3</staff-distance>)" );
                streamLine( os, i, R"(</staff-layout>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
