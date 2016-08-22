// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/MiscellaneousTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Miscellaneous )
{
    variant v = variant::one;
	MiscellaneousPtr object = tgenMiscellaneous( v );
	stringstream expected;
	tgenMiscellaneousExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Miscellaneous )
{
    variant v = variant::two;
	MiscellaneousPtr object = tgenMiscellaneous( v );
	stringstream expected;
	tgenMiscellaneousExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Miscellaneous )
{
    variant v = variant::three;
	MiscellaneousPtr object = tgenMiscellaneous( v );
	stringstream expected;
	tgenMiscellaneousExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    MiscellaneousPtr tgenMiscellaneous( variant v )
    {
        MiscellaneousPtr o = makeMiscellaneous();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                auto m1 = makeMiscellaneousField();
                m1->setValue( XsString( "Something" ) );
                m1->getAttributes()->name = XsToken( "one" );
                auto m2 = makeMiscellaneousField();
                m2->setValue( XsString( "Interesting" ) );
                m2->getAttributes()->name = XsToken( "two" );
                o->addMiscellaneousField( m1 );
                o->addMiscellaneousField( m2 );
            }
                break;
            case variant::three:
            {
                auto m1 = makeMiscellaneousField();
                m1->setValue( XsString( "One Line Only Here" ) );
                m1->getAttributes()->name = XsToken( "ricky" );
                o->addMiscellaneousField( m1 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenMiscellaneousExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<miscellaneous/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<miscellaneous>)" );
                streamLine( os, i+1, R"(<miscellaneous-field name="one">Something</miscellaneous-field>)" );
                streamLine( os, i+1, R"(<miscellaneous-field name="two">Interesting</miscellaneous-field>)" );
                streamLine( os, i, R"(</miscellaneous>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<miscellaneous>)" );
                streamLine( os, i+1, R"(<miscellaneous-field name="ricky">One Line Only Here</miscellaneous-field>)" );
                streamLine( os, i, R"(</miscellaneous>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
