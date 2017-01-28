// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScoreTimewiseTest.h"
#include "mxtest/core/TimewiseMeasureTest.h"
#include "mxtest/core/ScoreHeaderGroupTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, ScoreTimewise )
{
    variant v = variant::one;
	ScoreTimewisePtr object = tgenScoreTimewise( v );
	stringstream expected;
	tgenScoreTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ScoreTimewise )
{
    variant v = variant::two;
	ScoreTimewisePtr object = tgenScoreTimewise( v );
	stringstream expected;
	tgenScoreTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ScoreTimewise )
{
    variant v = variant::three;
	ScoreTimewisePtr object = tgenScoreTimewise( v );
	stringstream expected;
	tgenScoreTimewiseExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ScoreTimewisePtr tgenScoreTimewise( variant v )
    {
        ScoreTimewisePtr o = makeScoreTimewise();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasVersion = true;
                o->getAttributes()->version = XsToken( "3.0" );
                o->setScoreHeaderGroup( tgenScoreHeaderGroup( v ) );
                o->addTimewiseMeasure( tgenTimewiseMeasure( v ) );
                o->removeTimewiseMeasure( o->getTimewiseMeasureSet().cbegin() );
            }
                break;
            case variant::three:
            {
                o->setScoreHeaderGroup( tgenScoreHeaderGroup( v ) );
                o->addTimewiseMeasure( tgenTimewiseMeasure( v ) );
                o->removeTimewiseMeasure( o->getTimewiseMeasureSet().cbegin() );
                o->addTimewiseMeasure( tgenTimewiseMeasure( variant::one ) );
                o->addTimewiseMeasure( tgenTimewiseMeasure( variant::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScoreTimewiseExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<score-timewise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewiseMeasureExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-timewise>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<score-timewise version="3.0">)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewiseMeasureExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-timewise>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<score-timewise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenTimewiseMeasureExpected( os, i+1, v );
                os << std::endl;
                tgenTimewiseMeasureExpected( os, i+1, variant::one );
                os << std::endl;
                tgenTimewiseMeasureExpected( os, i+1, variant::two );
                os << std::endl;
                streamLine( os, i, R"(</score-timewise>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
