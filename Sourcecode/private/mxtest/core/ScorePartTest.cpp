// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/ScorePartTest.h"
#include "mxtest/core/IdentificationTest.h"
#include "mxtest/core/PartNameDisplayTest.h"
#include "mxtest/core/PartAbbreviationDisplayTest.h"
#include "mxtest/core/ScoreInstrumentTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, ScorePart )
{
    variant v = variant::one;
	ScorePartPtr object = tgenScorePart( v );
	stringstream expected;
	tgenScorePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test02, ScorePart )
{
    variant v = variant::two;
	ScorePartPtr object = tgenScorePart( v );
	stringstream expected;
	tgenScorePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, ScorePart )
{
    variant v = variant::three;
	ScorePartPtr object = tgenScorePart( v );
	stringstream expected;
	tgenScorePartExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    ScorePartPtr tgenScorePart( variant v )
    {
        ScorePartPtr o = makeScorePart();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->getAttributes()->id = XsID( "P001" );
                o->setHasIdentification( true );
                o->setIdentification( tgenIdentification( v ) );
                o->getPartName()->setValue( XsString( "Trumpet" ) );
                o->setHasPartNameDisplay( true );
                o->setPartNameDisplay( tgenPartNameDisplay( v ) );
                o->setHasPartAbbreviation( true );
                o->getPartAbbreviation()->setValue( XsString( "Trp." ) );
                o->setHasPartAbbreviationDisplay( true );
                o->setPartAbbreviationDisplay( tgenPartAbbreviationDisplay( v ) );
                auto g = makeGroup();
                g->setValue( XsString ("Groupie" ) );
                o->addGroup( g );
                auto s = tgenScoreInstrument( v );
                o->addScoreInstrument( s );
                
                auto m = makeMidiDeviceInstrumentGroup();
                o->addMidiDeviceInstrumentGroup( m ); /* no values, weird! */
                m.reset();
                
                m = makeMidiDeviceInstrumentGroup();
                m->setHasMidiDevice( true );
                m->getMidiDevice()->setValue( XsString( "SomeDevice1" ) );
                o->addMidiDeviceInstrumentGroup( m );
                m.reset();
                
                m = makeMidiDeviceInstrumentGroup();
                m->setHasMidiDevice( true );
                m->getMidiDevice()->setValue( XsString( "SomeDevice2" ) );
                m->setHasMidiInstrument( true );
                m->getMidiInstrument()->getAttributes()->id = XsID( "Instrument2" );
                m->getMidiInstrument()->setHasVolume( true );
                m->getMidiInstrument()->getVolume()->setValue( Percent( 0.99) );
                m->getMidiInstrument()->setHasMidiChannel( true );
                m->getMidiInstrument()->getMidiChannel()->setValue( Midi16( 16 ) );
                o->addMidiDeviceInstrumentGroup( m );
                m.reset();
                
                m = makeMidiDeviceInstrumentGroup();
                m->setHasMidiInstrument( true );
                m->getMidiInstrument()->getAttributes()->id = XsID( "Instrument3" );
                m->getMidiInstrument()->setHasVolume( true );
                m->getMidiInstrument()->getVolume()->setValue( Percent( 0.11) );
                m->getMidiInstrument()->setHasMidiChannel( true );
                m->getMidiInstrument()->getMidiChannel()->setValue( Midi16( 13 ) );
                o->addMidiDeviceInstrumentGroup( m );
                m.reset();
                
            }
                break;
            case variant::three:
            {
                o->getAttributes()->id = XsID( "XXX123" );
                o->setHasIdentification( true );
                o->setIdentification( tgenIdentification( v ) );
                o->getPartName()->setValue( XsString( "Ruck" ) );
                o->setHasPartNameDisplay( true );
                o->setPartNameDisplay( tgenPartNameDisplay( v ) );
                o->setHasPartAbbreviation( true );
                o->getPartAbbreviation()->setValue( XsString( "R." ) );
                o->setHasPartAbbreviationDisplay( true );
                o->setPartAbbreviationDisplay( tgenPartAbbreviationDisplay( v ) );
                auto g = makeGroup();
                g->setValue( XsString ("Roapie" ) );
                o->addGroup( g );
                auto s = tgenScoreInstrument( v );
                o->addScoreInstrument( s );
                
                auto m = makeMidiDeviceInstrumentGroup();
                o->addMidiDeviceInstrumentGroup( m ); /* no values, weird! */
                m.reset();
                
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenScorePartExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<score-part id="ID">)" );
                streamLine( os, i+1, R"(<part-name></part-name>)" );
                streamLine( os, i, R"(</score-part>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<score-part id="P001">)" );
                tgenIdentificationExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<part-name>Trumpet</part-name>)" );
                tgenPartNameDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<part-abbreviation>Trp.</part-abbreviation>)" );
                tgenPartAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group>Groupie</group>)" );
                tgenScoreInstrumentExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<midi-device>SomeDevice1</midi-device>)" );
                streamLine( os, i+1, R"(<midi-device>SomeDevice2</midi-device>)" );
                streamLine( os, i+1, R"(<midi-instrument id="Instrument2">)" );
                streamLine( os, i+2, R"(<midi-channel>16</midi-channel>)" );
                streamLine( os, i+2, R"(<volume>0.99</volume>)" );
                streamLine( os, i+1, R"(</midi-instrument>)" );
                streamLine( os, i+1, R"(<midi-instrument id="Instrument3">)" );
                streamLine( os, i+2, R"(<midi-channel>13</midi-channel>)" );
                streamLine( os, i+2, R"(<volume>0.11</volume>)" );
                streamLine( os, i+1, R"(</midi-instrument>)" );
                streamLine( os, i, R"(</score-part>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<score-part id="XXX123">)" );
                tgenIdentificationExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<part-name>Ruck</part-name>)" );
                tgenPartNameDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<part-abbreviation>R.</part-abbreviation>)" );
                tgenPartAbbreviationDisplayExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<group>Roapie</group>)" );
                tgenScoreInstrumentExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i, R"(</score-part>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
