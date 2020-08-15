// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/AccordTest.h"
#include "mxtest/core/ScordaturaTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Scordatura )
{
    TestMode v = TestMode::one;
	ScordaturaPtr object = tgenScordatura( v );
	stringstream expected;
	tgenScordaturaExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Scordatura )
{
    TestMode v = TestMode::two;
	ScordaturaPtr object = tgenScordatura( v );
	stringstream expected;
	tgenScordaturaExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Scordatura )
{
    TestMode v = TestMode::three;
	ScordaturaPtr object = tgenScordatura( v );
	stringstream expected;
	tgenScordaturaExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ScordaturaPtr tgenScordatura( TestMode v )
    {
        ScordaturaPtr o = makeScordatura();
        switch ( v )
        {
            case TestMode::one:
            {
                ;
            }
                break;
            case TestMode::two:
            {
                o->addAccord( tgenAccord( TestMode::two ) );
                o->removeAccord( o->getAccordSet().cbegin() );
                o->addAccord( tgenAccord( TestMode::three ) );
            }
                break;
            case TestMode::three:
            {
                o->addAccord( tgenAccord( TestMode::three ) );
                o->removeAccord( o->getAccordSet().cbegin() );
                o->addAccord( tgenAccord( TestMode::two ) );
                o->addAccord( tgenAccord( TestMode::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScordaturaExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected(os, i+1, TestMode::one );
                os << std::endl;
                streamLine( os, i, R"(</scordatura>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected(os, i+1, TestMode::two );
                os << std::endl;
                tgenAccordExpected(os, i+1, TestMode::three );
                os << std::endl;
                streamLine( os, i, R"(</scordatura>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected(os, i+1, TestMode::three );
                os << std::endl;
                tgenAccordExpected(os, i+1, TestMode::two );
                os << std::endl;
                tgenAccordExpected(os, i+1, TestMode::one );
                os << std::endl;
                streamLine( os, i, R"(</scordatura>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
