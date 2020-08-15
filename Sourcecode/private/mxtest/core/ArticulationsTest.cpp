// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/ArticulationsTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Articulations )
{
    TestMode v = TestMode::one;
	ArticulationsPtr object = tgenArticulations( v );
	stringstream expected;
	tgenArticulationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Articulations )
{
    TestMode v = TestMode::two;
	ArticulationsPtr object = tgenArticulations( v );
	stringstream expected;
	tgenArticulationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Articulations )
{
    TestMode v = TestMode::three;
	ArticulationsPtr object = tgenArticulations( v );
	stringstream expected;
	tgenArticulationsExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ArticulationsChoicePtr tgenArticulationsChoice( TestMode v )
    {
        ArticulationsChoicePtr o = makeArticulationsChoice();
        switch ( v )
        {
            case TestMode::one:
            {
                o->setChoice( ArticulationsChoice::Choice::breathMark );
                o->getBreathMark()->getAttributes()->hasFontStyle = true;
                o->getBreathMark()->getAttributes()->fontStyle = FontStyle::italic;
            }
                break;
            case TestMode::two:
            {
                o->setChoice( ArticulationsChoice::Choice::strongAccent );
                o->getStrongAccent()->getAttributes()->hasColor = true;
                o->getStrongAccent()->getAttributes()->color = Color( 56, 90, 12 );
            }
                break;
            case TestMode::three:
            {
                o->setChoice( ArticulationsChoice::Choice::tenuto );
                o->getTenuto()->getAttributes()->hasPlacement = true;
                o->getTenuto()->getAttributes()->placement = AboveBelow::above;
            }
                break;
            default:
                break;
        }
        return o;
    }
    ArticulationsPtr tgenArticulations( TestMode v )
    {
        ArticulationsPtr o = makeArticulations();
        switch ( v )
        {
            case TestMode::one:
            {
                ;
            }
                break;
            case TestMode::two:
            {
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::one ) );
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::three ) );
            }
                break;
            case TestMode::three:
            {
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::two ) );
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::three ) );
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::one ) );
                o->addArticulationsChoice( tgenArticulationsChoice( TestMode::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenArticulationsExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<articulations/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<articulations>)" );
                streamLine( os, i+1, R"(<breath-mark font-style="italic"></breath-mark>)" );
                streamLine( os, i+1, R"(<tenuto placement="above"/>)" );
                streamLine( os, i, R"(</articulations>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<articulations>)" );
                streamLine( os, i+1, R"(<strong-accent color="#385A0C"/>)" );
                streamLine( os, i+1, R"(<tenuto placement="above"/>)" );
                streamLine( os, i+1, R"(<breath-mark font-style="italic"></breath-mark>)" );
                streamLine( os, i+1, R"(<strong-accent color="#385A0C"/>)" );
                streamLine( os, i, R"(</articulations>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
