// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScorePartwiseTest.h"
#include "mxtest/core/PartwisePartTest.h"
#include "mxtest/core/ScoreHeaderGroupTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, ScorePartwise )
{
    TestMode v = TestMode::one;
	ScorePartwisePtr object = tgenScorePartwise( v );
	stringstream expected;
	tgenScorePartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ScorePartwise )
{
    TestMode v = TestMode::two;
	ScorePartwisePtr object = tgenScorePartwise( v );
	stringstream expected;
	tgenScorePartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ScorePartwise )
{
    TestMode v = TestMode::three;
	ScorePartwisePtr object = tgenScorePartwise( v );
	stringstream expected;
	tgenScorePartwiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ScorePartwisePtr tgenScorePartwise( TestMode v )
    {
        ScorePartwisePtr o = makeScorePartwise();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasVersion = true;
                o->getAttributes()->version = XsToken( "3.0" );
                o->setScoreHeaderGroup( tgenScoreHeaderGroup( v ) );
                o->addPartwisePart( tgenPartwisePart( v ) );
                o->removePartwisePart( o->getPartwisePartSet().cbegin() );
            }
                break;
            case TestMode::three:
            {
                o->setScoreHeaderGroup( tgenScoreHeaderGroup( v ) );
                o->addPartwisePart( tgenPartwisePart( v ) );
                o->removePartwisePart( o->getPartwisePartSet().cbegin() );
                o->addPartwisePart( tgenPartwisePart( TestMode::one ) );
                o->addPartwisePart( tgenPartwisePart( TestMode::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScorePartwiseExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<score-partwise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-partwise>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<score-partwise version="3.0">)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-partwise>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<score-partwise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartwisePartExpected(os, i+1, TestMode::one );
                os << std::endl;
                tgenPartwisePartExpected(os, i+1, TestMode::two );
                os << std::endl;
                streamLine( os, i, R"(</score-partwise>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

