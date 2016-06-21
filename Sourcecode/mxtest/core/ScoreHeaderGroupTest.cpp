#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScoreHeaderGroupTest.h"
#include "mxtest/core/WorkTest.h"
#include "mxtest/core/IdentificationTest.h"
#include "mxtest/core/DefaultsTest.h"
#include "mxtest/core/CreditTest.h"
#include "mxtest/core/PartListTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, ScoreHeaderGroup )
{
    variant v = variant::one;
	ScoreHeaderGroupPtr object = tgenScoreHeaderGroup( v );
	stringstream expected;
	tgenScoreHeaderGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ScoreHeaderGroup )
{
    variant v = variant::two;
	ScoreHeaderGroupPtr object = tgenScoreHeaderGroup( v );
	stringstream expected;
	tgenScoreHeaderGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ScoreHeaderGroup )
{
    variant v = variant::three;
	ScoreHeaderGroupPtr object = tgenScoreHeaderGroup( v );
	stringstream expected;
	tgenScoreHeaderGroupExpected( expected, 1, v );
	stringstream actual;
	bool isOneLineOnly;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    ScoreHeaderGroupPtr tgenScoreHeaderGroup( variant v )
    {
        ScoreHeaderGroupPtr o = makeScoreHeaderGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasWork( true );
                o->setWork( tgenWork( v ) );
                o->setHasMovementNumber( true );
                o->getMovementNumber()->setValue( XsString( "I" ) );
                o->setHasMovementTitle( true );
                o->getMovementTitle()->setValue( XsString( "Allegro" ) );
                o->setPartList( tgenPartList( v ) );
            }
                break;
            case variant::three:
            {
                o->addCredit( tgenCredit( variant::two ) );
                o->addCredit( tgenCredit( variant::three ) );
                o->setPartList( tgenPartList( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScoreHeaderGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenPartListExpected( os, i, v );
            }
                break;
            case variant::two:
            {
                tgenWorkExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<movement-number>I</movement-number>)" );
                streamLine( os, i, R"(<movement-title>Allegro</movement-title>)" );
                tgenPartListExpected( os, i, v );
            }
                break;
            case variant::three:
            {
                tgenCreditExpected( os, i, variant::two );
                os << std::endl;
                tgenCreditExpected( os, i, variant::three );
                os << std::endl;
                tgenPartListExpected( os, i, v );
            }
                break;
            default:
                break;
        }
    }
}

#endif
