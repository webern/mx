// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/FontSize.h"
#include <sstream>

using namespace mx::core;

TEST( FontSize01, FontSize )
{
    FontSize x;
    std::string expected = "medium";
    CHECK( x.getIsCssFontSize() )
    CHECK( !x.getIsNumber() )
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( FontSize02, FontSize )
{
    FontSize x( Decimal( 31.2 ) );
    CHECK( !x.getIsCssFontSize() )
    CHECK( x.getIsNumber() )
    std::string expected = "31.2";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( FontSize03, FontSize )
{
    FontSize x( CssFontSize::xSmall );
    CHECK( x.getIsCssFontSize() )
    CHECK( !x.getIsNumber() )
    std::string expected = "x-small";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( FontSize04, FontSize )
{
    FontSize x( "x-large" );
    CHECK( x.getIsCssFontSize() )
    CHECK( !x.getIsNumber() )
    CssFontSize expected = CssFontSize::xLarge;
    CssFontSize actual = x.getValueCssFontSize();
    CHECK_EQUAL( expected, actual )
}
TEST( FontSize05, FontSize )
{
    FontSize x( "24.0" );
    CHECK( ! x.getIsCssFontSize() )
    CHECK( x.getIsNumber() )
    DecimalType expected = 24;
    DecimalType actual = x.getValueNumber().getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kDefaultPrecision )
}
TEST( FontSize06, FontSize )
{
    FontSize x( "24.0" );
    x.setValue( CssFontSize::xxLarge );
    CHECK( x.getIsCssFontSize() )
    CHECK( ! x.getIsNumber() )
    std::string expected = "xx-large";
    std::stringstream ss;
    ss << x;
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( FontSize07, FontSize )
{
    FontSize x( "xx-small" );
    x.setValue( Decimal( 30.9 ) );
    CHECK( ! x.getIsCssFontSize() )
    CHECK( x.getIsNumber() )
    std::string expected = "30.9";
    std::stringstream ss;
    toStream( ss, x );
    std::string actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

#endif
