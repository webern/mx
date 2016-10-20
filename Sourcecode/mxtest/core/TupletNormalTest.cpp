// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/TupletNormalTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TupletNormal )
{
    variant v = variant::one;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, TupletNormal )
{
    variant v = variant::two;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TupletNormal )
{
    variant v = variant::three;
	TupletNormalPtr object = tgenTupletNormal( v );
	stringstream expected;
	tgenTupletNormalExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TupletNormalPtr tgenTupletNormal( variant v )
    {
        TupletNormalPtr o = makeTupletNormal();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 5 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::eighth );
            }
                break;
            case variant::three:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 3 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::quarter );
                o->addTupletDot( makeTupletDot() );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTupletNormalExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<tuplet-normal/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<tuplet-normal>)" );
                streamLine( os, i+1, R"(<tuplet-number>5</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>eighth</tuplet-type>)" );
                streamLine( os, i, R"(</tuplet-normal>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<tuplet-normal>)" );
                streamLine( os, i+1, R"(<tuplet-number>3</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>quarter</tuplet-type>)" );
                streamLine( os, i+1, R"(<tuplet-dot/>)" );
                streamLine( os, i, R"(</tuplet-normal>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
