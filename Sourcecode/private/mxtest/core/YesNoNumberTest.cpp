// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/YesNoNumber.h"
#include <sstream>

using namespace mx::core;

TEST( YesNoNumber01, YesNoNumber )
{
    YesNoNumber x;
    std::string expected = "no";
    CHECK( x.getIsYesNo() )
    CHECK( !x.getIsNumber() )
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( YesNoNumber02, YesNoNumber )
{
    YesNoNumber x( Decimal( 31.2 ) );
    CHECK( !x.getIsYesNo() )
    CHECK( x.getIsNumber() )
    std::string expected = "31.2";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( YesNoNumber03, YesNoNumber )
{
    YesNoNumber x( YesNo::yes );
    CHECK( x.getIsYesNo() )
    CHECK( !x.getIsNumber() )
    std::string expected = "yes";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( YesNoNumber04, YesNoNumber )
{
    YesNoNumber x( "no" );
    CHECK( x.getIsYesNo() )
    CHECK( !x.getIsNumber() )
    YesNo expected = YesNo::no;
    YesNo actual = x.getValueYesNo();
    CHECK_EQUAL( expected, actual )
}
TEST( YesNoNumber05, YesNoNumber )
{
    YesNoNumber x( "24.0" );
    CHECK( ! x.getIsYesNo() )
    CHECK( x.getIsNumber() )
    DecimalType expected = 24;
    DecimalType actual = x.getValueNumber().getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, DEFAULT_PRECISION )
}
TEST( YesNoNumber06, YesNoNumber )
{
    YesNoNumber x( "24.0" );
    x.setValue( YesNo::yes );
    CHECK( x.getIsYesNo() )
    CHECK( ! x.getIsNumber() )
    std::string expected = "yes";
    std::stringstream ss;
    ss << x;
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( YesNoNumber07, YesNoNumber )
{
    YesNoNumber x( "yes" );
    x.setValue( Decimal( 30.9 ) );
    CHECK( ! x.getIsYesNo() )
    CHECK( x.getIsNumber() )
    std::string expected = "30.9";
    std::stringstream ss;
    toStream( ss, x );
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

#endif
