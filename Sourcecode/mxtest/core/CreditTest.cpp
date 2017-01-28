// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/CreditTest.h"
#include "mxtest/core/CreditChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Credit )
{
    variant v = variant::one;
	CreditPtr object = tgenCredit( v );
	stringstream expected;
	tgenCreditExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Credit )
{
    variant v = variant::two;
	CreditPtr object = tgenCredit( v );
	stringstream expected;
	tgenCreditExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Credit )
{
    variant v = variant::three;
	CreditPtr object = tgenCredit( v );
	stringstream expected;
	tgenCreditExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    CreditPtr tgenCredit( variant v )
    {
        CreditPtr o = makeCredit();
        switch ( v )
        {
            case variant::one:
            {
                o->setCreditChoice( tgenCreditChoice( v ) );
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasPage = true;
                o->getAttributes()->page = PositiveInteger( 2 );
                o->setCreditChoice( tgenCreditChoice( v ) );
                auto l1 = makeLink();
                l1->getAttributes()->href = XlinkHref("hfrefABC");
                o->addLink( l1 );
                auto l2 = makeLink();
                l2->getAttributes()->href = XlinkHref("hrefDEF");
                o->addLink( l2 );
                auto b1 = makeBookmark();
                b1->getAttributes()->hasElement = true;
                b1->getAttributes()->element = XsNMToken( "element1 23" );
                o->addBookmark( b1 );
                auto b2 = makeBookmark();
                b2->getAttributes()->hasElement = true;
                b2->getAttributes()->element = XsNMToken( "elemtn673" );
                o->addBookmark( b2 );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasPage = true;
                o->getAttributes()->page = PositiveInteger( 3 );
                o->setCreditChoice( tgenCreditChoice( v ) );
                auto l1 = makeLink();
                l1->getAttributes()->href = XlinkHref("sdfljkhsldjfkg");
                o->addLink( l1 );
                auto b1 = makeBookmark();
                b1->getAttributes()->hasElement = true;
                b1->getAttributes()->element = XsNMToken( "sdf89g7sd0f67g" );
                o->addBookmark( b1 );
                auto t1 = makeCreditType();
                t1->setValue( XsString( "composer" ) );
                o->addCreditType( t1 );
                t1 = makeCreditType();
                t1->setValue( XsString( "lyricist" ) );
                o->addCreditType( t1 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenCreditExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<credit>)" );
                tgenCreditChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</credit>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<credit page="2">)" );
                streamLine( os, i+1, R"(<link xlink:href="hfrefABC"/>)" );
                streamLine( os, i+1, R"(<link xlink:href="hrefDEF"/>)" );
                streamLine( os, i+1, R"(<bookmark id="ID" element="element1 23"/>)" );
                streamLine( os, i+1, R"(<bookmark id="ID" element="elemtn673"/>)" );
                tgenCreditChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</credit>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<credit page="3">)" );
                streamLine( os, i+1, R"(<credit-type>composer</credit-type>)" );
                streamLine( os, i+1, R"(<credit-type>lyricist</credit-type>)" );
                streamLine( os, i+1, R"(<link xlink:href="sdfljkhsldjfkg"/>)" );
                streamLine( os, i+1, R"(<bookmark id="ID" element="sdf89g7sd0f67g"/>)" );
                tgenCreditChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</credit>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
