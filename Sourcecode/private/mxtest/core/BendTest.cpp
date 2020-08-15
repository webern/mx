// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/BendTest.h"
#include "mxtest/core/BendChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Bend )
{
    TestMode v = TestMode::one;
	BendPtr object = tgenBend( v );
	stringstream expected;
	tgenBendExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Bend )
{
    TestMode v = TestMode::two;
	BendPtr object = tgenBend( v );
	stringstream expected;
	tgenBendExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Bend )
{
    TestMode v = TestMode::three;
	BendPtr object = tgenBend( v );
	stringstream expected;
	tgenBendExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    BendPtr tgenBend( TestMode v )
    {
        BendPtr o = makeBend();
        switch ( v )
        {
            case TestMode::one:
            {
                o->setBendChoice( tgenBendChoice( v ) );
            }
                break;
            case TestMode::two:
            {
                o->setBendChoice( tgenBendChoice( v ) );
                o->getBendAlter()->setValue( Semitones( 2 ) );
                o->getAttributes()->hasAccelerate = true;
                o->getAttributes()->hasBeats = true;
                o->getAttributes()->beats = TrillBeats( 3 );
                o->setHasBendChoice( true );
            }
                break;
            case TestMode::three:
            {
                o->setBendChoice( tgenBendChoice( v ) );
                o->getBendAlter()->setValue( Semitones( -1.1 ) );
                o->getAttributes()->hasColor = true;
                o->getAttributes()->color = Color( 43, 90, 123 );
                o->getAttributes()->hasLastBeat = true;
                o->getAttributes()->lastBeat = Percent( 0.999 );
                o->setHasWithBar( true );
                o->getWithBar()->setValue( XsString( "FooBar" ) );
                o->setHasBendChoice( true );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenBendExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<bend>)" );
                streamLine( os, i+1, R"(<bend-alter>0</bend-alter>)" );
                //tgenBendChoiceExpected( os, i+1, v );
                //os << std::endl;
                streamLine( os, i, R"(</bend>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<bend accelerate="no" beats="3">)" );
                streamLine( os, i+1, R"(<bend-alter>2</bend-alter>)" );
                tgenBendChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</bend>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<bend color="#2B5A7B" last-beat="0.999">)" );
                streamLine( os, i+1, R"(<bend-alter>-1.1</bend-alter>)" );
                tgenBendChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<with-bar>FooBar</with-bar>)" );
                streamLine( os, i, R"(</bend>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
