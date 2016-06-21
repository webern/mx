#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/GroupAbbreviationDisplayTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, GroupAbbreviationDisplay )
{
    variant v = variant::one;
	GroupAbbreviationDisplayPtr object = tgenGroupAbbreviationDisplay( v );
	stringstream expected;
	tgenGroupAbbreviationDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, GroupAbbreviationDisplay )
{
    variant v = variant::two;
	GroupAbbreviationDisplayPtr object = tgenGroupAbbreviationDisplay( v );
	stringstream expected;
	tgenGroupAbbreviationDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, GroupAbbreviationDisplay )
{
    variant v = variant::three;
	GroupAbbreviationDisplayPtr object = tgenGroupAbbreviationDisplay( v );
	stringstream expected;
	tgenGroupAbbreviationDisplayExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    GroupAbbreviationDisplayPtr tgenGroupAbbreviationDisplay( variant v )
    {
        GroupAbbreviationDisplayPtr o = makeGroupAbbreviationDisplay();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setChoice( GroupAbbreviationDisplay::Choice::accidentalText );
                o->getAccidentalText()->setValue( AccidentalValue::doubleSharp );
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::no;
            }
                break;
            case variant::three:
            {
                o->setChoice( GroupAbbreviationDisplay::Choice::displayText );
                o->getDisplayText()->setValue( XsString( "My Display String!" ) );
                o->getDisplayText()->getAttributes()->hasSpace = true;
                o->getDisplayText()->getAttributes()->space = XmlSpace::preserve;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenGroupAbbreviationDisplayExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<group-abbreviation-display>)" );
                streamLine( os, i+1, R"(<display-text></display-text>)" );
                streamLine( os, i, R"(</group-abbreviation-display>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<group-abbreviation-display print-object="no">)" );
                streamLine( os, i+1, R"(<accidental-text>double-sharp</accidental-text>)" );
                streamLine( os, i, R"(</group-abbreviation-display>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<group-abbreviation-display>)" );
                streamLine( os, i+1, R"(<display-text xml:space="preserve">My Display String!</display-text>)" );
                streamLine( os, i, R"(</group-abbreviation-display>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
