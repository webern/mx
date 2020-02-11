// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/PartNameDisplayTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PartNameDisplay )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    PartNameDisplayPtr tgenPartNameDisplay( variant v )
    {
        PartNameDisplayPtr o = makePartNameDisplay();
        switch ( v )
        {
            case variant::one:
            {
                ;
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::no;
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::one ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::two ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::yes;
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::three ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::three ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::two ) );
                o->addDisplayTextOrAccidentalText( tgenDisplayTextOrAccidentalText( variant::one ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartNameDisplayExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<part-name-display></part-name-display>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<part-name-display print-object="no">)" );
                streamLine( os, i+1, R"(<accidental-text enclosure="oval">flat-flat</accidental-text>)" );
                streamLine( os, i+1, R"(<display-text justify="center">two</display-text>)" );
                streamLine( os, i, R"(</part-name-display>)", false );
            }
                break;
            case variant::three:
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
    DisplayTextOrAccidentalTextPtr tgenDisplayTextOrAccidentalText( variant v )
    {
        DisplayTextOrAccidentalTextPtr o = makeDisplayTextOrAccidentalText();
        switch ( v )
        {
            case variant::one:
            {
                o->setChoice( DisplayTextOrAccidentalText::Choice::accidentalText );
                o->getAccidentalText()->setValue( AccidentalValue::flatFlat );
                o->getAccidentalText()->getAttributes()->hasEnclosure = true;
                o->getAccidentalText()->getAttributes()->enclosure = EnclosureShape::oval;
                
            }
                break;
            case variant::two:
            {
                o->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
                o->getDisplayText()->setValue( XsString( "two" ) );
                o->getDisplayText()->getAttributes()->hasJustify = true;
                o->getDisplayText()->getAttributes()->justify = LeftCenterRight::center;
            }
                break;
            case variant::three:
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
