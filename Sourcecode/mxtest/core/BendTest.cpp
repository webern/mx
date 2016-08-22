// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/BendTest.h"
#include "mxtest/core/BendChoiceTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Bend )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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

namespace MxTestHelpers
{
    BendPtr tgenBend( variant v )
    {
        BendPtr o = makeBend();
        switch ( v )
        {
            case variant::one:
            {
                o->setBendChoice( tgenBendChoice( v ) );
            }
                break;
            case variant::two:
            {
                o->setBendChoice( tgenBendChoice( v ) );
                o->getBendAlter()->setValue( Semitones( 2 ) );
                o->getAttributes()->hasAccelerate = true;
                o->getAttributes()->hasBeats = true;
                o->getAttributes()->beats = TrillBeats( 3 );
                o->setHasBendChoice( true );
            }
                break;
            case variant::three:
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
    void tgenBendExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<bend>)" );
                streamLine( os, i+1, R"(<bend-alter>0</bend-alter>)" );
                //tgenBendChoiceExpected( os, i+1, v );
                //os << std::endl;
                streamLine( os, i, R"(</bend>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<bend accelerate="no" beats="3">)" );
                streamLine( os, i+1, R"(<bend-alter>2</bend-alter>)" );
                tgenBendChoiceExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</bend>)", false );
            }
                break;
            case variant::three:
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
