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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    ScordaturaPtr tgenScordatura( variant v )
    {
        ScordaturaPtr o = makeScordatura();
        switch ( v )
        {
            case variant::one:
            {
                ;
            }
                break;
            case variant::two:
            {
                o->addAccord( tgenAccord( variant::two ) );
                o->removeAccord( o->getAccordSet().cbegin() );
                o->addAccord( tgenAccord( variant::three ) );
            }
                break;
            case variant::three:
            {
                o->addAccord( tgenAccord( variant::three ) );
                o->removeAccord( o->getAccordSet().cbegin() );
                o->addAccord( tgenAccord( variant::two ) );
                o->addAccord( tgenAccord( variant::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScordaturaExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected( os, i+1, variant::one );
                os << std::endl;
                streamLine( os, i, R"(</scordatura>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected( os, i+1, variant::two );
                os << std::endl;
                tgenAccordExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i, R"(</scordatura>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<scordatura>)" );
                tgenAccordExpected( os, i+1, variant::three );
                os << std::endl;
                tgenAccordExpected( os, i+1, variant::two );
                os << std::endl;
                tgenAccordExpected( os, i+1, variant::one );
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
