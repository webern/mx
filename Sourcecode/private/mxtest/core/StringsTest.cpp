// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Strings.h"
#include <sstream>

using namespace mx::core;

TEST( XsString01, Strings )
{
    XsString x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsString02, Strings )
{
    XsString x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsString03, Strings )
{
    XsString x( "Test" );
    x.setValue( "Different" );
    StringType expected = "Different";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsString04, Strings )
{
    XsString x( "Test" );
    x.setValue( "Think" );
    std::stringstream ss;
    toStream( ss, x );
    StringType expected = "Think";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XsString05, Strings )
{
    XsString x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XsString06, Strings )
{
    XsString x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( XsToken01, Strings )
{
    XsToken x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsToken02, Strings )
{
    XsToken x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsToken03, Strings )
{
    XsToken x( "\n\t  Test \r \t \n String  \t" );
    StringType expected = "Test String";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsNMToken01, Strings )
{
    XsNMToken x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsNMToken02, Strings )
{
    XsNMToken x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsNMToken03, Strings )
{
    XsNMToken x( "\n\t  Test \r \t \n String  \t" );
    StringType expected = "Test String";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST( XsID01, Strings )
{
    XsID x( "$%#1" );
    StringType expected = "___1";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsID02, Strings )
{
    XsID x( "" );
    StringType expected = "ID";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsID03, Strings )
{
    XsID x( "123" );
    StringType expected = "ID123";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsID04, Strings )
{
    XsID x( "ABC045" );
    StringType expected = "ABC045";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST( XsID05, Strings )
{
    XsID x( "X 21" );
    StringType expected = "X_21";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST(  XsIDREF01, Strings )
{
     XsIDREF x( "$%#1" );
    StringType expected = "___1";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST(  XsIDREF02, Strings )
{
     XsIDREF x( "" );
    StringType expected = "ID";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST(  XsIDREF03, Strings )
{
     XsIDREF x( "123" );
    StringType expected = "ID123";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST(  XsIDREF04, Strings )
{
     XsIDREF x( "ABC045" );
    StringType expected = "ABC045";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST(  XsIDREF05, Strings )
{
     XsIDREF x( "X 21" );
    StringType expected = "X_21";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST( XmlLang01, Strings )
{
    XmlLang x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XmlLang02, Strings )
{
    XmlLang x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XmlLang03, Strings )
{
    XmlLang x( "Test" );
    x.setValue( "Different" );
    StringType expected = "Different";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XmlLang04, Strings )
{
    XmlLang x( "Test" );
    x.setValue( "Think" );
    std::stringstream ss;
    toStream( ss, x );
    StringType expected = "Think";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XmlLang05, Strings )
{
    XmlLang x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XmlLang06, Strings )
{
    XmlLang x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual )
}



TEST( XsAnyUri01, Strings )
{
    XsAnyUri x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsAnyUri02, Strings )
{
    XsAnyUri x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsAnyUri03, Strings )
{
    XsAnyUri x( "Test" );
    x.setValue( "Different" );
    StringType expected = "Different";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XsAnyUri04, Strings )
{
    XsAnyUri x( "Test" );
    x.setValue( "Think" );
    std::stringstream ss;
    toStream( ss, x );
    StringType expected = "Think";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XsAnyUri05, Strings )
{
    XsAnyUri x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = ss.str();
    CHECK_EQUAL( expected, actual )
}
TEST( XsAnyUri06, Strings )
{
    XsAnyUri x( "Test" );
    x.setValue( "Hello" );
    std::stringstream ss;
    ss << x;
    StringType expected = "Hello";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual )
}

TEST( XlinkRole01, Strings )
{
    XlinkRole x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XlinkRole02, Strings )
{
    XlinkRole x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XlinkRole03, Strings )
{
    XlinkRole x( "\n\t  Test \r \t \n String  \t" );
    StringType expected = "Test String";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST( XlinkTitle01, Strings )
{
    XlinkTitle x;
    StringType expected = "";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XlinkTitle02, Strings )
{
    XlinkTitle x( "Test" );
    StringType expected = "Test";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}
TEST( XlinkTitle03, Strings )
{
    XlinkTitle x( "\n\t  Test \r \t \n String  \t" );
    StringType expected = "Test String";
    StringType actual = x.getValue();
    CHECK_EQUAL( expected, actual )
}

TEST( CommaSeparatedText01, Strings )
{
    CommaSeparatedText x( " Hello , \nWorld$#&" );
    StringType expected = "Hello,World$#&amp;";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual )
}
TEST( CommaSeparatedText02, Strings )
{
    CommaSeparatedText x;
    x.parse( " One, two, Three,     \nFOUR" );
    auto it = x.getValues().cbegin();
    
    StringType expected = "One";
    StringType actual = toString( *it );
    CHECK_EQUAL( expected, actual )
    
    ++it;
    expected = "two";
    actual = toString( *it );
    CHECK_EQUAL( expected, actual )
    
    ++it;
    expected = "Three";
    actual = toString( *it );
    CHECK_EQUAL( expected, actual )
    
    ++it;
    expected = "FOUR";
    actual = toString( *it );
    CHECK_EQUAL( expected, actual )
    
    expected = "One,two,Three,FOUR";
    actual = toString( x );
    CHECK_EQUAL( expected, actual )
}

TEST( CommaSeparatedText03, Strings )
{
    CommaSeparatedText x;
    x.parse( " One, two, Three,     \nFOUR" );
    CHECK( x.getValuesBegin() != x.getValuesEnd() );
    CHECK( x.getValuesBeginConst() != x.getValuesEndConst() );
    
    StringType expected = "One";
    StringType actual = toString((*x.getValuesBegin()));
    CHECK_EQUAL( expected, actual )
    
    expected = "two";
    actual = toString((*(++x.getValuesBeginConst())));
    CHECK_EQUAL( expected, actual )
    
    auto it = ++(x.getValuesBegin());
    *it = XsToken( " Bones  &  Bish " );
    expected = "Bones & Bish";
    actual = toString((*(++x.getValuesBeginConst())));
    CHECK_EQUAL( expected, actual )
    
}

TEST( EndingNumber01, Strings )
{
    EndingNumber x;
    CHECK( x.getValuesBegin() == x.getValuesEnd() )
    CHECK_EQUAL( "", toString( x ) )
}
TEST( EndingNumber02, Strings )
{
    EndingNumber x( "-2,-1,,,,XYZ, 0,  @#$@*&#^1,2,3,3,3,3,3" );
    CHECK( x.getValuesBegin() != x.getValuesEnd() )
    CHECK( x.getValuesBeginConst() != x.getValuesEndConst() )
    StringType expected = "1,2,3";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual );
    CHECK_EQUAL( 1, *(x.getValues().begin()) )
    CHECK_EQUAL( 2, *( ++( x.getValuesBegin() ) ) )
    CHECK_EQUAL( 2, *( ++( x.getValuesBeginConst() ) ) )
    CHECK_EQUAL( 3, *( --( x.getValuesEnd() ) ) )
    CHECK_EQUAL( 3, *( --( x.getValuesEndConst() ) ) )
    std::stringstream ss;
    ss << x;
    expected = "1,2,3";
    actual = ss.str();
    CHECK_EQUAL( expected, actual );
    ss.str( "" );
    toStream( ss, x );
    expected = "1,2,3";
    actual = ss.str();
    CHECK_EQUAL( expected, actual );
}
TEST( TimeOnly01, Strings )
{
    TimeOnly x;
    CHECK( x.getValuesBegin() == x.getValuesEnd() )
    CHECK_EQUAL( "", toString( x ) )
}
TEST( TimeOnly02, Strings )
{
    TimeOnly x( "-2,-1,,,,XYZ, 0,  @#$@*&#^1,2,3,3,3,3,3" );
    CHECK( x.getValuesBegin() != x.getValuesEnd() )
    CHECK( x.getValuesBeginConst() != x.getValuesEndConst() )
    StringType expected = "1,2,3";
    StringType actual = toString( x );
    CHECK_EQUAL( expected, actual );
    CHECK_EQUAL( 1, *(x.getValues().begin()) )
    CHECK_EQUAL( 2, *( ++( x.getValuesBegin() ) ) )
    CHECK_EQUAL( 2, *( ++( x.getValuesBeginConst() ) ) )
    CHECK_EQUAL( 3, *( --( x.getValuesEnd() ) ) )
    CHECK_EQUAL( 3, *( --( x.getValuesEndConst() ) ) )
    std::stringstream ss;
    ss << x;
    expected = "1,2,3";
    actual = ss.str();
    CHECK_EQUAL( expected, actual );
    ss.str( "" );
    toStream( ss, x );
    expected = "1,2,3";
    actual = ss.str();
    CHECK_EQUAL( expected, actual );
}

#endif
