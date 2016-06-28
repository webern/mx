#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScorePartwiseTest.h"
#include "mxtest/core/PartwisePartTest.h"
#include "mxtest/core/ScoreHeaderGroupTest.h"


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, ScorePartwise )
{
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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

namespace MxTestHelpers
{
    ScorePartwisePtr tgenScorePartwise( variant v )
    {
        ScorePartwisePtr o = makeScorePartwise();
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
                o->addPartwisePart( tgenPartwisePart( v ) );
                o->removePartwisePart( o->getPartwisePartSet().cbegin() );
            }
                break;
            case variant::three:
            {
                o->setScoreHeaderGroup( tgenScoreHeaderGroup( v ) );
                o->addPartwisePart( tgenPartwisePart( v ) );
                o->removePartwisePart( o->getPartwisePartSet().cbegin() );
                o->addPartwisePart( tgenPartwisePart( variant::one ) );
                o->addPartwisePart( tgenPartwisePart( variant::two ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScorePartwiseExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<score-partwise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-partwise>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<score-partwise version="3.0">)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-partwise>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<score-partwise>)" );
                tgenScoreHeaderGroupExpected( os, i+1,  v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, variant::one );
                os << std::endl;
                tgenPartwisePartExpected( os, i+1, variant::two );
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

