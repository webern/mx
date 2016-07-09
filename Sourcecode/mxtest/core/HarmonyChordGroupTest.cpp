// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/HarmonyChordGroupTest.h"
#include "mxtest/core/VirtualInstrumentTest.h"
#include "mxtest/core/EncodingTest.h"
#include "mxtest/core/RootTest.h"
#include "mxtest/core/BassTest.h"
#include "mxtest/core/DegreeTest.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, HarmonyChordGroup )
{
    variant v = variant::one;
	HarmonyChordGroupPtr object = tgenHarmonyChordGroup( v );
	stringstream expected;
	tgenHarmonyChordGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, HarmonyChordGroup )
{
    variant v = variant::two;
	HarmonyChordGroupPtr object = tgenHarmonyChordGroup( v );
	stringstream expected;
	tgenHarmonyChordGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, HarmonyChordGroup )
{
    variant v = variant::three;
	HarmonyChordGroupPtr object = tgenHarmonyChordGroup( v );
	stringstream expected;
	tgenHarmonyChordGroupExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
    bool isOneLineOnly = true;
	object->streamContents( actual, 1, isOneLineOnly );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    HarmonyChordGroupPtr tgenHarmonyChordGroup( variant v )
    {
        HarmonyChordGroupPtr o = makeHarmonyChordGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setRoot( tgenRoot( v ) );
                o->setChoice( HarmonyChordGroup::Choice::function );
                o->getFunction()->setValue( XsString("II" ) );
                o->getKind()->setValue( KindValue::diminished );
                o->setHasBass( true );
                o->setBass( tgenBass( v ) );
                o->addDegree( tgenDegree( v ) );
            }
                break;
            case variant::three:
            {
                o->setRoot( tgenRoot( v ) );
                o->setChoice( HarmonyChordGroup::Choice::root );
                o->getKind()->setValue( KindValue::dominant13Th );
                o->setHasBass( true );
                o->setBass( tgenBass( v ) );
                o->addDegree( tgenDegree( variant::one ) );
                o->addDegree( tgenDegree( variant::three ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenHarmonyChordGroupExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                tgenRootExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<kind>none</kind>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<function>II</function>)" );
                streamLine( os, i, R"(<kind>diminished</kind>)" );
                tgenBassExpected( os, i, v );
                os << std::endl;
                tgenDegreeExpected( os, i, v );
            }
                break;
            case variant::three:
            {
                tgenRootExpected( os, i, v );
                os << std::endl;
                streamLine( os, i, R"(<kind>dominant-13th</kind>)" );
                tgenBassExpected( os, i, v );
                os << std::endl;
                tgenDegreeExpected( os, i, variant::one );
                os << std::endl;
                tgenDegreeExpected( os, i, variant::three );
            }
                break;
            default:
                break;
        }
    }
}

#endif
