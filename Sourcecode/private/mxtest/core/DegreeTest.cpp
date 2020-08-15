// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/DegreeTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Degree )
{
    TestMode v = TestMode::one;
	DegreePtr object = tgenDegree( v );
	stringstream expected;
	tgenDegreeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Degree )
{
    TestMode v = TestMode::two;
	DegreePtr object = tgenDegree( v );
	stringstream expected;
	tgenDegreeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Degree )
{
    TestMode v = TestMode::three;
	DegreePtr object = tgenDegree( v );
	stringstream expected;
	tgenDegreeExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    DegreePtr tgenDegree( TestMode v )
    {
        DegreePtr o = makeDegree();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getDegreeAlter()->setValue( Semitones( 1.01 ) );
                o->getDegreeType()->setValue( DegreeTypeValue::subtract );
                o->getDegreeValue()->setValue( PositiveInteger( 9 ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasPrintObject = true;
                o->getAttributes()->printObject = YesNo::no;
                o->getDegreeAlter()->setValue( Semitones( -2.01 ) );
                o->getDegreeType()->setValue( DegreeTypeValue::alter );
                o->getDegreeValue()->setValue( PositiveInteger( 7 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenDegreeExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<degree>)" );
                streamLine( os, i+1, R"(<degree-value>1</degree-value>)" );
                streamLine( os, i+1, R"(<degree-alter>0</degree-alter>)" );
                streamLine( os, i+1, R"(<degree-type>add</degree-type>)" );
                streamLine( os, i, R"(</degree>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<degree>)" );
                streamLine( os, i+1, R"(<degree-value>9</degree-value>)" );
                streamLine( os, i+1, R"(<degree-alter>1.01</degree-alter>)" );
                streamLine( os, i+1, R"(<degree-type>subtract</degree-type>)" );
                streamLine( os, i, R"(</degree>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<degree print-object="no">)" );
                streamLine( os, i+1, R"(<degree-value>7</degree-value>)" );
                streamLine( os, i+1, R"(<degree-alter>-2.01</degree-alter>)" );
                streamLine( os, i+1, R"(<degree-type>alter</degree-type>)" );
                streamLine( os, i, R"(</degree>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
