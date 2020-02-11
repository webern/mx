// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
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
    variant v = variant::one;
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
    variant v = variant::two;
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
    variant v = variant::three;
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
    PartListPtr tgenPartList( variant v )
    {
        PartListPtr o = makePartList();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->addPartGroup( tgenPartGroup( v ) );
                o->addPartGroup( tgenPartGroup( variant::three ) );
                o->setScorePart( tgenScorePart( v ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( v ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( variant::one ) );
                o->addPartGroupOrScorePart( tgenPartGroupOrScorePart( variant::three ) );
            }
                break;
            case variant::three:
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
    void tgenPartListExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<part-list>)" );
                tgenScorePartExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</part-list>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<part-list>)" );
                tgenPartGroupExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupExpected( os, i+1, variant::three );
                os << std::endl;
                tgenScorePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupOrScorePartExpected( os, i+1, v );
                os << std::endl;
                tgenPartGroupOrScorePartExpected( os, i+1, variant::one );
                os << std::endl;
                tgenPartGroupOrScorePartExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i, R"(</part-list>)", false );
            }
                break;
            case variant::three:
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
