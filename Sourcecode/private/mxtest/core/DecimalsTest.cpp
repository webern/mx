// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_CORE_TESTS

    #include "cpul/testFramework.h"
    #include "mx/core/Decimals.h"
    #include <sstream>

using namespace mx::core;

DecimalType kThreshold = 0.00001;

// using DecimalType = double;
TEST( DecimalType, Decimals )
{
    DecimalType d = 0;
    CHECK_DOUBLES_EQUAL( 0, d, kThreshold )
}

// class Decimal
TEST( Decimal01, Decimals )
{
    Decimal d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Decimal02, Decimals )
{
    Decimal d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Decimal02b, Decimals )
{
    Decimal d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Decimal03, Decimals )
{
    Decimal d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Decimal04, Decimals )
{
    Decimal d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( Decimal05, Decimals )
{
    Decimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-435.249" );
    DecimalType expected = -435.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Decimal06, Decimals )
{
    Decimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// class DecimalRange : public Decimal
TEST( DecimalRange01, Decimals )
{
    DecimalRange d( -1, 1, 0 );
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DecimalRange02, Decimals )
{
    DecimalRange d( -1, 1, 1.234 );
    DecimalType expected = 1;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DecimalRange02b, Decimals )
{
    DecimalRange d( -1, 123456789013, 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DecimalRange03, Decimals )
{
    DecimalRange d( -100, 100, 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DecimalRange04, Decimals )
{
    DecimalRange d( -1, 1, 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( DecimalRange05, Decimals )
{
    DecimalRange d( -1, 1, 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-0.3164978546312" );
    DecimalType expected = -0.3164978546312;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DecimalRange06, Decimals )
{
    DecimalRange d( -1, 1, 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// class PositiveDecimal: public Decimal
TEST( PositiveDecimal01, Decimals )
{
    PositiveDecimal d;
    DecimalType expected = 1;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDecimal02, Decimals )
{
    PositiveDecimal d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDecimal02b, Decimals )
{
    PositiveDecimal d( -123456789012.123456789012 );
    DecimalType expected = NON_ZERO_AMOUNT;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDecimal03, Decimals )
{
    PositiveDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDecimal04, Decimals )
{
    PositiveDecimal d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( PositiveDecimal05, Decimals )
{
    PositiveDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-0.3164978546312" );
    DecimalType expected = NON_ZERO_AMOUNT;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDecimal06, Decimals )
{
    PositiveDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// class NonNegativeDecimal: public Decimal
TEST( NonNegativeDecimal01, Decimals )
{
    NonNegativeDecimal d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( NonNegativeDecimal02, Decimals )
{
    NonNegativeDecimal d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( NonNegativeDecimal02b, Decimals )
{
    NonNegativeDecimal d( -123456789012.123456789012 );
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( NonNegativeDecimal03, Decimals )
{
    NonNegativeDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( NonNegativeDecimal04, Decimals )
{
    NonNegativeDecimal d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( NonNegativeDecimal05, Decimals )
{
    NonNegativeDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-0.3164978546312" );
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( NonNegativeDecimal06, Decimals )
{
    NonNegativeDecimal d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// using DivisionsValue = Decimal;
TEST( DivisionsValue01, Decimals )
{
    DivisionsValue d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DivisionsValue02, Decimals )
{
    DivisionsValue d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DivisionsValue02b, Decimals )
{
    DivisionsValue d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DivisionsValue03, Decimals )
{
    DivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DivisionsValue04, Decimals )
{
    DivisionsValue d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( DivisionsValue05, Decimals )
{
    DivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-435.249" );
    DecimalType expected = -435.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( DivisionsValue06, Decimals )
{
    DivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// using MillimetersValue = Decimal;
TEST( MillimetersValue01, Decimals )
{
    MillimetersValue d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( MillimetersValue02, Decimals )
{
    MillimetersValue d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( MillimetersValue02b, Decimals )
{
    MillimetersValue d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( MillimetersValue03, Decimals )
{
    MillimetersValue d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( MillimetersValue04, Decimals )
{
    MillimetersValue d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( MillimetersValue05, Decimals )
{
    MillimetersValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-435.249" );
    DecimalType expected = -435.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( MillimetersValue06, Decimals )
{
    MillimetersValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

/* MIN = 0, MAX = 100, DEFAULT = 0 */
// class Percent : DecimalRange
TEST( Percent01, Decimals )
{
    Percent d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent02, Decimals )
{
    Percent d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent02b, Decimals )
{
    Percent d( 123456789012.123456789012 );
    DecimalType expected = 100;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent03, Decimals )
{
    Percent d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent04, Decimals )
{
    Percent d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( Percent05, Decimals )
{
    Percent d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "4.249" );
    DecimalType expected = 4.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent05b, Decimals )
{
    Percent d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-4.249" );
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Percent06, Decimals )
{
    Percent d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// using PositiveDivisionsValue = PositiveDecimal;
TEST( PositiveDivisionsValue01, Decimals )
{
    PositiveDivisionsValue d;
    DecimalType expected = 1;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDivisionsValue02, Decimals )
{
    PositiveDivisionsValue d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDivisionsValue02b, Decimals )
{
    PositiveDivisionsValue d( -123456789012.123456789012 );
    DecimalType expected = NON_ZERO_AMOUNT;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDivisionsValue03, Decimals )
{
    PositiveDivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDivisionsValue04, Decimals )
{
    PositiveDivisionsValue d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( PositiveDivisionsValue05, Decimals )
{
    PositiveDivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-0.3164978546312" );
    DecimalType expected = NON_ZERO_AMOUNT;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( PositiveDivisionsValue06, Decimals )
{
    PositiveDivisionsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

/* MIN = -180, MAX = 180, DEFAULT = 0 */
// class RotationDegrees : DecimalRange
TEST( RotationDegrees01, Decimals )
{
    RotationDegrees d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees02, Decimals )
{
    RotationDegrees d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees02b, Decimals )
{
    RotationDegrees d( 123456789012.123456789012 );
    DecimalType expected = 180;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees03, Decimals )
{
    RotationDegrees d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees04, Decimals )
{
    RotationDegrees d( 1.234 );
    d.setValue( -999 );
    std::string expected = "-180";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( RotationDegrees05, Decimals )
{
    RotationDegrees d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "4.249" );
    DecimalType expected = 4.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees05b, Decimals )
{
    RotationDegrees d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-466.249" );
    DecimalType expected = -180;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( RotationDegrees06, Decimals )
{
    RotationDegrees d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// using Semitones = Decimal;
TEST( Semitones01, Decimals )
{
    Semitones d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Semitones02, Decimals )
{
    Semitones d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Semitones02b, Decimals )
{
    Semitones d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Semitones03, Decimals )
{
    Semitones d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Semitones04, Decimals )
{
    Semitones d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( Semitones05, Decimals )
{
    Semitones d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-435.249" );
    DecimalType expected = -435.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( Semitones06, Decimals )
{
    Semitones d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

// using TenthsValue = Decimal;
TEST( TenthsValue01, Decimals )
{
    TenthsValue d;
    DecimalType expected = 0;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TenthsValue02, Decimals )
{
    TenthsValue d( 1.234 );
    DecimalType expected = 1.234;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TenthsValue02b, Decimals )
{
    TenthsValue d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TenthsValue03, Decimals )
{
    TenthsValue d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TenthsValue04, Decimals )
{
    TenthsValue d( 1.234 );
    d.setValue( 0.00000384109 );
    std::string expected = "0.000004";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( TenthsValue05, Decimals )
{
    TenthsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-435.249" );
    DecimalType expected = -435.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TenthsValue06, Decimals )
{
    TenthsValue d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 0.00000384;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

/* MIN = 2, MAX = N/A, DEFAULT = 2 */
// class TrillBeats: public Decimal
TEST( TrillBeats01, Decimals )
{
    TrillBeats d;
    DecimalType expected = 2;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats02, Decimals )
{
    TrillBeats d( 1.234 );
    DecimalType expected = 2;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats02b, Decimals )
{
    TrillBeats d( 123456789012.123456789012 );
    DecimalType expected = 123456789012.123456789012;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats03, Decimals )
{
    TrillBeats d( 1.234 );
    d.setValue( 0.00000384 );
    DecimalType expected = 2;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats04, Decimals )
{
    TrillBeats d( 1.234 );
    d.setValue( -999 );
    std::string expected = "2";
    std::string actual = toString( d, 11 );
    CHECK_EQUAL( expected, actual )

    std::stringstream ss;
    toStream( ss, d, 11 );
    actual = ss.str();
    CHECK_EQUAL( expected, actual )

    ss.str( "" );
    ss << d;
    actual = ss.str();
    CHECK_EQUAL( expected, actual )
}

TEST( TrillBeats05, Decimals )
{
    TrillBeats d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "4.249" );
    DecimalType expected = 4.249;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats05b, Decimals )
{
    TrillBeats d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "-466.249" );
    DecimalType expected = 2;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

TEST( TrillBeats06, Decimals )
{
    TrillBeats d( 1.234 );
    d.setValue( 0.00000384 );
    d.parse( "- 435.249" );
    DecimalType expected = 2;
    DecimalType actual = d.getValue();
    CHECK_DOUBLES_EQUAL( expected, actual, kThreshold )
}

#endif
