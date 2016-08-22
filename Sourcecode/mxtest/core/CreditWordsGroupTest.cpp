// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/CreditWordsGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, CreditWordsGroup )
{
    variant v = variant::one;
	CreditWordsGroupPtr object = tgenCreditWordsGroup( v );
	stringstream expected;
	tgenCreditWordsGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, CreditWordsGroup )
{
    variant v = variant::two;
	CreditWordsGroupPtr object = tgenCreditWordsGroup( v );
	stringstream expected;
	tgenCreditWordsGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, CreditWordsGroup )
{
    variant v = variant::three;
	CreditWordsGroupPtr object = tgenCreditWordsGroup( v );
	stringstream expected;
	tgenCreditWordsGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    CreditWordsGroupPtr tgenCreditWordsGroup( variant v )
    {
        CreditWordsGroupPtr o = makeCreditWordsGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getCreditWords()->setValue( XsString( "Matthew James Briggs" ) );
                auto l1 = makeLink();
                l1->getAttributes()->href = XlinkHref("matthewjamesbriggs.com");
                o->addLink( l1 );
                auto b1 = makeBookmark();
                b1->getAttributes()->hasElement = true;
                b1->getAttributes()->element = XsNMToken( "NMToken1" );
                o->addBookmark( b1 );
            }
                break;
            case variant::three:
            {
                o->getCreditWords()->setValue( XsString( ".mjb" ) );
                auto l1 = makeLink();
                l1->getAttributes()->href = XlinkHref("matthewjamesbriggs.com");
                o->addLink( l1 );
                auto l2 = makeLink();
                l2->getAttributes()->href = XlinkHref("somethingelse.com");
                o->addLink( l2 );
                auto b1 = makeBookmark();
                b1->getAttributes()->hasElement = true;
                b1->getAttributes()->element = XsNMToken( "NMToken2" );
                o->addBookmark( b1 );
                auto b2 = makeBookmark();
                b2->getAttributes()->hasElement = true;
                b2->getAttributes()->element = XsNMToken( "NMToken3" );
                o->addBookmark( b2 );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenCreditWordsGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<credit-words></credit-words>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<link xlink:href="matthewjamesbriggs.com"/>)" );
                streamLine( os, i, R"(<bookmark id="ID" element="NMToken1"/>)" );
                streamLine( os, i, R"(<credit-words>Matthew James Briggs</credit-words>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<link xlink:href="matthewjamesbriggs.com"/>)" );
                streamLine( os, i, R"(<link xlink:href="somethingelse.com"/>)" );
                streamLine( os, i, R"(<bookmark id="ID" element="NMToken2"/>)" );
                streamLine( os, i, R"(<bookmark id="ID" element="NMToken3"/>)" );
                streamLine( os, i, R"(<credit-words>.mjb</credit-words>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
