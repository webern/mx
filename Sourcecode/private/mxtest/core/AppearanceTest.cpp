// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"
#include "mxtest/core/AppearanceTest.h"
#include "mxtest/core/MidiInstrumentTest.h"
/* #include "MidiDeviceTest.cpp" */


using namespace mx::core;
using namespace std;
using namespace mxtest;
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

TEST( Test01, Appearance )
{
    variant v = variant::one;
	AppearancePtr object = tgenAppearance( v );
	stringstream expected;
	tgenAppearanceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( ! object->hasContents() )
}
TEST( Test02, Appearance )
{
    variant v = variant::two;
	AppearancePtr object = tgenAppearance( v );
	stringstream expected;
	tgenAppearanceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
TEST( Test03, Appearance )
{
    variant v = variant::three;
	AppearancePtr object = tgenAppearance( v );
	stringstream expected;
	tgenAppearanceExpected( expected, 1, v );
	stringstream actual;
	// object->toStream( std::cout, 1 );
	object->toStream( actual, 1 );
	CHECK_EQUAL( expected.str(), actual.str() )
	CHECK( ! object->hasAttributes() )
	CHECK( object->hasContents() )
}
#endif
namespace mxtest
{
    AppearancePtr tgenAppearance( variant v )
    {
        AppearancePtr o = makeAppearance();
        switch ( v )
        {
            case variant::one:
            {
                
            }
                break;
            case variant::two:
            {
                OtherAppearancePtr oa1 = makeOtherAppearance();
                OtherAppearancePtr oa2 = makeOtherAppearance();
                LineWidthPtr lw1 = makeLineWidth();
                LineWidthPtr lw2 = makeLineWidth();
                DistancePtr d1 = makeDistance();
                oa1->setValue( XsString( "oa1" ) );
                oa2->setValue( XsString( "oa2" ) );
                oa1->getAttributes()->type = XsToken( "T1" );
                oa2->getAttributes()->type = XsToken( "T2" );
                lw1->setValue( TenthsValue( 11.1 ) );
                lw2->setValue( TenthsValue( 11.2 ) );
                lw1->getAttributes()->type = LineWidthType( LineWidthTypeEnum::dashes );
                lw2->getAttributes()->type = LineWidthType( LineWidthTypeEnum::slurTip );
                d1->setValue( TenthsValue( 13.3 ) );
                d1->getAttributes()->type = DistanceType( DistanceTypeEnum::hyphen );
                o->addDistance( d1 );
                o->addLineWidth( lw1 );
                o->addLineWidth( lw2 );
                o->addOtherAppearance( oa1 );
                o->addOtherAppearance( oa2 );
            }
                break;
            case variant::three:
            {
                OtherAppearancePtr oa1 = makeOtherAppearance();
                OtherAppearancePtr oa2 = makeOtherAppearance();
                LineWidthPtr lw1 = makeLineWidth();
                LineWidthPtr lw2 = makeLineWidth();
                DistancePtr d1 = makeDistance();
                oa1->setValue( XsString( "xa1" ) );
                oa2->setValue( XsString( "xa2" ) );
                oa1->getAttributes()->type = XsToken( "T1x" );
                oa2->getAttributes()->type = XsToken( "Tx2" );
                lw1->setValue( TenthsValue( 1.1 ) );
                lw2->setValue( TenthsValue( 1.2 ) );
                lw1->getAttributes()->type = LineWidthType( LineWidthTypeEnum::dashes );
                lw2->getAttributes()->type = LineWidthType( LineWidthTypeEnum::slurTip );
                d1->setValue( TenthsValue( 3.3 ) );
                d1->getAttributes()->type = DistanceType( DistanceTypeEnum::hyphen );
                o->addDistance( d1 );
                o->addLineWidth( lw1 );
                o->addLineWidth( lw2 );
                o->addOtherAppearance( oa1 );
                o->addOtherAppearance( oa2 );
                o->addNoteSize( makeNoteSize( NonNegativeDecimal( 3 ) ) );
            }
                break;
            default:
                break;
        }
        return o;
    }
    void tgenAppearanceExpected( std::ostream& os, int i, variant v )
    {
        
        switch ( v )
        {
            case variant::one:
            {
                streamLine( os, i, R"(<appearance/>)", false );
            }
                break;
            case variant::two:
            {
                streamLine( os, i, R"(<appearance>)" );
                streamLine( os, i+1, R"(<line-width type="dashes">11.1</line-width>)" );
                streamLine( os, i+1, R"(<line-width type="slur tip">11.2</line-width>)" );
                streamLine( os, i+1, R"(<distance type="hyphen">13.3</distance>)" );
                streamLine( os, i+1, R"(<other-appearance type="T1">oa1</other-appearance>)" );
                streamLine( os, i+1, R"(<other-appearance type="T2">oa2</other-appearance>)" );
                streamLine( os, i, R"(</appearance>)", false );
            }
                break;
            case variant::three:
            {
                streamLine( os, i, R"(<appearance>)" );
                streamLine( os, i+1, R"(<line-width type="dashes">1.1</line-width>)" );
                streamLine( os, i+1, R"(<line-width type="slur tip">1.2</line-width>)" );
                streamLine( os, i+1, R"(<note-size type="large">3</note-size>)" );
                streamLine( os, i+1, R"(<distance type="hyphen">3.3</distance>)" );
                streamLine( os, i+1, R"(<other-appearance type="T1x">xa1</other-appearance>)" );
                streamLine( os, i+1, R"(<other-appearance type="Tx2">xa2</other-appearance>)" );
                streamLine( os, i, R"(</appearance>)", false );
            }
                break;
            default:
                break;
        }
    }
}

#endif
