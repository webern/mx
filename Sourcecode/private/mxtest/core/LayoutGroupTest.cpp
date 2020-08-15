// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
    TestMode v = TestMode::one;
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
    TestMode v = TestMode::two;
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
    TestMode v = TestMode::three;
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
    LayoutGroupPtr tgenLayoutGroup( TestMode v )
    {
        LayoutGroupPtr o = makeLayoutGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasPageLayout( true );
                o->setPageLayout( tgenPageLayout( v ) );
                o->setHasSystemLayout( true );
                o->setSystemLayout( tgenSystemLayout( v ) );
            }
                break;
            case TestMode::three:
            {
                o->addStaffLayout( tgenStaffLayout( TestMode::one ) );
                o->addStaffLayout( tgenStaffLayout( TestMode::two ) );
                o->addStaffLayout( tgenStaffLayout( TestMode::three ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenLayoutGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                tgenPageLayoutExpected( os, i, v );
                os << std::endl;
                tgenSystemLayoutExpected( os, i, v );
            }
                break;
            case TestMode::three:
            {
                tgenStaffLayoutExpected(os, i, TestMode::one );
                os << std::endl;
                tgenStaffLayoutExpected(os, i, TestMode::two );
                os << std::endl;
                tgenStaffLayoutExpected(os, i, TestMode::three );
            }
                break;
            default:
                break;
        }
    }
}

#endif
