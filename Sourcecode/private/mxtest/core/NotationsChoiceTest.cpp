// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/NotationsChoiceTest.h"
#include "mxtest/core/TupletTest.h"
#include "mxtest/core/OrnamentsTest.h"
#include "mxtest/core/TechnicalTest.h"
#include "mxtest/core/ArticulationsTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, NotationsChoice )
{
    TestMode v = TestMode::one;
	NotationsChoicePtr object = tgenNotationsChoice( v );
	stringstream expected;
	tgenNotationsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, NotationsChoice )
{
    TestMode v = TestMode::two;
	NotationsChoicePtr object = tgenNotationsChoice( v );
	stringstream expected;
	tgenNotationsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, NotationsChoice )
{
    TestMode v = TestMode::three;
	NotationsChoicePtr object = tgenNotationsChoice( v );
	stringstream expected;
	tgenNotationsChoiceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    NotationsChoicePtr tgenNotationsChoice( TestMode v )
    {
        NotationsChoicePtr o;
        switch ( v )
        {
            case TestMode::one:
            {
                o = makeNotationsChoice();
            }
                break;
            case TestMode::two:
            {
                o = tgenNotationsChoice();
                o->setChoice( NotationsChoice::Choice::dynamics );
            }
                break;
            case TestMode::three:
            {
                o = tgenNotationsChoice();
                o->setChoice( NotationsChoice::Choice::technical );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenNotationsChoiceExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<tied type="start"/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<dynamics>)" );
                streamLine( os, i+1, R"(<fz/>)" );
                streamLine( os, i, R"(</dynamics>)", false );
            }
                break;
            case TestMode::three:
            {
                tgenTechnicalExpected(os, i, TestMode::two );
            }
                break;
            default:
                break;
        }
    }
    NotationsChoicePtr tgenNotationsChoice()
    {
        NotationsChoicePtr o = makeNotationsChoice();
        o->getTied()->getAttributes()->hasBezierOffset = true;
        o->getTied()->getAttributes()->bezierOffset = DivisionsValue( 2.2 );
        o->getSlur()->getAttributes()->hasBezierOffset2 = true;
        o->getSlur()->getAttributes()->bezierOffset2 = DivisionsValue( 3.1 );
        o->setTuplet( tgenTuplet( TestMode::two ) );
        o->getGlissando()->setValue( XsString( "Weeee" ) );
        o->getSlide()->setValue( XsString( "Geronamo?" ) );
        o->setOrnaments( tgenOrnaments( TestMode::three ) );
        o->setTechnical( tgenTechnical( TestMode::two ) );
        o->setArticulations( tgenArticulations( TestMode::three ) );
        o->getDynamics()->setValue( DynamicsValue( DynamicsEnum::fz ) );
        o->getFermata()->setValue( FermataShape::square );
        o->getArpeggiate()->getAttributes()->hasNumber = true;
        o->getArpeggiate()->getAttributes()->number = NumberLevel( 3 );
        o->getNonArpeggiate()->getAttributes()->type = TopBottom::bottom;
        o->getAccidentalMark()->setValue( AccidentalValue::flatFlat );
        o->getOtherNotation()->setValue( XsString( "yo mamma" ) );
        return o;
    }
}

#endif
