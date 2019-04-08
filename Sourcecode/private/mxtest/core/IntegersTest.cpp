// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Integers.h"

using namespace mx::core;

TEST( Int01, Integers )
{
    Integer i( 5 );
    IntType expected = 5;
    IntType actual = i.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Int02, Integers )
{
    Integer i;
    IntType expected = 0;
    IntType actual = i.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Int03, Integers )
{
    Integer i;
    i.setValue( 51 );
    IntType expected = 51;
    IntType actual = i.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Int04, Integers )
{
    Integer i( 1 );
    i.parse( "32" );
    IntType expected = 32;
    IntType actual = i.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Int05, Integers )
{
    Integer i( 72 );
    i.parse( "xxx" );
    IntType expected = 72;
    IntType actual = i.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( Int06, Integers )
{
    Integer i( 124 );
    std::stringstream ss1;
    ss1 << i;
    std::string expected = "124";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( Int07, Integers )
{
    Integer i( 124 );
    std::stringstream ss1;
    toStream( ss1, i );
    std::string expected = "124";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( Int08, Integers )
{
    Integer i( 124 );
    std::stringstream ss1;
    std::string expected = "124";
    std::string actual = toString( i );
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange01, Integers )
{
    IntRange x( 1, 6, 0 );
    IntType expected = 1;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange02, Integers )
{
    IntRange x( 21, 25, 100 );
    IntType expected = 25;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange03, Integers )
{
    IntRange x( -100, -50, 100 );
    x.parse( "-90" );
    IntType expected = -90;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange04, Integers )
{
    IntRange x( -1, -3, 100 );
    x.parse( "-2" );
    IntType expected = -1;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange05, Integers )
{
    IntRange x( -15, 15, 10 );
    x.parse( "-%sf" );
    IntType expected = 10;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange06, Integers )
{
    IntRange x( -15, 15, 10 );
    std::stringstream ss1;
    ss1 << x;
    std::string expected = "10";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange07, Integers )
{
    IntRange x( -105, -15, 10 );
    std::stringstream ss1;
    toStream( ss1, x );
    std::string expected = "-15";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( IntRange08, Integers )
{
    IntRange x( -105, -15, 10 );
    std::stringstream ss1;
    std::string expected = "-15";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger01, Integers )
{
    PositiveInteger x( 10 );
    IntType expected = 10;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger02, Integers )
{
    PositiveInteger x( 0 );
    IntType expected = 1;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger03, Integers )
{
    PositiveInteger x( 0 );
    x.parse( "21" );
    IntType expected = 21;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger04, Integers )
{
    PositiveInteger x( 33 );
    x.parse( "2 2" );
    IntType expected = 33;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger05, Integers )
{
    PositiveInteger x( 10 );
    std::stringstream ss1;
    ss1 << x;
    std::string expected = "10";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger06, Integers )
{
    PositiveInteger x( 12 );
    std::stringstream ss1;
    toStream( ss1, x );
    std::string expected = "12";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( PositiveInteger07, Integers )
{
    PositiveInteger x( 16385000 );
    std::stringstream ss1;
    std::string expected = "16385000";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger01, Integers )
{
    NonNegativeInteger x( 10 );
    IntType expected = 10;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger02, Integers )
{
    NonNegativeInteger x( -1 );
    IntType expected = 0;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger03, Integers )
{
    NonNegativeInteger x( 0 );
    x.parse( "21" );
    IntType expected = 21;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger04, Integers )
{
    NonNegativeInteger x( 33 );
    x.parse( "2 2" );
    IntType expected = 33;
    IntType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger05, Integers )
{
    NonNegativeInteger x( 10 );
    std::stringstream ss1;
    ss1 << x;
    std::string expected = "10";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger06, Integers )
{
    NonNegativeInteger x( 12 );
    std::stringstream ss1;
    toStream( ss1, x );
    std::string expected = "12";
    std::string actual = ss1.str();
    CHECK_EQUAL( expected, actual )
}
TEST( NonNegativeInteger07, Integers )
{
    NonNegativeInteger x( 16385000 );
    std::stringstream ss1;
    std::string expected = "16385000";
    std::string actual = toString( x );
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = 3, DEFAULT = 1 */
//class AccordionMiddleValue : public IntRange
TEST( AccordionMiddleValue, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    AccordionMiddleValue x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    AccordionMiddleValue y( 1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    AccordionMiddleValue z( 4 );
    expected = 3;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "3";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}


/* MIN = 1, MAX = 8, DEFAULT = 1 */
//class BeamLevel : public IntRange
TEST( BeamLevel, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    BeamLevel x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    BeamLevel y( 0 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    BeamLevel z( 9 );
    expected = 8;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "8";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = N/A, MAX = N/A, DEFAULT = 0 */
//class FifthsValue : public Int
TEST( FifthsValue, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    FifthsValue x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    FifthsValue y( -1 );
    expected = -1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    FifthsValue z( 9 );
    expected = 9;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "9";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = 16, DEFAULT = 1 */
//class Midi16 : public IntRange
TEST( Midi16, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    Midi16 x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    Midi16 y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    Midi16 z( 17 );
    expected = 16;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "16";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = 128, DEFAULT = 1 */
//class Midi128 : public IntRange
TEST( Midi128, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    Midi128 x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    Midi128 y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    Midi128 z( 129 );
    expected = 128;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "128";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = 16384, DEFAULT = 1 */
//class Midi16384 : public IntRange
TEST( Midi16384, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    Midi16384 x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    Midi16384 y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    Midi16384 z( 16385 );
    expected = 16384;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "16384";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = 6, DEFAULT = 1 */
//class NumberLevel : public IntRange
TEST( NumberLevel, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    NumberLevel x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    NumberLevel y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    NumberLevel z( 16385 );
    expected = 6;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "6";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 0, MAX = 3, DEFAULT = 0 */
//class NumberOfLines : public IntRange
TEST( NumberOfLines, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    NumberOfLines x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    NumberOfLines y( -1 );
    expected = 0;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    NumberOfLines z( 16385 );
    expected = 3;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "3";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 0, MAX = 9, DEFAULT = 0 */
//class OctaveValue : public IntRange
TEST( OctaveValue, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    OctaveValue x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    OctaveValue y( -1 );
    expected = 0;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    OctaveValue z( 16385 );
    expected = 9;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "9";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = N/A, MAX = N/A, DEFAULT = 0 */
//class StaffLine : public Int
TEST( StaffLine, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    StaffLine x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    StaffLine y( -1 );
    expected = -1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    StaffLine z( 16385000 );
    expected = 16385000;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "16385000";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = N/A, DEFAULT = 1 */
//using StaffNumber = PositiveInteger;
TEST( StaffNumber, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    StaffNumber x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    StaffNumber y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    StaffNumber z( 16385000 );
    expected = 16385000;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "16385000";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 1, MAX = N/A, DEFAULT = 1 */
//using StringNumber = PositiveInteger;
TEST( StringNumber, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    StringNumber x;
    expected = 1;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    StringNumber y( -1 );
    expected = 1;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    StringNumber z( 16385000 );
    expected = 16385000;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    std::string expected_str = "16385000";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

/* MIN = 0, MAX = 8, DEFAULT = 0 */
//class TremoloMarks : public IntRange
TEST( TremoloMarks, Integers )
{
    IntType expected = 0;
    IntType actual = 0;
    
    TremoloMarks x;
    expected = 0;
    actual = x.getValue();
    CHECK_EQUAL( expected, actual )
    
    TremoloMarks y( -1 );
    expected = 0;
    actual = y.getValue();
    CHECK_EQUAL( expected, actual )
    
    TremoloMarks z( 16385000 );
    expected = 8;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
    
    std::string expected_str = "8";
    std::string actual_str = toString( z );
    CHECK_EQUAL( expected_str, actual_str )
    
    z.parse( "2" );
    expected = 2;
    actual = z.getValue();
    CHECK_EQUAL( expected, actual )
}

#endif
