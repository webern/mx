// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/TechnicalTest.h"
#include "mxtest/core/TechnicalChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Technical )
{
    TestMode v = TestMode::one;
	TechnicalPtr object = tgenTechnical( v );
	stringstream expected;
	tgenTechnicalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Technical )
{
    TestMode v = TestMode::two;
	TechnicalPtr object = tgenTechnical( v );
	stringstream expected;
	tgenTechnicalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Technical )
{
    TestMode v = TestMode::three;
	TechnicalPtr object = tgenTechnical( v );
	stringstream expected;
	tgenTechnicalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TechnicalPtr tgenTechnical( TestMode v )
    {
        TechnicalPtr o = makeTechnical();
        switch ( v )
        {
            case TestMode::one:
            {

            }
                break;
            case TestMode::two:
            {
                o->addTechnicalChoice( tgenTechnicalChoice( TestMode::one ) );
                o->addTechnicalChoice( tgenTechnicalChoice( TestMode::two ) );
                auto x = makeTechnicalChoice();
                x->setChoice( TechnicalChoice::Choice::handbell );
                x->getHandbell()->setValue( HandbellValue::gyro );
                o->addTechnicalChoice( x );
            }
                break;
            case TestMode::three:
            {
                o->addTechnicalChoice( tgenTechnicalChoice( v ) );
                auto x = makeTechnicalChoice();
                x->setChoice( TechnicalChoice::Choice::otherTechnical );
                x->getOtherTechnical()->setValue( XsString( "Jump up and down" ) );
                x->getOtherTechnical()->getAttributes()->hasFontStyle = true;
                x->getOtherTechnical()->getAttributes()->fontStyle = FontStyle::italic;
                o->addTechnicalChoice( x );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTechnicalExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<technical/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<technical>)" );
                tgenTechnicalChoiceExpected(os, i+1, TestMode::one );
                os << std::endl;
                tgenTechnicalChoiceExpected(os, i+1, TestMode::two );
                os << std::endl;
                streamLine( os, i+1, R"(<handbell>gyro</handbell>)" );
                streamLine( os, i, R"(</technical>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<technical>)" );
                tgenTechnicalChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<other-technical font-style="italic">Jump up and down</other-technical>)" );
                streamLine( os, i, R"(</technical>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

