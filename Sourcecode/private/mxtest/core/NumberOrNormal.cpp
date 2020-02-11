// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/testFramework.h"
#include "mx/core/NumberOrNormal.h"
#include <sstream>

using namespace mx::core;

TEST( NumberOrNormal01, NumberOrNormal )
{
    NumberOrNormal x;
    std::string expected = "normal";
    CHECK( x.getIsNormal() )
    CHECK( !x.getIsNumber() )
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( NumberOrNormal02, NumberOrNormal )
{
    NumberOrNormal x( Decimal( 31.2 ) );
    CHECK( !x.getIsNormal() )
    CHECK( x.getIsNumber() )
    std::string expected = "31.2";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( NumberOrNormal03, NumberOrNormal )
{
    NumberOrNormal x( "normal" );
    CHECK( x.getIsNormal() )
    CHECK( !x.getIsNumber() )
    std::string expected = "normal";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( NumberOrNormal04, NumberOrNormal )
{
    NumberOrNormal x( "XXX" );
    CHECK( !x.getIsNormal() )
    CHECK( x.getIsNumber() )
    std::string expected = "0";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( NumberOrNormal05, NumberOrNormal )
{
    NumberOrNormal x( "24.0" );
    CHECK( ! x.getIsNormal() )
    CHECK( x.getIsNumber() )
    DecimalType expected = 24;
    DecimalType actual = x.getValueNumber().getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, DEFAULT_PRECISION )
}
TEST( NumberOrNormal06, NumberOrNormal )
{
    NumberOrNormal x( "24.0" );
    x.setValueNormal();
    CHECK( x.getIsNormal() )
    CHECK( ! x.getIsNumber() )
    std::string expected = "normal";
    std::stringstream ss;
    ss << x;
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( NumberOrNormal07, NumberOrNormal )
{
    NumberOrNormal x( "normal" );
    x.setValue( Decimal( 30.9 ) );
    CHECK( ! x.getIsNormal() )
    CHECK( x.getIsNumber() )
    std::string expected = "30.9";
    std::stringstream ss;
    toStream( ss, x );
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

#endif
