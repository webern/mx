// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PrintTest.h"
#include "mxtest/core/LayoutGroupTest.h"
#include "mxtest/core/MeasureLayoutTest.h"
#include "mxtest/core/PartNameDisplayTest.h"
#include "mxtest/core/PartAbbreviationDisplayTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Print )
{
    variant v = variant::one;
	PrintPtr object = tgenPrint( v );
	stringstream expected;
	tgenPrintExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Print )
{
    variant v = variant::two;
	PrintPtr object = tgenPrint( v );
	stringstream expected;
	tgenPrintExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Print )
{
    variant v = variant::three;
	PrintPtr object = tgenPrint( v );
	stringstream expected;
	tgenPrintExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    PrintPtr tgenPrint( variant v )
    {
        PrintPtr o = makePrint();
        switch ( v )
        {
            case variant::one:
            {

            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasBlankPage = true;
                o->getAttributes()->blankPage = PositiveInteger( 2 );
                o->setLayoutGroup( tgenLayoutGroup( v ) );
                o->setHasMeasureLayout( true );
                o->setMeasureLayout( tgenMeasureLayout( v ) );
                o->setHasMeasureNumbering( true );
                o->getMeasureNumbering()->setValue( MeasureNumberingValue::system );
                o->setHasPartNameDisplay( true );
                o->setPartNameDisplay( tgenPartNameDisplay( v ) );
                o->setHasPartAbbreviationDisplay( true );
                o->setPartAbbreviationDisplay( tgenPartAbbreviationDisplay( v ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasNewPage = true;
                o->getAttributes()->newPage = YesNo::yes;
                o->getAttributes()->hasNewSystem = true;
                o->getAttributes()->newSystem = YesNo::no;
                o->setLayoutGroup( tgenLayoutGroup( v ) );
                o->setHasMeasureLayout( true );
                o->setMeasureLayout( tgenMeasureLayout( v ) );
                o->setHasMeasureNumbering( true );
                o->getMeasureNumbering()->setValue( MeasureNumberingValue::none );
                o->setHasPartNameDisplay( true );
                o->setPartNameDisplay( tgenPartNameDisplay( v ) );
                o->setHasPartAbbreviationDisplay( true );
                o->setPartAbbreviationDisplay( tgenPartAbbreviationDisplay( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPrintExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<print/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<print blank-page="2">)" );
                tgenLayoutGroupExpected( os, i+1, v );
                os << std::endl;
                tgenMeasureLayoutExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<measure-numbering>system</measure-numbering>)" );
                tgenPartNameDisplayExpected( os, i+1, v );
                os << std::endl;
                tgenPartAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</print>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<print new-system="no" new-page="yes">)" );
                tgenLayoutGroupExpected( os, i+1, v );
                os << std::endl;
                tgenMeasureLayoutExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<measure-numbering>none</measure-numbering>)" );
                tgenPartNameDisplayExpected( os, i+1, v );
                os << std::endl;
                tgenPartAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</print>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

