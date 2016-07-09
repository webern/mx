// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, MidiInstrument )
{
    variant v = variant::one;
	MidiInstrumentPtr object = tgenMidiInstrument( v );
	stringstream expected;
	tgenMidiInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, MidiInstrument )
{
    variant v = variant::two;
	MidiInstrumentPtr object = tgenMidiInstrument( v );
	stringstream expected;
	tgenMidiInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, MidiInstrument )
{
    variant v = variant::three;
	MidiInstrumentPtr object = tgenMidiInstrument( v );
	stringstream expected;
	tgenMidiInstrumentExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace MxTestHelpers
{
    MidiInstrumentPtr tgenMidiInstrument( variant v )
    {
        MidiInstrumentPtr o = makeMidiInstrument();
        switch ( v )
        {
            case variant::one:
            {

            }
                break;
            case variant::two:
            {
                o->getAttributes()->id = XsIDREF( "M1" );
                o->setHasMidiChannel( true );
                o->setHasMidiName( true );
                o->setHasMidiBank( true );
                o->setHasMidiUnpitched( false );
                o->setHasVolume( true );
                o->setHasPan( true );
                o->setHasElevation( false );
                o->getMidiChannel()->setValue( Midi16( 2 ) );
                o->getMidiName()->setValue( XsString( "Trumpet" ) );
                o->getMidiBank()->setValue( Midi16384( 3 ) );
                o->getMidiUnpitched()->setValue( Midi128( 100 ) );
                o->getVolume()->setValue( Percent( 55.5 ) );
                o->getPan()->setValue( RotationDegrees( -88.6 ) );
                o->getElevation()->setValue( RotationDegrees( 101.112 ) );
            }
                break;
            case variant::three:
            {
                o->getAttributes()->id = XsIDREF( "X2" );
                o->setHasMidiChannel( true );
                o->setHasMidiName( true );
                o->setHasMidiBank( true );
                o->setHasMidiUnpitched( true );
                o->setHasVolume( true );
                o->setHasPan( false );
                o->setHasElevation( true );
                o->getMidiChannel()->setValue( Midi16( 3 ) );
                o->getMidiName()->setValue( XsString( "Bassoon" ) );
                o->getMidiBank()->setValue( Midi16384( 4 ) );
                o->getMidiUnpitched()->setValue( Midi128( 97 ) );
                o->getVolume()->setValue( Percent( 0.123 ) );
                o->getPan()->setValue( RotationDegrees( 120.3 ) );
                o->getElevation()->setValue( RotationDegrees( -33.333 ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenMidiInstrumentExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<midi-instrument id="ID"/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<midi-instrument id="M1">)" );
                streamLine( os, i+1, R"(<midi-channel>2</midi-channel>)" );
                streamLine( os, i+1, R"(<midi-name>Trumpet</midi-name>)" );
                streamLine( os, i+1, R"(<midi-bank>3</midi-bank>)" );
                streamLine( os, i+1, R"(<volume>55.5</volume>)" );
                streamLine( os, i+1, R"(<pan>-88.6</pan>)" );
                streamLine( os, i, R"(</midi-instrument>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<midi-instrument id="X2">)" );
                streamLine( os, i+1, R"(<midi-channel>3</midi-channel>)" );
                streamLine( os, i+1, R"(<midi-name>Bassoon</midi-name>)" );
                streamLine( os, i+1, R"(<midi-bank>4</midi-bank>)" );
                streamLine( os, i+1, R"(<midi-unpitched>97</midi-unpitched>)" );
                streamLine( os, i+1, R"(<volume>0.123</volume>)" );
                streamLine( os, i+1, R"(<elevation>-33.333</elevation>)" );
                streamLine( os, i, R"(</midi-instrument>)", false );
            }
                break;
            default:
                break;
        }
    }
    //    MidiInstrumentChoicePtr tgenMidiInstrumentChoice( variant v )
    //    {
    //        MidiInstrumentChoicePtr o = makeMidiInstrumentChoice();
    //        switch ( v )
    //        {
    //            case variant::one:
    //            {
    //                o->setChoice( MidiInstrumentChoice::Choice::stickType );
    //            }
    //                break;
    //            case variant::two:
    //            {
    //                o->setChoice( MidiInstrumentChoice::Choice::wood );
    //            }
    //                break;
    //            case variant::three:
    //            {
    //                o->setChoice( MidiInstrumentChoice::Choice::otherMidiInstrument );
    //                o->getOtherMidiInstrument()->setValue( XsString("Hello" ) );
    //            }
    //                break;
    //            default:
    //                break;
    //        }
    //        return o;
    //    }
}

#endif
