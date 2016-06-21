#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScoreInstrumentTest.h"
#include "mxtest/core/VirtualInstrumentTest.h"
#include "mxtest/core/EncodingTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, ScoreInstrument )
{
    variant v = variant::one;
	ScoreInstrumentPtr object = tgenScoreInstrument( v );
	stringstream expected;
	tgenScoreInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ScoreInstrument )
{
    variant v = variant::two;
	ScoreInstrumentPtr object = tgenScoreInstrument( v );
	stringstream expected;
	tgenScoreInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ScoreInstrument )
{
    variant v = variant::three;
	ScoreInstrumentPtr object = tgenScoreInstrument( v );
	stringstream expected;
	tgenScoreInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    ScoreInstrumentPtr tgenScoreInstrument( variant v )
    {
        ScoreInstrumentPtr o = makeScoreInstrument();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getInstrumentName()->setValue( XsString( "Bassoon" ) );
                o->setHasInstrumentAbbreviation( true );
                o->getInstrumentAbbreviation()->setValue( XsString( "Bsn." ) );
                o->setHasSoloOrEnsemble( true );
                o->setHasVirtualInstrument( true );
                o->setVirtualInstrument( tgenVirtualInstrument( v ) );
                o->getAttributes()->id = XsID{ "Instrument01" };
                
            }
                break;
            case variant::three:
            {
                o->getInstrumentName()->setValue( XsString( "Trumpet" ) );
                o->setHasInstrumentAbbreviation( true );
                o->getInstrumentAbbreviation()->setValue( XsString( "Trp." ) );
                o->setHasSoloOrEnsemble( true );
                o->getSoloOrEnsemble()->setChoice( SoloOrEnsemble::Choice::ensemble );
                PositiveIntegerOrEmpty forteen{ PositiveInteger{ 14 } };
                o->getSoloOrEnsemble()->getEnsemble()->setValue( forteen );
                o->setHasVirtualInstrument( true );
                o->setVirtualInstrument( tgenVirtualInstrument( v ) );
                o->getAttributes()->id = XsID{ "Instrument02" };
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScoreInstrumentExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<score-instrument id="ID">)" );
                streamLine( os, i+1, R"(<instrument-name></instrument-name>)" );
                streamLine( os, i, R"(</score-instrument>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<score-instrument id="Instrument01">)" );
                streamLine( os, i+1, R"(<instrument-name>Bassoon</instrument-name>)" );
                streamLine( os, i+1, R"(<instrument-abbreviation>Bsn.</instrument-abbreviation>)" );
                streamLine( os, i+1, R"(<solo/>)" );
                tgenVirtualInstrumentExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-instrument>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<score-instrument id="Instrument02">)" );
                streamLine( os, i+1, R"(<instrument-name>Trumpet</instrument-name>)" );
                streamLine( os, i+1, R"(<instrument-abbreviation>Trp.</instrument-abbreviation>)" );
                streamLine( os, i+1, R"(<ensemble>14</ensemble>)" );
                tgenVirtualInstrumentExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-instrument>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
