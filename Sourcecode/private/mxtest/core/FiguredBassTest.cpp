// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/FiguredBassTest.h"
#include "mxtest/core/EditorialGroupTest.h"
#include "mxtest/core/FigureTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, FiguredBass )
{
    TestMode v = TestMode::one;
	FiguredBassPtr object = tgenFiguredBass( v );
	stringstream expected;
	tgenFiguredBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, FiguredBass )
{
    TestMode v = TestMode::two;
	FiguredBassPtr object = tgenFiguredBass( v );
	stringstream expected;
	tgenFiguredBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, FiguredBass )
{
    TestMode v = TestMode::three;
	FiguredBassPtr object = tgenFiguredBass( v );
	stringstream expected;
	tgenFiguredBassExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    FiguredBassPtr tgenFiguredBass( TestMode v )
    {
        FiguredBassPtr o = makeFiguredBass();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasDuration( true );
                o->getDuration()->setValue( PositiveDivisionsValue( 29 ) );
                o->addFigure( tgenFigure( TestMode::three ) );
                o->addFigure( tgenFigure( v ) );
                o->removeFigure( o->getFigureSet().cbegin() );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                o->getAttributes()->hasParentheses = true;
                o->getAttributes()->parentheses = YesNo::yes;
            }
                break;
            case TestMode::three:
            {
                o->addFigure( tgenFigure( TestMode::one ) );
                o->addFigure( tgenFigure( v ) );
                o->removeFigure( o->getFigureSet().cbegin() );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                o->getAttributes()->hasParentheses = true;
                o->getAttributes()->parentheses = YesNo::yes;
                o->getAttributes()->hasPrintDot = true;
                o->getAttributes()->printDot = YesNo::no;
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenFiguredBassExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<figured-bass>)" );
                streamLine( os, i+1, R"(<figure/>)" );
                streamLine( os, i, R"(</figured-bass>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<figured-bass parentheses="yes">)" );
                tgenFigureExpected(os, i+1, TestMode::three );
                os << std::endl;
                tgenFigureExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<duration>29</duration>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</figured-bass>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<figured-bass print-dot="no" parentheses="yes">)" );
                tgenFigureExpected(os, i+1, TestMode::one );
                os << std::endl;
                tgenFigureExpected( os, i+1, v );
                os << std::endl;
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</figured-bass>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
