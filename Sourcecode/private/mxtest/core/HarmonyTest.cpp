// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/HarmonyTest.h"
#include "mxtest/core/EditorialGroupTest.h"
#include "mxtest/core/HarmonyChordGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Harmony )
{
    variant v = variant::one;
	HarmonyPtr object = tgenHarmony( v );
	stringstream expected;
	tgenHarmonyExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, Harmony )
{
    variant v = variant::two;
	HarmonyPtr object = tgenHarmony( v );
	stringstream expected;
	tgenHarmonyExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Harmony )
{
    variant v = variant::three;
	HarmonyPtr object = tgenHarmony( v );
	stringstream expected;
	tgenHarmonyExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    HarmonyPtr tgenHarmony( variant v )
    {
        HarmonyPtr o = makeHarmony();
        switch ( v )
        {
            case variant::one:
            {
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::one ) );
                o->removeHarmonyChordGroup( o->getHarmonyChordGroupSet().cbegin() );
            }
                break;
            case variant::two:
            {
                o->getAttributes()->hasPlacement = true;
                o->getAttributes()->placement = AboveBelow::below;
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::one ) );
                o->removeHarmonyChordGroup( o->getHarmonyChordGroupSet().cbegin() );
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::two ) );
                o->setHasFrame( true );
                o->getFrame()->getFrameStrings()->setValue( PositiveInteger( 3 ) );
                o->setHasStaff( true );
                o->getStaff()->setValue( PositiveInteger( 2 ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->hasDefaultX = true;
                o->getAttributes()->defaultX = TenthsValue( 5 );
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::one ) );
                o->removeHarmonyChordGroup( o->getHarmonyChordGroupSet().cbegin() );
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::two ) );
                o->addHarmonyChordGroup( tgenHarmonyChordGroup( variant::three ) );
                o->setHasOffset( true );
                o->getOffset()->setValue( DivisionsValue( 1.1 ) );
                o->setEditorialGroup( tgenEditorialGroup( v ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHarmonyExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<harmony>)" );
                tgenHarmonyChordGroupExpected( os, i+1, variant::one );
                os << std::endl;
                streamLine( os, i, R"(</harmony>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<harmony placement="below">)" );
                tgenHarmonyChordGroupExpected( os, i+1, variant::one );
                os << std::endl;
                tgenHarmonyChordGroupExpected( os, i+1, variant::two );
                os << std::endl;
                streamLine( os, i+1, R"(<frame>)" );
                streamLine( os, i+2, R"(<frame-strings>3</frame-strings>)" );
                streamLine( os, i+2, R"(<frame-frets>1</frame-frets>)" );
                streamLine( os, i+2, R"(<frame-note>)" );
                streamLine( os, i+3, R"(<string>1</string>)" );
                streamLine( os, i+3, R"(<fret>1</fret>)" );
                streamLine( os, i+2, R"(</frame-note>)" );
                streamLine( os, i+1, R"(</frame>)" );
                streamLine( os, i+1, R"(<staff>2</staff>)" );
                streamLine( os, i, R"(</harmony>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<harmony default-x="5">)" );
                tgenHarmonyChordGroupExpected( os, i+1, variant::one );
                os << std::endl;
                tgenHarmonyChordGroupExpected( os, i+1, variant::two );
                os << std::endl;
                tgenHarmonyChordGroupExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i+1, R"(<offset>1.1</offset>)" );
                tgenEditorialGroupExpected( os, i+1, variant::three );
                os << std::endl;
                streamLine( os, i, R"(</harmony>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif

