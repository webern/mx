// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/SoundTest.h"
#include "mxtest/core/PlayTest.h"
#include "mxtest/core/MidiInstrumentTest.h"


using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Sound )
{
    TestMode v = TestMode::one;
	SoundPtr object = tgenSound( v );
	stringstream expected;
	tgenSoundExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Sound )
{
    TestMode v = TestMode::two;
	SoundPtr object = tgenSound( v );
	stringstream expected;
	tgenSoundExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Sound )
{
    TestMode v = TestMode::three;
	SoundPtr object = tgenSound( v );
	stringstream expected;
	tgenSoundExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    SoundPtr tgenSound( TestMode v )
    {
        SoundPtr o = makeSound();
        switch ( v )
        {
            case TestMode::one:
            {
                
            }
                break;
            case TestMode::two:
            {
                o->getAttributes()->hasCoda = true;
                o->getAttributes()->coda = XsToken( "TestCoda" );
                o->getAttributes()->hasPizzicato = true;
                o->getAttributes()->pizzicato = YesNo::yes;
                o->setHasMidiInstrument( true );
                o->setMidiInstrument( tgenMidiInstrument( v ) );
                o->setHasMidiDevice( true );
                o->getMidiDevice()->setValue( XsString( "lello" ) );
            }
                break;
            case TestMode::three:
            {
                o->getAttributes()->hasSoftPedal = true;
                o->getAttributes()->softPedal = YesNoNumber( Decimal( 0.1 ) );
                o->setHasPlay( true );
                o->setPlay( tgenPlay( v ) );
                o->setHasMidiInstrument( true );
                o->setMidiInstrument( tgenMidiInstrument( v ) );
                o->setHasMidiDevice( true );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenSoundExpected(std::ostream& os, int i, TestMode v )
    {
        
        switch ( v )
        {
            case TestMode::one:
            {
                streamLine( os, i, R"(<sound/>)", false );
            }
                break;
            case TestMode::two:
            {
                streamLine( os, i, R"(<sound coda="TestCoda" pizzicato="yes">)" );
                streamLine( os, i+1, R"(<midi-device>lello</midi-device>)" );
                tgenMidiInstrumentExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</sound>)", false );
            }
                break;
            case TestMode::three:
            {
                streamLine( os, i, R"(<sound soft-pedal="0.1">)" );
                streamLine( os, i+1, R"(<midi-device></midi-device>)" );
                tgenMidiInstrumentExpected( os, i+1, v );
                os << std::endl;
                tgenPlayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</sound>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
