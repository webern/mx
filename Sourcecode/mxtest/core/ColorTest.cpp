// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Color.h"
#include <sstream>

using namespace mx::core;

TEST( Color01, Strings )
{
    Color x;
    std::string expected = "#FFFFFF";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( Color02, Strings )
{
    Color x( 1, 2, 3, 4 );
    std::string expected = "#01020304";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( Color03, Strings )
{
    Color x( 98, 97, 95 );
    std::string expected = "#62615F";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 255, x.getAlpha() )
    CHECK_EQUAL( 98, x.getRed() )
    CHECK_EQUAL( 97, x.getGreen() )
    CHECK_EQUAL( 95, x.getBlue() )
}

TEST( Color04, Strings )
{
    Color x( 98, 97, 200 );
    x.setColorType( Color::ColorType::ARGB );
    x.setRed( 201 );
    x.setGreen( 202 );
    x.setBlue( 212 );
    std::string expected = "#FFC9CAD4";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 255, x.getAlpha() )
    CHECK_EQUAL( 201, x.getRed() )
    CHECK_EQUAL( 202, x.getGreen() )
    CHECK_EQUAL( 212, x.getBlue() )
    x.setAlpha( 0 );
    CHECK_EQUAL( 0, x.getAlpha() )
    expected = "#00C9CAD4";
    actual = toString( x );
    CHECK_EQUAL( expected, actual )
    x.setColorType( Color::ColorType::RGB );
    expected = "#C9CAD4";
    actual = toString( x );
    CHECK_EQUAL( expected, actual )
    x.setColorType( Color::ColorType::ARGB );
    expected = "#FFC9CAD4";
    actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( Color05, Strings )
{
    Color x;
    x.parse( "#AB00CDEF" );
    std::string expected = "#AB00CDEF";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 171, x.getAlpha() )
    CHECK_EQUAL( 0, x.getRed() )
    CHECK_EQUAL( 205, x.getGreen() )
    CHECK_EQUAL( 239, x.getBlue() )
}
TEST( Color06, Strings )
{
    Color x;
    x.parse( "#00CDEF11" );
    std::string expected = "#00CDEF11";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 0, x.getAlpha() )
    CHECK_EQUAL( 205, x.getRed() )
    CHECK_EQUAL( 239, x.getGreen() )
    CHECK_EQUAL( 17, x.getBlue() )
    CHECK( Color::ColorType::ARGB == x.getColorType() );
}

TEST( Color07, Strings )
{
    Color x;
    x.parse( "#CDEF11" );
    std::string expected = "#CDEF11";
    std::string actual = toString( x );
    x.setAlpha( 101 );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 101, x.getAlpha() )
    CHECK_EQUAL( 205, x.getRed() )
    CHECK_EQUAL( 239, x.getGreen() )
    CHECK_EQUAL( 17, x.getBlue() )
    CHECK( Color::ColorType::ARGB == x.getColorType() );
}

TEST( Color08, Strings )
{
    Color x;
    x.parse( "#00CDEF11" );
    std::string expected = "#00CDEF11";
    std::string actual = toString( x );
    x.setAlpha( 300 );
    x.setRed( -1 );
    x.setGreen( 999 );
    x.setBlue( -2 );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 255, x.getAlpha() )
    CHECK_EQUAL( 0, x.getRed() )
    CHECK_EQUAL( 255, x.getGreen() )
    CHECK_EQUAL( 0, x.getBlue() )
    CHECK( Color::ColorType::ARGB == x.getColorType() );
}
TEST( Color09, Strings )
{
    Color x;
    x.parse( "#ABCDEF" );
    std::string expected = "#ABCDEF";
    std::string actual = toString( x );
    x.setAlpha( 300 );
    x.setRed( -1 );
    x.setGreen( 999 );
    x.setBlue( -2 );
    x.parse( "#FF FF ff FF " );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 255, x.getAlpha() )
    CHECK_EQUAL( 0, x.getRed() )
    CHECK_EQUAL( 255, x.getGreen() )
    CHECK_EQUAL( 0, x.getBlue() )
    CHECK( Color::ColorType::ARGB == x.getColorType() );
}

TEST( Color10, Strings )
{
    Color x;
    x.parse( "#ABCDEF99" );
    std::string expected = "#ABCDEF99";
    std::string actual = toString( x );
    x.setAlpha( -1 );
    x.setRed( -1 );
    x.setGreen( 999 );
    x.setBlue( -2 );
    x.parse( "#01FF03" );
    CHECK_EQUAL( expected, actual )
    CHECK_EQUAL( 255, x.getAlpha() )
    CHECK_EQUAL( 1, x.getRed() )
    CHECK_EQUAL( 255, x.getGreen() )
    CHECK_EQUAL( 3, x.getBlue() )
    CHECK( Color::ColorType::RGB == x.getColorType() );
    std::stringstream ss;
    toStream( ss, x );
    expected = "#01FF03";
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
    ss.str( "" );
    ss << x;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

#endif
