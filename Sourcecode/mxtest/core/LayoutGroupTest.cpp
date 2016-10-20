// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/LayoutGroupTest.h"
#include "mxtest/core/SystemLayoutTest.h"
#include "mxtest/core/StaffLayoutTest.h"
#include "mxtest/core/PageLayoutTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, LayoutGroup )
{
    variant v = variant::one;
	LayoutGroupPtr object = tgenLayoutGroup( v );
	stringstream expected;
	tgenLayoutGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
    CHECK( isOneLineOnly )
}
TEST( Test02, LayoutGroup )
{
    variant v = variant::two;
	LayoutGroupPtr object = tgenLayoutGroup( v );
	stringstream expected;
	tgenLayoutGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, LayoutGroup )
{
    variant v = variant::three;
	LayoutGroupPtr object = tgenLayoutGroup( v );
	stringstream expected;
	tgenLayoutGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = false;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    LayoutGroupPtr tgenLayoutGroup( variant v )
    {
        LayoutGroupPtr o = makeLayoutGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasPageLayout( true );
                o->setPageLayout( tgenPageLayout( v ) );
                o->setHasSystemLayout( true );
                o->setSystemLayout( tgenSystemLayout( v ) );
            }
                break;
            case variant::three:
            {
                o->addStaffLayout( tgenStaffLayout( variant::one ) );
                o->addStaffLayout( tgenStaffLayout( variant::two ) );
                o->addStaffLayout( tgenStaffLayout( variant::three ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenLayoutGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                tgenPageLayoutExpected( os, i, v );
                os << std::endl;
                tgenSystemLayoutExpected( os, i, v );
            }
                break;
            case variant::three:
            {
                tgenStaffLayoutExpected( os, i, variant::one );
                os << std::endl;
                tgenStaffLayoutExpected( os, i, variant::two );
                os << std::endl;
                tgenStaffLayoutExpected( os, i, variant::three );
            }
                break;
            default:
                break;
        }
    }
}

#endif
