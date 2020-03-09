// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/TupletActualTest.h"
#include "mxtest/core/MidiInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, TupletActual )
{
    variant v = variant::one;
	TupletActualPtr object = tgenTupletActual( v );
	stringstream expected;
	tgenTupletActualExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, TupletActual )
{
    variant v = variant::two;
	TupletActualPtr object = tgenTupletActual( v );
	stringstream expected;
	tgenTupletActualExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, TupletActual )
{
    variant v = variant::three;
	TupletActualPtr object = tgenTupletActual( v );
	stringstream expected;
	tgenTupletActualExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    TupletActualPtr tgenTupletActual( variant v )
    {
        TupletActualPtr o = makeTupletActual();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 2 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::sixteenth );
                o->addTupletDot( makeTupletDot() );
            }
                break;
            case variant::three:
            {
                o->setHasTupletNumber( true );
                o->getTupletNumber()->setValue( NonNegativeInteger( 3 ) );
                o->setHasTupletType( true );
                o->getTupletType()->setValue( NoteTypeValue::half );
                o->addTupletDot( makeTupletDot() );
                o->addTupletDot( makeTupletDot() );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenTupletActualExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<tuplet-actual/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<tuplet-actual>)" );
                streamLine( os, i+1, R"(<tuplet-number>2</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>16th</tuplet-type>)" );
                streamLine( os, i+1, R"(<tuplet-dot/>)" );
                streamLine( os, i, R"(</tuplet-actual>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<tuplet-actual>)" );
                streamLine( os, i+1, R"(<tuplet-number>3</tuplet-number>)" );
                streamLine( os, i+1, R"(<tuplet-type>half</tuplet-type>)" );
                streamLine( os, i+1, R"(<tuplet-dot/>)" );
                streamLine( os, i+1, R"(<tuplet-dot/>)" );
                streamLine( os, i, R"(</tuplet-actual>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
