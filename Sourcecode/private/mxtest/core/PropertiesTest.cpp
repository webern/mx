// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mxtest/core/PropertiesTest.h"
#include "mxtest/core/EditorialGroupTest.h"

using namespace mx::core;
using namespace std;
using namespace mxtest;

TEST( Test01, Properties )
{
    variant v = variant::one;
	PropertiesPtr object = tgenProperties( v );
	stringstream expected;
	tgenPropertiesExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Properties )
{
    variant v = variant::two;
	PropertiesPtr object = tgenProperties( v );
	stringstream expected;
	tgenPropertiesExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Properties )
{
    variant v = variant::three;
	PropertiesPtr object = tgenProperties( v );
	stringstream expected;
	tgenPropertiesExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}

namespace mxtest
{
    PropertiesPtr tgenProperties( variant v )
    {
        PropertiesPtr o = makeProperties();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                auto d = makeDivisions();
                d->setValue( PositiveDivisionsValue( 120 ) );
                o->setDivisions( d );
                o->setHasDivisions( true );
                auto k = makeKey();
                k->getKeyChoice()->setChoice( KeyChoice::Choice::traditionalKey );
                k->getKeyChoice()->getTraditionalKey()->getFifths()->setValue( FifthsValue( -2 ) );
                k->getKeyChoice()->getTraditionalKey()->setHasCancel( true );
                k->getKeyChoice()->getTraditionalKey()->getCancel()->setValue( FifthsValue ( 1 ) );
                o->addKey( k );
                
                auto t = makeTime();
                t->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
                auto timeSignature = makeTimeSignatureGroup();
                timeSignature->getBeats()->setValue( XsString( "3" ) );
                timeSignature->getBeatType()->setValue( XsString( "4" ) );
                t->getTimeChoice()->addTimeSignatureGroup( timeSignature );
                t->getTimeChoice()->removeTimeSignatureGroup( t->getTimeChoice()->getTimeSignatureGroupSet().cbegin() );
                
                o->addTime( t );
                o->setHasStaves( true );
                o->getStaves()->setValue( NonNegativeInteger( 4 ) );
                o->setHasPartSymbol( true );
                o->getPartSymbol()->setValue( GroupSymbolValue::none );
                o->setHasInstruments( true );
                o->getInstruments()->setValue( NonNegativeInteger( 3 ) );
                auto c = makeClef();
                c->getSign()->setValue( ClefSign::g );
                c->getLine()->setValue( StaffLine( 2 ) );
                c->setHasLine( true );
                o->addClef( c );
                auto sd = makeStaffDetails();
                sd->setHasStaffLines( true );
                sd->getStaffLines()->setValue( NonNegativeInteger( 5 ) );
                sd->setHasStaffType( true );
                sd->getStaffType()->setValue( StaffTypeEnum::regular );
                o->addStaffDetails( sd );
                auto dir = makeDirective();
                dir->setValue( XsString( "allegro" ) );
                dir->getAttributes()->hasLang = true;
                o->addDirective( dir );
                auto ms = makeMeasureStyle();
                ms->getMeasureStyleChoice()->setChoice( MeasureStyleChoice::Choice::slash );
                ms->getMeasureStyleChoice()->getSlash()->getSlashType()->setValue( NoteTypeValue::eighth );
                o->addMeasureStyle( ms );
            }
                break;
            case variant::three:
            {
                o->setEditorialGroup( tgenEditorialGroup( v ) );
                auto d = makeDivisions();
                d->setValue( PositiveDivisionsValue( 99 ) );
                o->setDivisions( d );
                o->setHasDivisions( true );
                auto k = makeKey();
                k->getKeyChoice()->setChoice( KeyChoice::Choice::traditionalKey );
                k->getKeyChoice()->getTraditionalKey()->getFifths()->setValue( FifthsValue( -2 ) );
                k->getKeyChoice()->getTraditionalKey()->setHasCancel( true );
                k->getKeyChoice()->getTraditionalKey()->getCancel()->setValue( FifthsValue ( 1 ) );
                o->addKey( k );
                
                auto t = makeTime();
                t->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
                auto timeSignature = makeTimeSignatureGroup();
                timeSignature->getBeats()->setValue( XsString( "5" ) );
                timeSignature->getBeatType()->setValue( XsString( "4" ) );
                t->getTimeChoice()->addTimeSignatureGroup( timeSignature );
                t->getTimeChoice()->removeTimeSignatureGroup( t->getTimeChoice()->getTimeSignatureGroupSet().cbegin() );
                
                o->addTime( t );
                o->setHasStaves( true );
                o->getStaves()->setValue( NonNegativeInteger( 4 ) );
                o->setHasPartSymbol( true );
                o->getPartSymbol()->setValue( GroupSymbolValue::none );
                o->setHasInstruments( true );
                o->getInstruments()->setValue( NonNegativeInteger( 3 ) );
                auto c = makeClef();
                c->getSign()->setValue( ClefSign::g );
                c->getLine()->setValue( StaffLine( 2 ) );
                c->setHasLine( true );
                o->addClef( c );
                c = makeClef();
                c->getSign()->setValue( ClefSign::c );
                c->getLine()->setValue( StaffLine( 3 ) );
                c->setHasLine( true );
                o->addClef( c );
                
//                auto sd = makeStaffDetails();
//                sd->setHasStaffLines( true );
//                sd->getStaffLines()->setValue( NonNegativeInteger( 5 ) );
//                sd->setHasStaffType( true );
//                sd->getStaffType()->setValue( StaffTypeEnum::regular );
//                o->addStaffDetails( sd );
                auto dir = makeDirective();
                dir->setValue( XsString( "allegro" ) );
                dir->getAttributes()->hasLang = true;
                o->addDirective( dir );
                auto ms = makeMeasureStyle();
                ms->getMeasureStyleChoice()->setChoice( MeasureStyleChoice::Choice::slash );
                ms->getMeasureStyleChoice()->getSlash()->getSlashType()->setValue( NoteTypeValue::eighth );
                o->addMeasureStyle( ms );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenPropertiesExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<attributes/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<attributes>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<divisions>120</divisions>)" );
                streamLine( os, i+1, R"(<key>)" );
                streamLine( os, i+2, R"(<cancel>1</cancel>)" );
                streamLine( os, i+2, R"(<fifths>-2</fifths>)" );
                streamLine( os, i+1, R"(</key>)" );
                streamLine( os, i+1, R"(<time>)" );
                streamLine( os, i+2, R"(<beats>3</beats>)" );
                streamLine( os, i+2, R"(<beat-type>4</beat-type>)" );
                streamLine( os, i+1, R"(</time>)" );
                streamLine( os, i+1, R"(<staves>4</staves>)" );
                streamLine( os, i+1, R"(<part-symbol>none</part-symbol>)" );
                streamLine( os, i+1, R"(<instruments>3</instruments>)" );
                streamLine( os, i+1, R"(<clef>)" );
                streamLine( os, i+2, R"(<sign>G</sign>)" );
                streamLine( os, i+2, R"(<line>2</line>)" );
                streamLine( os, i+1, R"(</clef>)" );
                streamLine( os, i+1, R"(<staff-details>)" );
                streamLine( os, i+2, R"(<staff-type>regular</staff-type>)" );
                streamLine( os, i+2, R"(<staff-lines>5</staff-lines>)" );
                streamLine( os, i+1, R"(</staff-details>)" );
                streamLine( os, i+1, R"(<directive xml:lang="it">allegro</directive>)" );
                streamLine( os, i+1, R"(<measure-style>)" );
                streamLine( os, i+2, R"(<slash type="start">)" );
                streamLine( os, i+3, R"(<slash-type>eighth</slash-type>)" );
                streamLine( os, i+2, R"(</slash>)" );
                streamLine( os, i+1, R"(</measure-style>)" );
                streamLine( os, i, R"(</attributes>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<attributes>)" );
                tgenEditorialGroupExpected( os, i+1, v );
                os << std::endl;
                streamLine( os, i+1, R"(<divisions>99</divisions>)" );
                streamLine( os, i+1, R"(<key>)" );
                streamLine( os, i+2, R"(<cancel>1</cancel>)" );
                streamLine( os, i+2, R"(<fifths>-2</fifths>)" );
                streamLine( os, i+1, R"(</key>)" );
                streamLine( os, i+1, R"(<time>)" );
                streamLine( os, i+2, R"(<beats>5</beats>)" );
                streamLine( os, i+2, R"(<beat-type>4</beat-type>)" );
                streamLine( os, i+1, R"(</time>)" );
                streamLine( os, i+1, R"(<staves>4</staves>)" );
                streamLine( os, i+1, R"(<part-symbol>none</part-symbol>)" );
                streamLine( os, i+1, R"(<instruments>3</instruments>)" );
                streamLine( os, i+1, R"(<clef>)" );
                streamLine( os, i+2, R"(<sign>G</sign>)" );
                streamLine( os, i+2, R"(<line>2</line>)" );
                streamLine( os, i+1, R"(</clef>)" );
                streamLine( os, i+1, R"(<clef>)" );
                streamLine( os, i+2, R"(<sign>C</sign>)" );
                streamLine( os, i+2, R"(<line>3</line>)" );
                streamLine( os, i+1, R"(</clef>)" );
//                streamLine( os, i+1, R"(<staff-details>)" );
//                streamLine( os, i+2, R"(<staff-type>regular</staff-type>)" );
//                streamLine( os, i+2, R"(<staff-lines>5</staff-lines>)" );
//                streamLine( os, i+1, R"(</staff-details>)" );
                streamLine( os, i+1, R"(<directive xml:lang="it">allegro</directive>)" );
                streamLine( os, i+1, R"(<measure-style>)" );
                streamLine( os, i+2, R"(<slash type="start">)" );
                streamLine( os, i+3, R"(<slash-type>eighth</slash-type>)" );
                streamLine( os, i+2, R"(</slash>)" );
                streamLine( os, i+1, R"(</measure-style>)" );
                streamLine( os, i, R"(</attributes>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
