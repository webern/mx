// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/TimeModificationTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TimeModification )
{
    variant v = variant::one;
	TimeModificationPtr object = tgenTimeModification( v );
	stringstream expected;
	tgenTimeModificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, TimeModification )
{
    variant v = variant::two;
	TimeModificationPtr object = tgenTimeModification( v );
	stringstream expected;
	tgenTimeModificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TimeModification )
{
    variant v = variant::three;
	TimeModificationPtr object = tgenTimeModification( v );
	stringstream expected;
	tgenTimeModificationExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    NormalTypeNormalDotGroupPtr tgenNormalTypeNormalDotGroup( variant v )
    {
        NormalTypeNormalDotGroupPtr o = makeNormalTypeNormalDotGroup();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getNormalType()->setValue( NoteTypeValue::half );
                o->addNormalDot( makeNormalDot() );
                o->addNormalDot( makeNormalDot() );

            }
                break;
            case variant::three:
            {
                o->getNormalType()->setValue( NoteTypeValue::thirtySecond );
            }
                break;
            default:
                break;
        }
        return o;
    }
    TimeModificationPtr tgenTimeModification( variant v )
    {
        TimeModificationPtr o = makeTimeModification();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasNormalTypeNormalDotGroup( true );
                o->setNormalTypeNormalDotGroup( tgenNormalTypeNormalDotGroup( v ) );
                o->getActualNotes()->setValue( NonNegativeInteger( 5 ) );
                o->getNormalNotes()->setValue( NonNegativeInteger( 4 ) );
            }
                break;
            case variant::three:
            {
                o->setHasNormalTypeNormalDotGroup( true );
                o->setNormalTypeNormalDotGroup( tgenNormalTypeNormalDotGroup( v ) );
                o->getActualNotes()->setValue( NonNegativeInteger( 4 ) );
                o->getNormalNotes()->setValue( NonNegativeInteger( 3 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTimeModificationExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<time-modification>)" );
                streamLine( os, i+1, R"(<actual-notes>0</actual-notes>)" );
                streamLine( os, i+1, R"(<normal-notes>0</normal-notes>)" );
                streamLine( os, i, R"(</time-modification>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<time-modification>)" );
                streamLine( os, i+1, R"(<actual-notes>5</actual-notes>)" );
                streamLine( os, i+1, R"(<normal-notes>4</normal-notes>)" );
                streamLine( os, i+1, R"(<normal-type>half</normal-type>)" );
                streamLine( os, i+1, R"(<normal-dot/>)" );
                streamLine( os, i+1, R"(<normal-dot/>)" );
                streamLine( os, i, R"(</time-modification>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<time-modification>)" );
                streamLine( os, i+1, R"(<actual-notes>4</actual-notes>)" );
                streamLine( os, i+1, R"(<normal-notes>3</normal-notes>)" );
                streamLine( os, i+1, R"(<normal-type>32nd</normal-type>)" );
                streamLine( os, i, R"(</time-modification>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
