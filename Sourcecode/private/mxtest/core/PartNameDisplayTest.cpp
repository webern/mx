// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/PartNameDisplayTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PartNameDisplay )
{
    TestMode v = TestMode::one;
	PartNameDisplayPtr object = tgenPartNameDisplay( v );
	stringstream expected;
	tgenPartNameDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, PartNameDisplay )
{
    TestMode v = TestMode::two;
	PartNameDisplayPtr object = tgenPartNameDisplay( v );
	stringstream expected;
	tgenPartNameDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PartNameDisplay )
{
    TestMode v = TestMode::three;
	PartNameDisplayPtr object = tgenPartNameDisplay( v );
	stringstream expected;
	tgenPartNameDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PartNameDisplayPtr tgenPartNameDisplay( TestMode v )
    {
        PartNameDisplayPtr o = makePartNameDisplay();
        switch ( v )
        {
            case TestMode::one:
            {
                ;
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::no;
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::one ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::two ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::yes;
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::three ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::three ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( TestMode::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartNameDisplayExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<part-name-display></part-name-display>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<part-name-display print-object="no">)" );
                streamLine( os, i+1, R"(<accidental-text enclosure="oval">flat-flat</accidental-text>)" );
                streamLine( os, i+1, R"(<display-text justify="center">two</display-text>)" );
                streamLine( os, i, R"(</part-name-display>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<part-name-display print-object="yes">)" );
                streamLine( os, i+1, R"(<display-text justify="center">two</display-text>)" );
                streamLine( os, i+1, R"(<display-text font-weight="bold" halign="right">three</display-text>)" );
                streamLine( os, i+1, R"(<display-text justify="center">two</display-text>)" );
                streamLine( os, i+1, R"(<display-text font-weight="bold" halign="right">three</display-text>)" );
                streamLine( os, i+1, R"(<display-text justify="center">two</display-text>)" );
                streamLine( os, i+1, R"(<accidental-text enclosure="oval">flat-flat</accidental-text>)" );
                streamLine( os, i, R"(</part-name-display>)", false );
            }
                break;
            default:
                break;
        }
    }
    DisplayTextOrAccidentalTextPtr tgenDisplayTextOrAccidentalText( TestMode v )
    {
        DisplayTextOrAccidentalTextPtr o = makeDisplayTextOrAccidentalText();
        switch ( v )
        {
            case TestMode::one:
            {
                o->setChoice( DisplayTextOrAccidentalText::Choice::accidentalText );
                o->getAccidentalText()->setValue( AccidentalValue::flatFlat );
                o->getAccidentalText()->getAttributes()->hasEnclosure = true;
                o->getAccidentalText()->getAttributes()->enclosure = EnclosureShape::oval;
                
            }
                break;
            case TestMode::two:
            {
                o->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
                o->getDisplayText()->setValue( XsString( "two" ) );
                o->getDisplayText()->getAttributes()->hasJustify = true;
                o->getDisplayText()->getAttributes()->justify = LeftCenterRight::center;
            }
                break;
            case TestMode::three:
            {
                o->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
                o->getDisplayText()->setValue( XsString( "three" ) );
                o->getDisplayText()->getAttributes()->hasEnclosure = false;
                o->getDisplayText()->getAttributes()->hasHalign = true;
                o->getDisplayText()->getAttributes()->halign = LeftCenterRight::right;
                o->getDisplayText()->getAttributes()->hasFontWeight = true;
                o->getDisplayText()->getAttributes()->fontWeight = FontWeight::bold;
            }
                break;
            default:
                break;
        }
        return o;
    }
}

#endif
