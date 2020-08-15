// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
using namespace mxtest;

TEST( Test01, ScoreHeaderGroup )
{
    TestMode v = TestMode::one;
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
    TestMode v = TestMode::two;
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
    TestMode v = TestMode::three;
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

namespace mxtest
{
    ScoreHeaderGroupPtr tgenScoreHeaderGroup( TestMode v )
    {
        ScoreHeaderGroupPtr o = makeScoreHeaderGroup();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
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
            case TestMode::three:
            {
                o->addCredit( tgenCredit( TestMode::two ) );
                o->addCredit( tgenCredit( TestMode::three ) );
                o->setPartList( tgenPartList( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScoreHeaderGroupExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                tgenPartListExpected( os, i, v );
            }
                break;
            case TestMode::two:
            {
                tgenWorkExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<movement-number>I</movement-number>)" );
                streamLine( os, i, R"(<movement-title>Allegro</movement-title>)" );
                tgenPartListExpected( os, i, v );
            }
                break;
            case TestMode::three:
            {
                tgenCreditExpected(os, i, TestMode::two );
                os << std::endl;
                tgenCreditExpected(os, i, TestMode::three );
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
