// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PartListTest.h"
#include "mxtest/core/PartGroupOrScorePartTest.h"
#include "mxtest/core/PartGroupTest.h"
#include "mxtest/core/ScorePartTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, PartList )
{
    TestMode v = TestMode::one;
	PartListPtr object = tgenPartList( v );
	stringstream expected;
	tgenPartListExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, PartList )
{
    TestMode v = TestMode::two;
	PartListPtr object = tgenPartList( v );
	stringstream expected;
	tgenPartListExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, PartList )
{
    TestMode v = TestMode::three;
	PartListPtr object = tgenPartList( v );
	stringstream expected;
	tgenPartListExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PartListPtr tgenPartList( TestMode v )
    {
        PartListPtr o = makePartList();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->addPartGroup( tgenPartGroup( v ) );
                o->addPartGroup( tgenPartGroup( TestMode::three ) );
                o->setScorePart( tgenScorePart( v ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( v ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( TestMode::one ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( TestMode::three ) );
            }
                break;
            case TestMode::three:
            {
                o->setScorePart( tgenScorePart( v ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPartListExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<part-list>)" );
                tgenScorePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</part-list>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<part-list>)" );
                tgenPartGroupExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupExpected(os, i+1, TestMode::three );
                os << std::endl;
                tgenScorePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupOrScorePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupOrScorePartExpected(os, i+1, TestMode::one );
                os << std::endl;
                tgenPartGroupOrScorePartExpected(os, i+1, TestMode::three );
                os << std::endl;
                streamLine( os, i, R"(</part-list>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<part-list>)" );
                tgenScorePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupOrScorePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</part-list>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
