// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/PositiveIntegerOrEmpty.h"
#include <sstream>

using namespace mx::core;

TEST( PositiveIntegerOrEmpty01, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x;
    std::string expected = "";
    CHECK( x.getIsEmpty() )
    CHECK( !x.getIsNumber() )
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveIntegerOrEmpty02, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( PositiveInteger( 31 ) );
    CHECK( !x.getIsEmpty() )
    CHECK( x.getIsNumber() )
    std::string expected = "31";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveIntegerOrEmpty03, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( "" );
    CHECK( x.getIsEmpty() )
    CHECK( !x.getIsNumber() )
    std::string expected = "";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveIntegerOrEmpty04, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( "XXX" );
    CHECK( !x.getIsEmpty() )
    CHECK( x.getIsNumber() )
    std::string expected = "1";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveIntegerOrEmpty05, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( "24" );
    CHECK( ! x.getIsEmpty() )
    CHECK( x.getIsNumber() )
    IntType expected = 24;
    IntType actual = x.getValueNumber().getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveIntegerOrEmpty06, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( "24" );
    x.setValueEmpty();
    CHECK( x.getIsEmpty() )
    CHECK( ! x.getIsNumber() )
    std::string expected = "";
    std::stringstream ss;
    ss << x;
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( PositiveIntegerOrEmpty07, PositiveIntegerOrEmpty )
{
    PositiveIntegerOrEmpty x( "" );
    x.setValue( PositiveInteger( 30 ) );
    CHECK( ! x.getIsEmpty() )
    CHECK( x.getIsNumber() )
    std::string expected = "30";
    std::stringstream ss;
    toStream( ss, x );
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

#endif
