// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/FigureTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Figure )
{
    TestMode v = TestMode::one;
	FigurePtr object = tgenFigure( v );
	stringstream expected;
	tgenFigureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Figure )
{
    TestMode v = TestMode::two;
	FigurePtr object = tgenFigure( v );
	stringstream expected;
	tgenFigureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Figure )
{
    TestMode v = TestMode::three;
	FigurePtr object = tgenFigure( v );
	stringstream expected;
	tgenFigureExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    FigurePtr tgenFigure( TestMode v )
    {
        FigurePtr o = makeFigure();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->setHasExtend( true );
                o->setHasPrefix( true );
                o->getExtend()->getAttributes()->hasType = true;
                o->getPrefix()->setValue( XsString( "-" ) );
            }
                break;
            case TestMode::three:
            {
                o->setHasExtend( true );
                o->setHasPrefix( true );
                o->setHasSuffix( true );
                o->setHasFigureNumber( true );
                o->getFigureNumber()->setValue( XsString( "xx" ) );
                o->getExtend()->getAttributes()->hasType = true;
                o->getPrefix()->setValue( XsString( "-" ) );
                o->getSuffix()->setValue( XsString( "post" ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenFigureExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<figure/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<figure>)" );
                streamLine( os, i+1, R"(<prefix>-</prefix>)" );
                streamLine( os, i+1, R"(<extend type="start"/>)" );
                streamLine( os, i, R"(</figure>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<figure>)" );
                streamLine( os, i+1, R"(<prefix>-</prefix>)" );
                streamLine( os, i+1, R"(<figure-number>xx</figure-number>)" );
                streamLine( os, i+1, R"(<suffix>post</suffix>)" );
                streamLine( os, i+1, R"(<extend type="start"/>)" );
                streamLine( os, i, R"(</figure>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
