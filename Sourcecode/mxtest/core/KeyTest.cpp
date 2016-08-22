// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( Test01, Key )
{
    Key object;
    object.getKeyChoice()->getTraditionalKey()->getFifths()->setValue( FifthsValue{ 3 } );
    object.getKeyChoice()->getTraditionalKey()->setHasCancel( true );
    object.getKeyChoice()->getTraditionalKey()->getCancel()->setValue( FifthsValue{ 5 } );
    object.getKeyChoice()->getTraditionalKey()->setHasMode( true );
    object.getKeyChoice()->getTraditionalKey()->getMode()->setValue( ModeValue{ ModeEnum::dorian } );
    object.getAttributes()->hasPrintObject = true;
    object.getAttributes()->printObject = YesNo::yes;
    object.getAttributes()->hasFontWeight = true;
    object.getAttributes()->fontWeight = FontWeight::bold;
    stringstream expected;
    streamLine( expected, 1, R"(<key font-weight="bold" print-object="yes">)" );
    streamLine( expected, 2, R"(<cancel>5</cancel>)" );
    streamLine( expected, 2, R"(<fifths>3</fifths>)" );
    streamLine( expected, 2, R"(<mode>dorian</mode>)" );
    streamLine( expected, 1, R"(</key>)", false );
    stringstream actual;
    object.toStream( actual, 1 );
    CHECK_EQUAL( expected.str(), actual.str() )
}

TEST( Test02, TraditionalKey )
{
    Key object;
    object.getAttributes()->hasNumber = true;
    object.getAttributes()->number = StaffNumber{ 2 };
    object.getKeyChoice()->setChoice( KeyChoice::Choice::nonTraditionalKey );
    NonTraditionalKeyPtr ntk1 = makeNonTraditionalKey();
    ntk1->setHasKeyAccidental( true );
    ntk1->setKeyAccidental( std::make_shared<KeyAccidental>( AccidentalValue::flat ) );
    ntk1->setKeyAlter( std::make_shared<KeyAlter>( Semitones{ -0.2 } ) );
    object.getKeyChoice()->addNonTraditionalKey( ntk1 );
    NonTraditionalKeyPtr ntk2 = makeNonTraditionalKey();
    ntk2->setHasKeyAccidental( true );
    ntk2->setKeyAccidental( std::make_shared<KeyAccidental>( AccidentalValue::sharp ) );
    ntk2->setKeyAlter( std::make_shared<KeyAlter>( Semitones{ 2 } ) );
    object.getKeyChoice()->addNonTraditionalKey( ntk2 );
    stringstream expected;
    streamLine( expected, 1, R"(<key number="2">)" );
    streamLine( expected, 2, R"(<key-step>A</key-step>)" );
    streamLine( expected, 2, R"(<key-alter>-0.2</key-alter>)" );
    streamLine( expected, 2, R"(<key-accidental>flat</key-accidental>)" );
    streamLine( expected, 2, R"(<key-step>A</key-step>)" );
    streamLine( expected, 2, R"(<key-alter>2</key-alter>)" );
    streamLine( expected, 2, R"(<key-accidental>sharp</key-accidental>)" );
    streamLine( expected, 1, R"(</key>)", false );
    stringstream actual;
    object.toStream( actual, 1 );
    CHECK_EQUAL( expected.str(), actual.str() )
}

#endif
