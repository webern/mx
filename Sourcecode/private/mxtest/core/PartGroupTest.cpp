// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PartGroupTest.h"
#include "mxtest/core/GroupNameDisplayTest.h"
#include "mxtest/core/GroupAbbreviationDisplayTest.h"
#include "mxtest/core/EditorialGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PartGroup )
{
    TestMode v = TestMode::one;
	PartGroupPtr object = tgenPartGroup( v );
	stringstream expected;
	tgenPartGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, PartGroup )
{
    TestMode v = TestMode::two;
	PartGroupPtr object = tgenPartGroup( v );
	stringstream expected;
	tgenPartGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PartGroup )
{
    TestMode v = TestMode::three;
	PartGroupPtr object = tgenPartGroup( v );
	stringstream expected;
	tgenPartGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PartGroupPtr tgenPartGroup( TestMode v )
    {
        PartGroupPtr o = makePartGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->type = StartStop::stop;
                o->getAttributes()->hasNumber = true;
                o->setHasGroupName( true );
                o->getGroupName()->setValue( XsString( "Some Group of Instruments" ) );
                o->setHasGroupNameDisplay( true );
                o->setGroupNameDisplay( tgenGroupNameDisplay( v ) );
                o->setHasGroupAbbreviation( true );
                o->getGroupAbbreviation()->setValue( XsString( "SGI" ) );
                o->setHasGroupAbbreviationDisplay( true );
                o->setGroupAbbreviationDisplay( tgenGroupAbbreviationDisplay( v ) );
                o->setHasGroupSymbol( true );
                o->getGroupSymbol()->setValue( GroupSymbolValue::bracket );
                o->setHasGroupBarline( true );
                o->getGroupBarline()->setValue( GroupBarlineValue::mensurstrich );
                o->setHasGroupTime( true );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->type = StartStop::start;
                o->getAttributes()->hasNumber = true;
                o->getAttributes()->number = XsToken( "2" );
                o->setHasGroupName( true );
                o->getGroupName()->setValue( XsString( "Piano" ) );
                o->setHasGroupNameDisplay( true );
                o->setGroupNameDisplay( tgenGroupNameDisplay( v ) );
                o->setHasGroupAbbreviation( true );
                o->getGroupAbbreviation()->setValue( XsString( "pno." ) );
                o->setHasGroupAbbreviationDisplay( true );
                o->setGroupAbbreviationDisplay( tgenGroupAbbreviationDisplay( v ) );
                o->setHasGroupSymbol( true );
                o->getGroupSymbol()->setValue( GroupSymbolValue::square );
                o->setHasGroupBarline( true );
                o->getGroupBarline()->setValue( GroupBarlineValue::yes );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<part-group type="start"/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<part-group type="stop" number="1">)" );
                streamLine( os, i+1, R"(<group-name>Some Group of Instruments</group-name>)" );
                tgenGroupNameDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group-abbreviation>SGI</group-abbreviation>)" );
                tgenGroupAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group-symbol>bracket</group-symbol>)" );
                streamLine( os, i+1, R"(<group-barline>Mensurstrich</group-barline>)" );
                streamLine( os, i+1, R"(<group-time/>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</part-group>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<part-group type="start" number="2">)" );
                streamLine( os, i+1, R"(<group-name>Piano</group-name>)" );
                tgenGroupNameDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group-abbreviation>pno.</group-abbreviation>)" );
                tgenGroupAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group-symbol>square</group-symbol>)" );
                streamLine( os, i+1, R"(<group-barline>yes</group-barline>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</part-group>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
