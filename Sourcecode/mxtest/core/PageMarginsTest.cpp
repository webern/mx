// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/PageMarginsTest.h"
#include "mxtest/core/PlayTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PageMargins )
{
    variant v = variant::one;
	PageMarginsPtr object = tgenPageMargins( v );
	stringstream expected;
	tgenPageMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, PageMargins )
{
    variant v = variant::two;
	PageMarginsPtr object = tgenPageMargins( v );
	stringstream expected;
	tgenPageMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PageMargins )
{
    variant v = variant::three;
	PageMarginsPtr object = tgenPageMargins( v );
	stringstream expected;
	tgenPageMarginsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PageMarginsPtr tgenPageMargins( variant v )
    {
        PageMarginsPtr o = makePageMargins();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getLeftMargin()->setValue( TenthsValue( 11.1 ) );
                o->getRightMargin()->setValue( TenthsValue( 12.2 ) );
                o->getTopMargin()->setValue( TenthsValue( 13.3 ) );
                o->getBottomMargin()->setValue( TenthsValue( 14.4 ) );
                o->getAttributes()->hasType = true;
            }
                break;
            case variant::three:
            {
                o->getLeftMargin()->setValue( TenthsValue( 110.1 ) );
                o->getRightMargin()->setValue( TenthsValue( 120.2 ) );
                o->getTopMargin()->setValue( TenthsValue( 130.3 ) );
                o->getBottomMargin()->setValue( TenthsValue( 140.4 ) );
                o->getAttributes()->hasType = true;
                o->getAttributes()->type = MarginType::even;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPageMarginsExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<page-margins>)" );
                streamLine( os, i+1, R"(<left-margin>0</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>0</right-margin>)" );
                streamLine( os, i+1, R"(<top-margin>0</top-margin>)" );
                streamLine( os, i+1, R"(<bottom-margin>0</bottom-margin>)" );
                streamLine( os, i, R"(</page-margins>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<page-margins type="both">)" );
                streamLine( os, i+1, R"(<left-margin>11.1</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>12.2</right-margin>)" );
                streamLine( os, i+1, R"(<top-margin>13.3</top-margin>)" );
                streamLine( os, i+1, R"(<bottom-margin>14.4</bottom-margin>)" );
                streamLine( os, i, R"(</page-margins>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<page-margins type="even">)" );
                streamLine( os, i+1, R"(<left-margin>110.1</left-margin>)" );
                streamLine( os, i+1, R"(<right-margin>120.2</right-margin>)" );
                streamLine( os, i+1, R"(<top-margin>130.3</top-margin>)" );
                streamLine( os, i+1, R"(<bottom-margin>140.4</bottom-margin>)" );
                streamLine( os, i, R"(</page-margins>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
