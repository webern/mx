#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/Date.h"
#include <string>

using namespace mx::core;

TEST( toString01, Date )
{
    Date d;
    CHECK_EQUAL( d.getYear(), 1900 );
    CHECK_EQUAL( d.getMonth(), 1 );
    CHECK_EQUAL( d.getDay(), 1 );
    std::string expected = "1900-01-01";
    std::string actual = toString( d );
    CHECK_EQUAL( expected, actual )
}

TEST( toString02, Date )
{
    int year = 300;
    int month = 8;
    int day = 1;
    Date d ( year, month, day );
    CHECK_EQUAL( d.getYear(), year );
    CHECK_EQUAL( d.getMonth(), month );
    CHECK_EQUAL( d.getDay(), day );
    std::string expected = "0300-08-01";
    std::string actual = toString( d );
    CHECK_EQUAL( expected, actual )
}

TEST( toString03, Date )
{
    int year = 1500;
    int month = 12;
    int day = 10;
    Date d ( year, month, day );
    CHECK_EQUAL( d.getYear(), year );
    CHECK_EQUAL( d.getMonth(), month );
    CHECK_EQUAL( d.getDay(), day );
    std::string expected = "1500-12-10";
    std::string actual = toString( d );
    CHECK_EQUAL( expected, actual )
}

TEST(dateCompare_equals01, Date)
{
    Date d1( 2014, 6, 7 );
    Date d2( 2014, 6, 7 );
    bool expected = true;
    bool actual = d1 == d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_equals02, Date)
{
    Date d1( 1023, 12, 31 );
    Date d2( 1003, 12, 31 );
    bool expected = false;
    bool actual = d1 == d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_equals03, Date)
{
    Date d1( 3001, 4, 17 );
    Date d2( 3001, 10, 17 );
    bool expected = false;
    bool actual = d1 == d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_equals04, Date)
{
    Date d1( 1893, 12, 30 );
    Date d2( 1893, 12, 31 );
    bool expected = false;
    bool actual = d1 == d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_equals05, Date)
{
    Date d1( 9875, 3, 5 );
    Date d2( 9875, 3, 5 );
    bool expected = true;
    bool actual = d1 == d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_notequals01, Date)
{
    Date d1( 2014, 6, 7 );
    Date d2( 2014, 6, 7 );
    bool expected = false;
    bool actual = d1 != d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_notequals02, Date)
{
    Date d1( 1023, 12, 31 );
    Date d2( 1003, 12, 31 );
    bool expected = true;
    bool actual = d1 != d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_notequals03, Date)
{
    Date d1( 3001, 4, 17 );
    Date d2( 3001, 10, 17 );
    bool expected = true;
    bool actual = d1 != d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_notequals04, Date)
{
    Date d1( 1893, 12, 30 );
    Date d2( 1893, 12, 31 );
    bool expected = true;
    bool actual = d1 != d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_notequals05, Date)
{
    Date d1( 9875, 3, 5 );
    Date d2( 9875, 3, 5 );
    bool expected = false;
    bool actual = d1 != d2;
    CHECK_EQUAL( expected, actual )
}

TEST(dateCompare_greater_true_day, Date)
{
    Date d1( 1111, 1, 2 );
    Date d2( 1111, 1, 1 );
    bool expected = true;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_true_month, Date)
{
    Date d1( 2222, 4, 10 );
    Date d2( 2222, 3, 10 );
    bool expected = true;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_true_year, Date)
{
    Date d1( 1753, 12, 31 );
    Date d2( 1752, 12, 31 );
    bool expected = true;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_false_day, Date)
{
    Date d1( 7382, 2, 1 );
    Date d2( 7382, 2, 31 );
    bool expected = false;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_false_month, Date)
{
    Date d1( 2014, 1, 13 );
    Date d2( 2014, 12, 13 );
    bool expected = false;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_false_year, Date)
{
    Date d1( 1492, 12, 31 );
    Date d2( 1493, 1, 1 );
    bool expected = false;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_greater_falseEq_day, Date)
{
    Date d1( 1320, 2, 29 );
    Date d2( 1320, 2, 29 );
    bool expected = false;
    bool actual = d1 > d2;
    CHECK_EQUAL( expected, actual )
}


TEST(dateCompare_less_true_day, Date)
{
    Date d1( 1111, 1, 2 );
    Date d2( 1111, 1, 1 );
    bool expected = false;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_true_month, Date)
{
    Date d1( 2222, 4, 10 );
    Date d2( 2222, 3, 10 );
    bool expected = false;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_true_year, Date)
{
    Date d1( 1753, 12, 31 );
    Date d2( 1752, 12, 31 );
    bool expected = false;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_false_day, Date)
{
    Date d1( 7382, 2, 1 );
    Date d2( 7382, 2, 31 );
    bool expected = true;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_false_month, Date)
{
    Date d1( 2014, 1, 13 );
    Date d2( 2014, 12, 13 );
    bool expected = true;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_false_year, Date)
{
    Date d1( 1492, 12, 31 );
    Date d2( 1493, 1, 1 );
    bool expected = true;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateCompare_less_falseEq_day, Date)
{
    Date d1( 1320, 2, 29 );
    Date d2( 1320, 2, 29 );
    bool expected = false;
    bool actual = d1 < d2;
    CHECK_EQUAL( expected, actual )
}


TEST(dateComare_EqOr_greater_true_day, Date)
{
    Date d1( 1111, 1, 2 );
    Date d2( 1111, 1, 1 );
    bool expected = true;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_true_month, Date)
{
    Date d1( 2222, 4, 10 );
    Date d2( 2222, 3, 10 );
    bool expected = true;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_true_year, Date)
{
    Date d1( 1753, 12, 31 );
    Date d2( 1752, 12, 31 );
    bool expected = true;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_false_day, Date)
{
    Date d1( 7382, 2, 1 );
    Date d2( 7382, 2, 31 );
    bool expected = false;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_false_month, Date)
{
    Date d1( 2014, 1, 13 );
    Date d2( 2014, 12, 13 );
    bool expected = false;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_false_year, Date)
{
    Date d1( 1492, 12, 31 );
    Date d2( 1493, 1, 1 );
    bool expected = false;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_greater_trueEq_day, Date)
{
    Date d1( 1320, 2, 29 );
    Date d2( 1320, 2, 29 );
    bool expected = true;
    bool actual = d1 >= d2;
    CHECK_EQUAL( expected, actual )
}


TEST(dateComare_EqOr_less_true_day, Date)
{
    Date d1( 1111, 1, 2 );
    Date d2( 1111, 1, 1 );
    bool expected = false;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_true_month, Date)
{
    Date d1( 2222, 4, 10 );
    Date d2( 2222, 3, 10 );
    bool expected = false;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_true_year, Date)
{
    Date d1( 1753, 12, 31 );
    Date d2( 1752, 12, 31 );
    bool expected = false;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_false_day, Date)
{
    Date d1( 7382, 2, 1 );
    Date d2( 7382, 2, 31 );
    bool expected = true;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_false_month, Date)
{
    Date d1( 2014, 1, 13 );
    Date d2( 2014, 12, 13 );
    bool expected = true;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_false_year, Date)
{
    Date d1( 1492, 12, 31 );
    Date d2( 1493, 1, 1 );
    bool expected = true;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}
TEST(dateComare_EqOr_less_trueEq_day, Date)
{
    Date d1( 1320, 2, 29 );
    Date d2( 1320, 2, 29 );
    bool expected = true;
    bool actual = d1 <= d2;
    CHECK_EQUAL( expected, actual )
}



TEST(weekday_20140607, Date)
{
    Date d( 2014, 6, 7 );
    int expected = 7;
    int actual = d.getWeekday();
    CHECK_EQUAL( expected, actual )
}

TEST(weekday_22530714, Date)
{
    Date d1( 2253, 7, 14 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}

TEST(weekday_11740516, Date)
{
    Date d1( 1174, 5, 16 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_58290910, Date)
{
    Date d1( 5829, 9, 10 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_82361023, Date)
{
    Date d1( 8236, 10, 23 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_88730204, Date)
{
    Date d1( 8873, 2, 4 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_72140914, Date)
{
    Date d1( 7214, 9, 14 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_90790927, Date)
{
    Date d1( 9079, 9, 27 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_65730115, Date)
{
    Date d1( 6573, 1, 15 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_84520920, Date)
{
    Date d1( 8452, 9, 20 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_98990217, Date)
{
    Date d1( 9899, 2, 17 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_17720401, Date)
{
    Date d1( 1772, 4, 1 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_38400221, Date)
{
    Date d1( 3840, 2, 21 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_12550615, Date)
{
    Date d1( 1255, 6, 15 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_64750228, Date)
{
    Date d1( 6475, 2, 28 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_05771013, Date)
{
    Date d1( 577, 10, 13 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_92430918, Date)
{
    Date d1( 9243, 9, 18 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_74520911, Date)
{
    Date d1( 7452, 9, 11 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_09360207, Date)
{
    Date d1( 936, 2, 7 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_98970309, Date)
{
    Date d1( 9897, 3, 9 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_71290818, Date)
{
    Date d1( 7129, 8, 18 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_95591029, Date)
{
    Date d1( 9559, 10, 29 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_07910904, Date)
{
    Date d1( 791, 9, 4 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_08780728, Date)
{
    Date d1( 878, 7, 28 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_95381002, Date)
{
    Date d1( 9538, 10, 2 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_36320721, Date)
{
    Date d1( 3632, 7, 21 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_11640419, Date)
{
    Date d1( 1164, 4, 19 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_48281128, Date)
{
    Date d1( 4828, 11, 28 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_53531212, Date)
{
    Date d1( 5353, 12, 12 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_29060210, Date)
{
    Date d1( 2906, 2, 10 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_50210927, Date)
{
    Date d1( 5021, 9, 27 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_90011225, Date)
{
    Date d1( 9001, 12, 25 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_86790220, Date)
{
    Date d1( 8679, 2, 20 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_31790909, Date)
{
    Date d1( 3179, 9, 9 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_85330923, Date)
{
    Date d1( 8533, 9, 23 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_37770109, Date)
{
    Date d1( 3777, 1, 9 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_00701202, Date)
{
    Date d1( 70, 12, 2 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_92550605, Date)
{
    Date d1( 9255, 6, 5 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_78840719, Date)
{
    Date d1( 7884, 7, 19 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_48370828, Date)
{
    Date d1( 4837, 8, 28 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_62770906, Date)
{
    Date d1( 6277, 9, 6 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_80220810, Date)
{
    Date d1( 8022, 8, 10 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_60130424, Date)
{
    Date d1( 6013, 4, 24 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_54420904, Date)
{
    Date d1( 5442, 9, 4 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_54781128, Date)
{
    Date d1( 5478, 11, 28 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_24950505, Date)
{
    Date d1( 2495, 5, 5 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_60960325, Date)
{
    Date d1( 6096, 3, 25 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_69910726, Date)
{
    Date d1( 6991, 7, 26 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_28660611, Date)
{
    Date d1( 2866, 6, 11 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_02860713, Date)
{
    Date d1( 286, 7, 13 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_14480512, Date)
{
    Date d1( 1448, 5, 12 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_65670407, Date)
{
    Date d1( 6567, 4, 7 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}

TEST(weekday_19810219, Date)
{
    Date d1( 1981, 2, 19 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19931116, Date)
{
    Date d1( 1993, 11, 16 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20250620, Date)
{
    Date d1( 2025, 6, 20 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20230318, Date)
{
    Date d1( 2023, 3, 18 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20471013, Date)
{
    Date d1( 2047, 10, 13 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20730103, Date)
{
    Date d1( 2073, 1, 3 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20390117, Date)
{
    Date d1( 2039, 1, 17 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20440513, Date)
{
    Date d1( 2044, 5, 13 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19841123, Date)
{
    Date d1( 1984, 11, 23 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20250912, Date)
{
    Date d1( 2025, 9, 12 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20480924, Date)
{
    Date d1( 2048, 9, 24 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20280603, Date)
{
    Date d1( 2028, 6, 3 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20620514, Date)
{
    Date d1( 2062, 5, 14 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20010206, Date)
{
    Date d1( 2001, 2, 6 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20770114, Date)
{
    Date d1( 2077, 1, 14 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20361004, Date)
{
    Date d1( 2036, 10, 4 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20400304, Date)
{
    Date d1( 2040, 3, 4 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20530805, Date)
{
    Date d1( 2053, 8, 5 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20320530, Date)
{
    Date d1( 2032, 5, 30 );
    int expected = 1;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20660817, Date)
{
    Date d1( 2066, 8, 17 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20220219, Date)
{
    Date d1( 2022, 2, 19 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20320410, Date)
{
    Date d1( 2032, 4, 10 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20390209, Date)
{
    Date d1( 2039, 2, 9 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20320628, Date)
{
    Date d1( 2032, 6, 28 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19770914, Date)
{
    Date d1( 1977, 9, 14 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20680228, Date)
{
    Date d1( 2068, 2, 28 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19810428, Date)
{
    Date d1( 1981, 4, 28 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20611231, Date)
{
    Date d1( 2061, 12, 31 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20230408, Date)
{
    Date d1( 2023, 4, 8 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20290505, Date)
{
    Date d1( 2029, 5, 5 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20020622, Date)
{
    Date d1( 2002, 6, 22 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20610730, Date)
{
    Date d1( 2061, 7, 30 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20130624, Date)
{
    Date d1( 2013, 6, 24 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20640811, Date)
{
    Date d1( 2064, 8, 11 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20691120, Date)
{
    Date d1( 2069, 11, 20 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20220506, Date)
{
    Date d1( 2022, 5, 6 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20080111, Date)
{
    Date d1( 2008, 1, 11 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20371217, Date)
{
    Date d1( 2037, 12, 17 );
    int expected = 5;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20571005, Date)
{
    Date d1( 2057, 10, 5 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20380118, Date)
{
    Date d1( 2038, 1, 18 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20640321, Date)
{
    Date d1( 2064, 3, 21 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20610404, Date)
{
    Date d1( 2061, 4, 4 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19930713, Date)
{
    Date d1( 1993, 7, 13 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20010824, Date)
{
    Date d1( 2001, 8, 24 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20040730, Date)
{
    Date d1( 2004, 7, 30 );
    int expected = 6;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_20030913, Date)
{
    Date d1( 2003, 9, 13 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19880518, Date)
{
    Date d1( 1988, 5, 18 );
    int expected = 4;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19980829, Date)
{
    Date d1( 1998, 8, 29 );
    int expected = 7;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19770207, Date)
{
    Date d1( 1977, 2, 7 );
    int expected = 2;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}
TEST(weekday_19831227, Date)
{
    Date d1( 1983, 12, 27 );
    int expected = 3;
    int actual = d1.getWeekday();
    CHECK_EQUAL( expected, actual )
}



TEST(Ctor_out_of_Range001, Date)
{
    Date d( -1, 2, 3 );
    int expected = 1;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 3;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range002, Date)
{
    Date d( 2014, 0, 13 );
    int expected = 2014;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 13;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range003, Date)
{
    Date d( 2018, 6, -15 );
    int expected = 2018;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 6;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range004, Date)
{
    Date d( 10000, 12, 15 );
    int expected = 9999;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 12;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 15;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range005, Date)
{
    Date d( 1988, 32, 31 );
    int expected = 1988;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 12;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 31;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range006, Date)
{
    Date d( 1812, 2, 64 );
    int expected = 1812;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29; // leap year
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_out_of_Range007, Date)
{
    Date d( 1905, 2, 29 );
    int expected = 1905;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 28; // non leap year
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor_in_Range001, Date)
{
    Date d( 1, 1, 1 );
    int expected = 1;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}


TEST(Ctor00680229, Date)
{
    Date d( 68, 2, 29 );
    int expected = 68;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00720229, Date)
{
    Date d( 72, 2, 29 );
    int expected = 72;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00760229, Date)
{
    Date d( 76, 2, 29 );
    int expected = 76;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00800229, Date)
{
    Date d( 80, 2, 29 );
    int expected = 80;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00840229, Date)
{
    Date d( 84, 2, 29 );
    int expected = 84;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00880229, Date)
{
    Date d( 88, 2, 29 );
    int expected = 88;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00920229, Date)
{
    Date d( 92, 2, 29 );
    int expected = 92;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor00960229, Date)
{
    Date d( 96, 2, 29 );
    int expected = 96;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}


TEST(Ctor54440229, Date)
{
    Date d( 5444, 2, 29 );
    int expected = 5444;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor87960229, Date)
{
    Date d( 8796, 2, 29 );
    int expected = 8796;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor39760229, Date)
{
    Date d( 3976, 2, 29 );
    int expected = 3976;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor51280229, Date)
{
    Date d( 5128, 2, 29 );
    int expected = 5128;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor35320229, Date)
{
    Date d( 3532, 2, 29 );
    int expected = 3532;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor68440229, Date)
{
    Date d( 6844, 2, 29 );
    int expected = 6844;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor93080229, Date)
{
    Date d( 9308, 2, 29 );
    int expected = 9308;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor35360229, Date)
{
    Date d( 3536, 2, 29 );
    int expected = 3536;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor91720229, Date)
{
    Date d( 9172, 2, 29 );
    int expected = 9172;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor28200229, Date)
{
    Date d( 2820, 2, 29 );
    int expected = 2820;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor99560229, Date)
{
    Date d( 9956, 2, 29 );
    int expected = 9956;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor61920229, Date)
{
    Date d( 6192, 2, 29 );
    int expected = 6192;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor82840229, Date)
{
    Date d( 8284, 2, 29 );
    int expected = 8284;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor31080229, Date)
{
    Date d( 3108, 2, 29 );
    int expected = 3108;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor87840229, Date)
{
    Date d( 8784, 2, 29 );
    int expected = 8784;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor88560229, Date)
{
    Date d( 8856, 2, 29 );
    int expected = 8856;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor94680229, Date)
{
    Date d( 9468, 2, 29 );
    int expected = 9468;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor73080229, Date)
{
    Date d( 7308, 2, 29 );
    int expected = 7308;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor21400229, Date)
{
    Date d( 2140, 2, 29 );
    int expected = 2140;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor26560229, Date)
{
    Date d( 2656, 2, 29 );
    int expected = 2656;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor56800229, Date)
{
    Date d( 5680, 2, 29 );
    int expected = 5680;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor37160229, Date)
{
    Date d( 3716, 2, 29 );
    int expected = 3716;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor99680229, Date)
{
    Date d( 9968, 2, 29 );
    int expected = 9968;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor61120229, Date)
{
    Date d( 6112, 2, 29 );
    int expected = 6112;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor53480229, Date)
{
    Date d( 5348, 2, 29 );
    int expected = 5348;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor40680229, Date)
{
    Date d( 4068, 2, 29 );
    int expected = 4068;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor31920229, Date)
{
    Date d( 3192, 2, 29 );
    int expected = 3192;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor88760229, Date)
{
    Date d( 8876, 2, 29 );
    int expected = 8876;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor64880229, Date)
{
    Date d( 6488, 2, 29 );
    int expected = 6488;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor08480229, Date)
{
    Date d( 848, 2, 29 );
    int expected = 848;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor66760229, Date)
{
    Date d( 6676, 2, 29 );
    int expected = 6676;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor76760229, Date)
{
    Date d( 7676, 2, 29 );
    int expected = 7676;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor54920229, Date)
{
    Date d( 5492, 2, 29 );
    int expected = 5492;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor67920229, Date)
{
    Date d( 6792, 2, 29 );
    int expected = 6792;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor40160229, Date)
{
    Date d( 4016, 2, 29 );
    int expected = 4016;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor03160229, Date)
{
    Date d( 316, 2, 29 );
    int expected = 316;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor58600229, Date)
{
    Date d( 5860, 2, 29 );
    int expected = 5860;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor83640229, Date)
{
    Date d( 8364, 2, 29 );
    int expected = 8364;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor32400229, Date)
{
    Date d( 3240, 2, 29 );
    int expected = 3240;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor11400229, Date)
{
    Date d( 1140, 2, 29 );
    int expected = 1140;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor16640229, Date)
{
    Date d( 1664, 2, 29 );
    int expected = 1664;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor64080229, Date)
{
    Date d( 6408, 2, 29 );
    int expected = 6408;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor13320229, Date)
{
    Date d( 1332, 2, 29 );
    int expected = 1332;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor32280229, Date)
{
    Date d( 3228, 2, 29 );
    int expected = 3228;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor50440229, Date)
{
    Date d( 5044, 2, 29 );
    int expected = 5044;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor59600229, Date)
{
    Date d( 5960, 2, 29 );
    int expected = 5960;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor39200229, Date)
{
    Date d( 3920, 2, 29 );
    int expected = 3920;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor90800229, Date)
{
    Date d( 9080, 2, 29 );
    int expected = 9080;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor88400229, Date)
{
    Date d( 8840, 2, 29 );
    int expected = 8840;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor52040229, Date)
{
    Date d( 5204, 2, 29 );
    int expected = 5204;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor85520229, Date)
{
    Date d( 8552, 2, 29 );
    int expected = 8552;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor82440229, Date)
{
    Date d( 8244, 2, 29 );
    int expected = 8244;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor11600229, Date)
{
    Date d( 1160, 2, 29 );
    int expected = 1160;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor19920229, Date)
{
    Date d( 1992, 2, 29 );
    int expected = 1992;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor56840229, Date)
{
    Date d( 5684, 2, 29 );
    int expected = 5684;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor57520229, Date)
{
    Date d( 5752, 2, 29 );
    int expected = 5752;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor83120229, Date)
{
    Date d( 8312, 2, 29 );
    int expected = 8312;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor28960229, Date)
{
    Date d( 2896, 2, 29 );
    int expected = 2896;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor45480229, Date)
{
    Date d( 4548, 2, 29 );
    int expected = 4548;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor75080229, Date)
{
    Date d( 7508, 2, 29 );
    int expected = 7508;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 2;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}


TEST(Ctor15300129, Date)
{
    Date d( 1530, 1, 29 );
    int expected = 1530;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 29;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor44990928, Date)
{
    Date d( 4499, 9, 28 );
    int expected = 4499;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 9;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 28;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor22151103, Date)
{
    Date d( 2215, 11, 3 );
    int expected = 2215;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 11;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 3;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor32530427, Date)
{
    Date d( 3253, 4, 27 );
    int expected = 3253;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 4;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 27;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor30580119, Date)
{
    Date d( 3058, 1, 19 );
    int expected = 3058;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 1;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 19;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}

TEST(Ctor96770614, Date)
{
    Date d( 9677, 6, 14 );
    int expected = 9677;
    int actual = d.getYear();
    CHECK_EQUAL( expected, actual )
    expected = 6;
    actual = d.getMonth();
    CHECK_EQUAL( expected, actual )
    expected = 14;
    actual = d.getDay();
    CHECK_EQUAL( expected, actual )
}



TEST(add_day_ceiling, Date)
{
    Date d1( 9999, 5, 7 );
    Date d2 = d1.addDays( 365 );
    Date expected( 9999, 12, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(add_day_floor, Date)
{
    Date d1( 1, 5, 7 );
    Date d2 = d1.addDays( -365 );
    Date expected( 1, 1, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(add_day_20140606_P00001, Date)
{
    Date d1( 2014, 6, 6 );
    Date d2 = d1.addDays( 1 );
    Date expected( 2014, 6, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(add_day_20140606_P00002, Date)
{
    Date d1( 2014, 6, 6 );
    Date d2 = d1.addDays( 2 );
    Date expected( 2014, 6, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(add_day_20081224_neg1, Date)
{
    Date d1( 2008, 12, 24 );
    Date d2 = d1.addDays( -1 );
    Date expected( 2008, 12, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}

TEST(add_day_55580621_1, Date)
{
    Date d1( 5558, 6, 21 );
    Date d2 = d1.addDays( 1 );
    Date expected( 5558, 6, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_55140420_neg1, Date)
{
    Date d1( 5514, 4, 20 );
    Date d2 = d1.addDays( -1 );
    Date expected( 5514, 4, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_94270310_3, Date)
{
    Date d1( 9427, 3, 10 );
    Date d2 = d1.addDays( 3 );
    Date expected( 9427, 3, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92890828_neg3, Date)
{
    Date d1( 9289, 8, 28 );
    Date d2 = d1.addDays( -3 );
    Date expected( 9289, 8, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60990319_5, Date)
{
    Date d1( 6099, 3, 19 );
    Date d2 = d1.addDays( 5 );
    Date expected( 6099, 3, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90520117_neg5, Date)
{
    Date d1( 9052, 1, 17 );
    Date d2 = d1.addDays( -5 );
    Date expected( 9052, 1, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66060714_7, Date)
{
    Date d1( 6606, 7, 14 );
    Date d2 = d1.addDays( 7 );
    Date expected( 6606, 7, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82020217_neg7, Date)
{
    Date d1( 8202, 2, 17 );
    Date d2 = d1.addDays( -7 );
    Date expected( 8202, 2, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76400318_9, Date)
{
    Date d1( 7640, 3, 18 );
    Date d2 = d1.addDays( 9 );
    Date expected( 7640, 3, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40800218_neg9, Date)
{
    Date d1( 4080, 2, 18 );
    Date d2 = d1.addDays( -9 );
    Date expected( 4080, 2, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74740617_11, Date)
{
    Date d1( 7474, 6, 17 );
    Date d2 = d1.addDays( 11 );
    Date expected( 7474, 6, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66200708_neg11, Date)
{
    Date d1( 6620, 7, 8 );
    Date d2 = d1.addDays( -11 );
    Date expected( 6620, 6, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36351105_13, Date)
{
    Date d1( 3635, 11, 5 );
    Date d2 = d1.addDays( 13 );
    Date expected( 3635, 11, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50110226_neg13, Date)
{
    Date d1( 5011, 2, 26 );
    Date d2 = d1.addDays( -13 );
    Date expected( 5011, 2, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95090903_15, Date)
{
    Date d1( 9509, 9, 3 );
    Date d2 = d1.addDays( 15 );
    Date expected( 9509, 9, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23290914_neg15, Date)
{
    Date d1( 2329, 9, 14 );
    Date d2 = d1.addDays( -15 );
    Date expected( 2329, 8, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92540711_17, Date)
{
    Date d1( 9254, 7, 11 );
    Date d2 = d1.addDays( 17 );
    Date expected( 9254, 7, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_93140130_neg17, Date)
{
    Date d1( 9314, 1, 30 );
    Date d2 = d1.addDays( -17 );
    Date expected( 9314, 1, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26920707_19, Date)
{
    Date d1( 2692, 7, 7 );
    Date d2 = d1.addDays( 19 );
    Date expected( 2692, 7, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57940121_neg19, Date)
{
    Date d1( 5794, 1, 21 );
    Date d2 = d1.addDays( -19 );
    Date expected( 5794, 1, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28591105_21, Date)
{
    Date d1( 2859, 11, 5 );
    Date d2 = d1.addDays( 21 );
    Date expected( 2859, 11, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60970922_neg21, Date)
{
    Date d1( 6097, 9, 22 );
    Date d2 = d1.addDays( -21 );
    Date expected( 6097, 9, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36571219_23, Date)
{
    Date d1( 3657, 12, 19 );
    Date d2 = d1.addDays( 23 );
    Date expected( 3658, 1, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_58850716_neg23, Date)
{
    Date d1( 5885, 7, 16 );
    Date d2 = d1.addDays( -23 );
    Date expected( 5885, 6, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56980416_25, Date)
{
    Date d1( 5698, 4, 16 );
    Date d2 = d1.addDays( 25 );
    Date expected( 5698, 5, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32060324_neg25, Date)
{
    Date d1( 3206, 3, 24 );
    Date d2 = d1.addDays( -25 );
    Date expected( 3206, 2, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29241102_27, Date)
{
    Date d1( 2924, 11, 2 );
    Date d2 = d1.addDays( 27 );
    Date expected( 2924, 11, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45010406_neg27, Date)
{
    Date d1( 4501, 4, 6 );
    Date d2 = d1.addDays( -27 );
    Date expected( 4501, 3, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21670806_29, Date)
{
    Date d1( 2167, 8, 6 );
    Date d2 = d1.addDays( 29 );
    Date expected( 2167, 9, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66090214_neg29, Date)
{
    Date d1( 6609, 2, 14 );
    Date d2 = d1.addDays( -29 );
    Date expected( 6609, 1, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82870823_31, Date)
{
    Date d1( 8287, 8, 23 );
    Date d2 = d1.addDays( 31 );
    Date expected( 8287, 9, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75000820_neg31, Date)
{
    Date d1( 7500, 8, 20 );
    Date d2 = d1.addDays( -31 );
    Date expected( 7500, 7, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_33000613_33, Date)
{
    Date d1( 3300, 6, 13 );
    Date d2 = d1.addDays( 33 );
    Date expected( 3300, 7, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_67540618_neg33, Date)
{
    Date d1( 6754, 6, 18 );
    Date d2 = d1.addDays( -33 );
    Date expected( 6754, 5, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36970915_35, Date)
{
    Date d1( 3697, 9, 15 );
    Date d2 = d1.addDays( 35 );
    Date expected( 3697, 10, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_65561202_neg35, Date)
{
    Date d1( 6556, 12, 2 );
    Date d2 = d1.addDays( -35 );
    Date expected( 6556, 10, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_46960722_37, Date)
{
    Date d1( 4696, 7, 22 );
    Date d2 = d1.addDays( 37 );
    Date expected( 4696, 8, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74420105_neg37, Date)
{
    Date d1( 7442, 1, 5 );
    Date d2 = d1.addDays( -37 );
    Date expected( 7441, 11, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_93480531_39, Date)
{
    Date d1( 9348, 5, 31 );
    Date d2 = d1.addDays( 39 );
    Date expected( 9348, 7, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_93091121_neg39, Date)
{
    Date d1( 9309, 11, 21 );
    Date d2 = d1.addDays( -39 );
    Date expected( 9309, 10, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_70120413_41, Date)
{
    Date d1( 7012, 4, 13 );
    Date d2 = d1.addDays( 41 );
    Date expected( 7012, 5, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_34261122_neg41, Date)
{
    Date d1( 3426, 11, 22 );
    Date d2 = d1.addDays( -41 );
    Date expected( 3426, 10, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74310123_43, Date)
{
    Date d1( 7431, 1, 23 );
    Date d2 = d1.addDays( 43 );
    Date expected( 7431, 3, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53500209_neg43, Date)
{
    Date d1( 5350, 2, 9 );
    Date d2 = d1.addDays( -43 );
    Date expected( 5349, 12, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56960629_45, Date)
{
    Date d1( 5696, 6, 29 );
    Date d2 = d1.addDays( 45 );
    Date expected( 5696, 8, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53330103_neg45, Date)
{
    Date d1( 5333, 1, 3 );
    Date d2 = d1.addDays( -45 );
    Date expected( 5332, 11, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22930103_47, Date)
{
    Date d1( 2293, 1, 3 );
    Date d2 = d1.addDays( 47 );
    Date expected( 2293, 2, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74341130_neg47, Date)
{
    Date d1( 7434, 11, 30 );
    Date d2 = d1.addDays( -47 );
    Date expected( 7434, 10, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78651207_49, Date)
{
    Date d1( 7865, 12, 7 );
    Date d2 = d1.addDays( 49 );
    Date expected( 7866, 1, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_46121030_neg49, Date)
{
    Date d1( 4612, 10, 30 );
    Date d2 = d1.addDays( -49 );
    Date expected( 4612, 9, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_39750922_51, Date)
{
    Date d1( 3975, 9, 22 );
    Date d2 = d1.addDays( 51 );
    Date expected( 3975, 11, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45910531_neg51, Date)
{
    Date d1( 4591, 5, 31 );
    Date d2 = d1.addDays( -51 );
    Date expected( 4591, 4, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87860402_53, Date)
{
    Date d1( 8786, 4, 2 );
    Date d2 = d1.addDays( 53 );
    Date expected( 8786, 5, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52941230_neg53, Date)
{
    Date d1( 5294, 12, 30 );
    Date d2 = d1.addDays( -53 );
    Date expected( 5294, 11, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50420615_55, Date)
{
    Date d1( 5042, 6, 15 );
    Date d2 = d1.addDays( 55 );
    Date expected( 5042, 8, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96190629_neg55, Date)
{
    Date d1( 9619, 6, 29 );
    Date d2 = d1.addDays( -55 );
    Date expected( 9619, 5, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_30290514_57, Date)
{
    Date d1( 3029, 5, 14 );
    Date d2 = d1.addDays( 57 );
    Date expected( 3029, 7, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_42270712_neg57, Date)
{
    Date d1( 4227, 7, 12 );
    Date d2 = d1.addDays( -57 );
    Date expected( 4227, 5, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_34710317_59, Date)
{
    Date d1( 3471, 3, 17 );
    Date d2 = d1.addDays( 59 );
    Date expected( 3471, 5, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53920807_neg59, Date)
{
    Date d1( 5392, 8, 7 );
    Date d2 = d1.addDays( -59 );
    Date expected( 5392, 6, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71241012_61, Date)
{
    Date d1( 7124, 10, 12 );
    Date d2 = d1.addDays( 61 );
    Date expected( 7124, 12, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_58020623_neg61, Date)
{
    Date d1( 5802, 6, 23 );
    Date d2 = d1.addDays( -61 );
    Date expected( 5802, 4, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20240311_63, Date)
{
    Date d1( 2024, 3, 11 );
    Date d2 = d1.addDays( 63 );
    Date expected( 2024, 5, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27730606_neg63, Date)
{
    Date d1( 2773, 6, 6 );
    Date d2 = d1.addDays( -63 );
    Date expected( 2773, 4, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51040218_65, Date)
{
    Date d1( 5104, 2, 18 );
    Date d2 = d1.addDays( 65 );
    Date expected( 5104, 4, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_98841208_neg65, Date)
{
    Date d1( 9884, 12, 8 );
    Date d2 = d1.addDays( -65 );
    Date expected( 9884, 10, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_67970921_67, Date)
{
    Date d1( 6797, 9, 21 );
    Date d2 = d1.addDays( 67 );
    Date expected( 6797, 11, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_94760401_neg67, Date)
{
    Date d1( 9476, 4, 1 );
    Date d2 = d1.addDays( -67 );
    Date expected( 9476, 1, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51050612_69, Date)
{
    Date d1( 5105, 6, 12 );
    Date d2 = d1.addDays( 69 );
    Date expected( 5105, 8, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_89070117_neg69, Date)
{
    Date d1( 8907, 1, 17 );
    Date d2 = d1.addDays( -69 );
    Date expected( 8906, 11, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_44300304_71, Date)
{
    Date d1( 4430, 3, 4 );
    Date d2 = d1.addDays( 71 );
    Date expected( 4430, 5, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36520131_neg71, Date)
{
    Date d1( 3652, 1, 31 );
    Date d2 = d1.addDays( -71 );
    Date expected( 3651, 11, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27450317_73, Date)
{
    Date d1( 2745, 3, 17 );
    Date d2 = d1.addDays( 73 );
    Date expected( 2745, 5, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_47760831_neg73, Date)
{
    Date d1( 4776, 8, 31 );
    Date d2 = d1.addDays( -73 );
    Date expected( 4776, 6, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50100729_75, Date)
{
    Date d1( 5010, 7, 29 );
    Date d2 = d1.addDays( 75 );
    Date expected( 5010, 10, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_72980316_neg75, Date)
{
    Date d1( 7298, 3, 16 );
    Date d2 = d1.addDays( -75 );
    Date expected( 7297, 12, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91520423_77, Date)
{
    Date d1( 9152, 4, 23 );
    Date d2 = d1.addDays( 77 );
    Date expected( 9152, 7, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35280515_neg77, Date)
{
    Date d1( 3528, 5, 15 );
    Date d2 = d1.addDays( -77 );
    Date expected( 3528, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_86390619_79, Date)
{
    Date d1( 8639, 6, 19 );
    Date d2 = d1.addDays( 79 );
    Date expected( 8639, 9, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60660524_neg79, Date)
{
    Date d1( 6066, 5, 24 );
    Date d2 = d1.addDays( -79 );
    Date expected( 6066, 3, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76850911_81, Date)
{
    Date d1( 7685, 9, 11 );
    Date d2 = d1.addDays( 81 );
    Date expected( 7685, 12, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_86111106_neg81, Date)
{
    Date d1( 8611, 11, 6 );
    Date d2 = d1.addDays( -81 );
    Date expected( 8611, 8, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_62480927_83, Date)
{
    Date d1( 6248, 9, 27 );
    Date d2 = d1.addDays( 83 );
    Date expected( 6248, 12, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40940308_neg83, Date)
{
    Date d1( 4094, 3, 8 );
    Date d2 = d1.addDays( -83 );
    Date expected( 4093, 12, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36780827_85, Date)
{
    Date d1( 3678, 8, 27 );
    Date d2 = d1.addDays( 85 );
    Date expected( 3678, 11, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32031003_neg85, Date)
{
    Date d1( 3203, 10, 3 );
    Date d2 = d1.addDays( -85 );
    Date expected( 3203, 7, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92760410_87, Date)
{
    Date d1( 9276, 4, 10 );
    Date d2 = d1.addDays( 87 );
    Date expected( 9276, 7, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57050315_neg87, Date)
{
    Date d1( 5705, 3, 15 );
    Date d2 = d1.addDays( -87 );
    Date expected( 5704, 12, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_59210930_89, Date)
{
    Date d1( 5921, 9, 30 );
    Date d2 = d1.addDays( 89 );
    Date expected( 5921, 12, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60050307_neg89, Date)
{
    Date d1( 6005, 3, 7 );
    Date d2 = d1.addDays( -89 );
    Date expected( 6004, 12, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71390612_91, Date)
{
    Date d1( 7139, 6, 12 );
    Date d2 = d1.addDays( 91 );
    Date expected( 7139, 9, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97300219_neg91, Date)
{
    Date d1( 9730, 2, 19 );
    Date d2 = d1.addDays( -91 );
    Date expected( 9729, 11, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_84410625_93, Date)
{
    Date d1( 8441, 6, 25 );
    Date d2 = d1.addDays( 93 );
    Date expected( 8441, 9, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_70460304_neg93, Date)
{
    Date d1( 7046, 3, 4 );
    Date d2 = d1.addDays( -93 );
    Date expected( 7045, 12, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81980504_95, Date)
{
    Date d1( 8198, 5, 4 );
    Date d2 = d1.addDays( 95 );
    Date expected( 8198, 8, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32351024_neg95, Date)
{
    Date d1( 3235, 10, 24 );
    Date d2 = d1.addDays( -95 );
    Date expected( 3235, 7, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_25110804_97, Date)
{
    Date d1( 2511, 8, 4 );
    Date d2 = d1.addDays( 97 );
    Date expected( 2511, 11, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75620218_neg97, Date)
{
    Date d1( 7562, 2, 18 );
    Date d2 = d1.addDays( -97 );
    Date expected( 7561, 11, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_47180108_99, Date)
{
    Date d1( 4718, 1, 8 );
    Date d2 = d1.addDays( 99 );
    Date expected( 4718, 4, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_77881021_neg99, Date)
{
    Date d1( 7788, 10, 21 );
    Date d2 = d1.addDays( -99 );
    Date expected( 7788, 7, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57491222_101, Date)
{
    Date d1( 5749, 12, 22 );
    Date d2 = d1.addDays( 101 );
    Date expected( 5750, 4, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61330324_neg101, Date)
{
    Date d1( 6133, 3, 24 );
    Date d2 = d1.addDays( -101 );
    Date expected( 6132, 12, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}

TEST(add_day_29800912_202, Date)
{
    Date d1( 2980, 9, 12 );
    Date d2 = d1.addDays( 202 );
    Date expected( 2981, 4, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27870929_neg202, Date)
{
    Date d1( 2787, 9, 29 );
    Date d2 = d1.addDays( -202 );
    Date expected( 2787, 3, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40630303_403, Date)
{
    Date d1( 4063, 3, 3 );
    Date d2 = d1.addDays( 403 );
    Date expected( 4064, 4, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40770814_neg403, Date)
{
    Date d1( 4077, 8, 14 );
    Date d2 = d1.addDays( -403 );
    Date expected( 4076, 7, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66430123_604, Date)
{
    Date d1( 6643, 1, 23 );
    Date d2 = d1.addDays( 604 );
    Date expected( 6644, 9, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82990222_neg604, Date)
{
    Date d1( 8299, 2, 22 );
    Date d2 = d1.addDays( -604 );
    Date expected( 8297, 6, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_85861110_805, Date)
{
    Date d1( 8586, 11, 10 );
    Date d2 = d1.addDays( 805 );
    Date expected( 8589, 1, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_38450606_neg805, Date)
{
    Date d1( 3845, 6, 6 );
    Date d2 = d1.addDays( -805 );
    Date expected( 3843, 3, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76550522_1006, Date)
{
    Date d1( 7655, 5, 22 );
    Date d2 = d1.addDays( 1006 );
    Date expected( 7658, 2, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_39470114_neg1006, Date)
{
    Date d1( 3947, 1, 14 );
    Date d2 = d1.addDays( -1006 );
    Date expected( 3944, 4, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87690906_1207, Date)
{
    Date d1( 8769, 9, 6 );
    Date d2 = d1.addDays( 1207 );
    Date expected( 8772, 12, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90280120_neg1207, Date)
{
    Date d1( 9028, 1, 20 );
    Date d2 = d1.addDays( -1207 );
    Date expected( 9024, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_44070430_1408, Date)
{
    Date d1( 4407, 4, 30 );
    Date d2 = d1.addDays( 1408 );
    Date expected( 4411, 3, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_55840916_neg1408, Date)
{
    Date d1( 5584, 9, 16 );
    Date d2 = d1.addDays( -1408 );
    Date expected( 5580, 11, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78400616_1609, Date)
{
    Date d1( 7840, 6, 16 );
    Date d2 = d1.addDays( 1609 );
    Date expected( 7844, 11, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97471114_neg1609, Date)
{
    Date d1( 9747, 11, 14 );
    Date d2 = d1.addDays( -1609 );
    Date expected( 9743, 6, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_58791109_1810, Date)
{
    Date d1( 5879, 11, 9 );
    Date d2 = d1.addDays( 1810 );
    Date expected( 5884, 10, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96660815_neg1810, Date)
{
    Date d1( 9666, 8, 15 );
    Date d2 = d1.addDays( -1810 );
    Date expected( 9661, 8, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_54981103_2011, Date)
{
    Date d1( 5498, 11, 3 );
    Date d2 = d1.addDays( 2011 );
    Date expected( 5504, 5, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29170625_neg2011, Date)
{
    Date d1( 2917, 6, 25 );
    Date d2 = d1.addDays( -2011 );
    Date expected( 2911, 12, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73820825_2212, Date)
{
    Date d1( 7382, 8, 25 );
    Date d2 = d1.addDays( 2212 );
    Date expected( 7388, 9, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96821106_neg2212, Date)
{
    Date d1( 9682, 11, 6 );
    Date d2 = d1.addDays( -2212 );
    Date expected( 9676, 10, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_38260301_2413, Date)
{
    Date d1( 3826, 3, 1 );
    Date d2 = d1.addDays( 2413 );
    Date expected( 3832, 10, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_72960713_neg2413, Date)
{
    Date d1( 7296, 7, 13 );
    Date d2 = d1.addDays( -2413 );
    Date expected( 7289, 12, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66791206_2614, Date)
{
    Date d1( 6679, 12, 6 );
    Date d2 = d1.addDays( 2614 );
    Date expected( 6687, 2, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_98440223_neg2614, Date)
{
    Date d1( 9844, 2, 23 );
    Date d2 = d1.addDays( -2614 );
    Date expected( 9836, 12, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50050227_2815, Date)
{
    Date d1( 5005, 2, 27 );
    Date d2 = d1.addDays( 2815 );
    Date expected( 5012, 11, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_68031206_neg2815, Date)
{
    Date d1( 6803, 12, 6 );
    Date d2 = d1.addDays( -2815 );
    Date expected( 6796, 3, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40770421_3016, Date)
{
    Date d1( 4077, 4, 21 );
    Date d2 = d1.addDays( 3016 );
    Date expected( 4085, 7, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52701106_neg3016, Date)
{
    Date d1( 5270, 11, 6 );
    Date d2 = d1.addDays( -3016 );
    Date expected( 5262, 8, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22910106_3217, Date)
{
    Date d1( 2291, 1, 6 );
    Date d2 = d1.addDays( 3217 );
    Date expected( 2299, 10, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88910816_neg3217, Date)
{
    Date d1( 8891, 8, 16 );
    Date d2 = d1.addDays( -3217 );
    Date expected( 8882, 10, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_47760617_3418, Date)
{
    Date d1( 4776, 6, 17 );
    Date d2 = d1.addDays( 3418 );
    Date expected( 4785, 10, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73261022_neg3418, Date)
{
    Date d1( 7326, 10, 22 );
    Date d2 = d1.addDays( -3418 );
    Date expected( 7317, 6, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91800428_3619, Date)
{
    Date d1( 9180, 4, 28 );
    Date d2 = d1.addDays( 3619 );
    Date expected( 9190, 3, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74940813_neg3619, Date)
{
    Date d1( 7494, 8, 13 );
    Date d2 = d1.addDays( -3619 );
    Date expected( 7484, 9, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_63010718_3820, Date)
{
    Date d1( 6301, 7, 18 );
    Date d2 = d1.addDays( 3820 );
    Date expected( 6312, 1, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53241124_neg3820, Date)
{
    Date d1( 5324, 11, 24 );
    Date d2 = d1.addDays( -3820 );
    Date expected( 5314, 6, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_43060827_4021, Date)
{
    Date d1( 4306, 8, 27 );
    Date d2 = d1.addDays( 4021 );
    Date expected( 4317, 8, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26380413_neg4021, Date)
{
    Date d1( 2638, 4, 13 );
    Date d2 = d1.addDays( -4021 );
    Date expected( 2627, 4, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52700731_4222, Date)
{
    Date d1( 5270, 7, 31 );
    Date d2 = d1.addDays( 4222 );
    Date expected( 5282, 2, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26570303_neg4222, Date)
{
    Date d1( 2657, 3, 3 );
    Date d2 = d1.addDays( -4222 );
    Date expected( 2645, 8, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_79560304_4423, Date)
{
    Date d1( 7956, 3, 4 );
    Date d2 = d1.addDays( 4423 );
    Date expected( 7968, 4, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_33001207_neg4423, Date)
{
    Date d1( 3300, 12, 7 );
    Date d2 = d1.addDays( -4423 );
    Date expected( 3288, 10, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92370107_4624, Date)
{
    Date d1( 9237, 1, 7 );
    Date d2 = d1.addDays( 4624 );
    Date expected( 9249, 9, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_63621220_neg4624, Date)
{
    Date d1( 6362, 12, 20 );
    Date d2 = d1.addDays( -4624 );
    Date expected( 6350, 4, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88061021_4825, Date)
{
    Date d1( 8806, 10, 21 );
    Date d2 = d1.addDays( 4825 );
    Date expected( 8820, 1, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_25540829_neg4825, Date)
{
    Date d1( 2554, 8, 29 );
    Date d2 = d1.addDays( -4825 );
    Date expected( 2541, 6, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82171223_5026, Date)
{
    Date d1( 8217, 12, 23 );
    Date d2 = d1.addDays( 5026 );
    Date expected( 8231, 9, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78110702_neg5026, Date)
{
    Date d1( 7811, 7, 2 );
    Date d2 = d1.addDays( -5026 );
    Date expected( 7797, 9, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_83940516_5227, Date)
{
    Date d1( 8394, 5, 16 );
    Date d2 = d1.addDays( 5227 );
    Date expected( 8408, 9, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90491114_neg5227, Date)
{
    Date d1( 9049, 11, 14 );
    Date d2 = d1.addDays( -5227 );
    Date expected( 9035, 7, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71710506_5428, Date)
{
    Date d1( 7171, 5, 6 );
    Date d2 = d1.addDays( 5428 );
    Date expected( 7186, 3, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50640624_neg5428, Date)
{
    Date d1( 5064, 6, 24 );
    Date d2 = d1.addDays( -5428 );
    Date expected( 5049, 8, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95540206_5629, Date)
{
    Date d1( 9554, 2, 6 );
    Date d2 = d1.addDays( 5629 );
    Date expected( 9569, 7, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49350910_neg5629, Date)
{
    Date d1( 4935, 9, 10 );
    Date d2 = d1.addDays( -5629 );
    Date expected( 4920, 4, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27280215_5830, Date)
{
    Date d1( 2728, 2, 15 );
    Date d2 = d1.addDays( 5830 );
    Date expected( 2744, 2, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52470915_neg5830, Date)
{
    Date d1( 5247, 9, 15 );
    Date d2 = d1.addDays( -5830 );
    Date expected( 5231, 9, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_31160607_6031, Date)
{
    Date d1( 3116, 6, 7 );
    Date d2 = d1.addDays( 6031 );
    Date expected( 3132, 12, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78021007_neg6031, Date)
{
    Date d1( 7802, 10, 7 );
    Date d2 = d1.addDays( -6031 );
    Date expected( 7786, 4, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91320816_6232, Date)
{
    Date d1( 9132, 8, 16 );
    Date d2 = d1.addDays( 6232 );
    Date expected( 9149, 9, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_67240610_neg6232, Date)
{
    Date d1( 6724, 6, 10 );
    Date d2 = d1.addDays( -6232 );
    Date expected( 6707, 5, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92511204_6433, Date)
{
    Date d1( 9251, 12, 4 );
    Date d2 = d1.addDays( 6433 );
    Date expected( 9269, 7, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22480312_neg6433, Date)
{
    Date d1( 2248, 3, 12 );
    Date d2 = d1.addDays( -6433 );
    Date expected( 2230, 8, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88251003_6634, Date)
{
    Date d1( 8825, 10, 3 );
    Date d2 = d1.addDays( 6634 );
    Date expected( 8843, 12, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97690525_neg6634, Date)
{
    Date d1( 9769, 5, 25 );
    Date d2 = d1.addDays( -6634 );
    Date expected( 9751, 3, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57810712_6835, Date)
{
    Date d1( 5781, 7, 12 );
    Date d2 = d1.addDays( 6835 );
    Date expected( 5800, 3, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_64660306_neg6835, Date)
{
    Date d1( 6466, 3, 6 );
    Date d2 = d1.addDays( -6835 );
    Date expected( 6447, 6, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35030711_7036, Date)
{
    Date d1( 3503, 7, 11 );
    Date d2 = d1.addDays( 7036 );
    Date expected( 3522, 10, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_85800519_neg7036, Date)
{
    Date d1( 8580, 5, 19 );
    Date d2 = d1.addDays( -7036 );
    Date expected( 8561, 2, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_67840417_7237, Date)
{
    Date d1( 6784, 4, 17 );
    Date d2 = d1.addDays( 7237 );
    Date expected( 6804, 2, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35580825_neg7237, Date)
{
    Date d1( 3558, 8, 25 );
    Date d2 = d1.addDays( -7237 );
    Date expected( 3538, 11, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73170216_7438, Date)
{
    Date d1( 7317, 2, 16 );
    Date d2 = d1.addDays( 7438 );
    Date expected( 7337, 6, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49171212_neg7438, Date)
{
    Date d1( 4917, 12, 12 );
    Date d2 = d1.addDays( -7438 );
    Date expected( 4897, 7, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28820604_7639, Date)
{
    Date d1( 2882, 6, 4 );
    Date d2 = d1.addDays( 7639 );
    Date expected( 2903, 5, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82230404_neg7639, Date)
{
    Date d1( 8223, 4, 4 );
    Date d2 = d1.addDays( -7639 );
    Date expected( 8202, 5, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22380421_7840, Date)
{
    Date d1( 2238, 4, 21 );
    Date d2 = d1.addDays( 7840 );
    Date expected( 2259, 10, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24001029_neg7840, Date)
{
    Date d1( 2400, 10, 29 );
    Date d2 = d1.addDays( -7840 );
    Date expected( 2379, 5, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88450530_8041, Date)
{
    Date d1( 8845, 5, 30 );
    Date d2 = d1.addDays( 8041 );
    Date expected( 8867, 6, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26980212_neg8041, Date)
{
    Date d1( 2698, 2, 12 );
    Date d2 = d1.addDays( -8041 );
    Date expected( 2676, 2, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88350419_8242, Date)
{
    Date d1( 8835, 4, 19 );
    Date d2 = d1.addDays( 8242 );
    Date expected( 8857, 11, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57940927_neg8242, Date)
{
    Date d1( 5794, 9, 27 );
    Date d2 = d1.addDays( -8242 );
    Date expected( 5772, 3, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_72510307_8443, Date)
{
    Date d1( 7251, 3, 7 );
    Date d2 = d1.addDays( 8443 );
    Date expected( 7274, 4, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_70040908_neg8443, Date)
{
    Date d1( 7004, 9, 8 );
    Date d2 = d1.addDays( -8443 );
    Date expected( 6981, 7, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32921124_8644, Date)
{
    Date d1( 3292, 11, 24 );
    Date d2 = d1.addDays( 8644 );
    Date expected( 3316, 7, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96450729_neg8644, Date)
{
    Date d1( 9645, 7, 29 );
    Date d2 = d1.addDays( -8644 );
    Date expected( 9621, 11, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35820127_8845, Date)
{
    Date d1( 3582, 1, 27 );
    Date d2 = d1.addDays( 8845 );
    Date expected( 3606, 4, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49810408_neg8845, Date)
{
    Date d1( 4981, 4, 8 );
    Date d2 = d1.addDays( -8845 );
    Date expected( 4957, 1, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29310523_9046, Date)
{
    Date d1( 2931, 5, 23 );
    Date d2 = d1.addDays( 9046 );
    Date expected( 2956, 2, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52750613_neg9046, Date)
{
    Date d1( 5275, 6, 13 );
    Date d2 = d1.addDays( -9046 );
    Date expected( 5250, 9, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23641104_9247, Date)
{
    Date d1( 2364, 11, 4 );
    Date d2 = d1.addDays( 9247 );
    Date expected( 2390, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73290524_neg9247, Date)
{
    Date d1( 7329, 5, 24 );
    Date d2 = d1.addDays( -9247 );
    Date expected( 7304, 1, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_46100618_9448, Date)
{
    Date d1( 4610, 6, 18 );
    Date d2 = d1.addDays( 9448 );
    Date expected( 4636, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27500102_neg9448, Date)
{
    Date d1( 2750, 1, 2 );
    Date d2 = d1.addDays( -9448 );
    Date expected( 2724, 2, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71510822_9649, Date)
{
    Date d1( 7151, 8, 22 );
    Date d2 = d1.addDays( 9649 );
    Date expected( 7178, 1, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_72910322_neg9649, Date)
{
    Date d1( 7291, 3, 22 );
    Date d2 = d1.addDays( -9649 );
    Date expected( 7264, 10, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21310323_9850, Date)
{
    Date d1( 2131, 3, 23 );
    Date d2 = d1.addDays( 9850 );
    Date expected( 2158, 3, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66160518_neg9850, Date)
{
    Date d1( 6616, 5, 18 );
    Date d2 = d1.addDays( -9850 );
    Date expected( 6589, 5, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}

TEST(add_month_20140606_P00002, Date)
{
    Date d1( 2014, 6, 6 );
    Date d2 = d1.addMonths( 2 );
    Date expected( 2014, 8, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}
/* landing on the leap day */
TEST(add_day_93160130_1, Date)
{
    Date d1( 9316, 1, 30 );
    Date d2 = d1.addMonths( 1 );
    Date expected( 9316, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_48960330_neg1, Date)
{
    Date d1( 4896, 3, 30 );
    Date d2 = d1.addMonths( -1 );
    Date expected( 4896, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28191231_2, Date)
{
    Date d1( 2819, 12, 31 );
    Date d2 = d1.addMonths( 2 );
    Date expected( 2820, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_84160429_neg2, Date)
{
    Date d1( 8416, 4, 29 );
    Date d2 = d1.addMonths( -2 );
    Date expected( 8416, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_55031129_3, Date)
{
    
    Date d1( 5503, 11, 29 );
    Date d2 = d1.addMonths( 3 );
    Date expected( 5504, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_68040530_neg3, Date)
{
    Date d1( 6804, 5, 30 );
    Date d2 = d1.addMonths( -3 );
    Date expected( 6804, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50351031_4, Date)
{
    Date d1( 5035, 10, 31 );
    Date d2 = d1.addMonths( 4 );
    Date expected( 5036, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95160630_neg4, Date)
{
    Date d1( 9516, 6, 30 );
    Date d2 = d1.addMonths( -4 );
    Date expected( 9516, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95590930_5, Date)
{
    Date d1( 9559, 9, 30 );
    Date d2 = d1.addMonths( 5 );
    Date expected( 9560, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73280731_neg5, Date)
{
    Date d1( 7328, 7, 31 );
    Date d2 = d1.addMonths( -5 );
    Date expected( 7328, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56030830_6, Date)
{
    Date d1( 5603, 8, 30 );
    Date d2 = d1.addMonths( 6 );
    Date expected( 5604, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49920830_neg6, Date)
{
    Date d1( 4992, 8, 30 );
    Date d2 = d1.addMonths( -6 );
    Date expected( 4992, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76390730_7, Date)
{
    Date d1( 7639, 7, 30 );
    Date d2 = d1.addMonths( 7 );
    Date expected( 7640, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95440930_neg7, Date)
{
    Date d1( 9544, 9, 30 );
    Date d2 = d1.addMonths( -7 );
    Date expected( 9544, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61630630_8, Date)
{
    Date d1( 6163, 6, 30 );
    Date d2 = d1.addMonths( 8 );
    Date expected( 6164, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97321029_neg8, Date)
{
    Date d1( 9732, 10, 29 );
    Date d2 = d1.addMonths( -8 );
    Date expected( 9732, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_42430530_9, Date)
{
    Date d1( 4243, 5, 30 );
    Date d2 = d1.addMonths( 9 );
    Date expected( 4244, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50361129_neg9, Date)
{
    Date d1( 5036, 11, 29 );
    Date d2 = d1.addMonths( -9 );
    Date expected( 5036, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32590430_10, Date)
{
    Date d1( 3259, 4, 30 );
    Date d2 = d1.addMonths( 10 );
    Date expected( 3260, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40841231_neg10, Date)
{
    Date d1( 4084, 12, 31 );
    Date d2 = d1.addMonths( -10 );
    Date expected( 4084, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32630331_11, Date)
{
    Date d1( 3263, 3, 31 );
    Date d2 = d1.addMonths( 11 );
    Date expected( 3264, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_63530129_neg11, Date)
{
    Date d1( 6353, 1, 29 );
    Date d2 = d1.addMonths( -11 );
    Date expected( 6352, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24470131_13, Date)
{
    Date d1( 2447, 1, 31 );
    Date d2 = d1.addMonths( 13 );
    Date expected( 2448, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_37610330_neg13, Date)
{
    Date d1( 3761, 3, 30 );
    Date d2 = d1.addMonths( -13 );
    Date expected( 3760, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
/* landing on Feb 28 NOT in a leap year */
TEST(add_day_47490129_1, Date)
{
    Date d1( 4749, 1, 29 );
    Date d2 = d1.addMonths( 1 );
    Date expected( 4749, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_79210331_neg1, Date)
{
    Date d1( 7921, 3, 31 );
    Date d2 = d1.addMonths( -1 );
    Date expected( 7921, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_99531230_2, Date)
{
    Date d1( 9953, 12, 30 );
    Date d2 = d1.addMonths( 2 );
    Date expected( 9954, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27980430_neg2, Date)
{
    Date d1( 2798, 4, 30 );
    Date d2 = d1.addMonths( -2 );
    Date expected( 2798, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_50981130_3, Date)
{
    Date d1( 5098, 11, 30 );
    Date d2 = d1.addMonths( 3 );
    Date expected( 5099, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51620529_neg3, Date)
{
    Date d1( 5162, 5, 29 );
    Date d2 = d1.addMonths( -3 );
    Date expected( 5162, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61331030_4, Date)
{
    Date d1( 6133, 10, 30 );
    Date d2 = d1.addMonths( 4 );
    Date expected( 6134, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81370630_neg4, Date)
{
    Date d1( 8137, 6, 30 );
    Date d2 = d1.addMonths( -4 );
    Date expected( 8137, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_69100930_5, Date)
{
    Date d1( 6910, 9, 30 );
    Date d2 = d1.addMonths( 5 );
    Date expected( 6911, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87230731_neg5, Date)
{
    Date d1( 8723, 7, 31 );
    Date d2 = d1.addMonths( -5 );
    Date expected( 8723, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_80060830_6, Date)
{
    Date d1( 8006, 8, 30 );
    Date d2 = d1.addMonths( 6 );
    Date expected( 8007, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56890829_neg6, Date)
{
    Date d1( 5689, 8, 29 );
    Date d2 = d1.addMonths( -6 );
    Date expected( 5689, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61440731_7, Date)
{
    Date d1( 6144, 7, 31 );
    Date d2 = d1.addMonths( 7 );
    Date expected( 6145, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_48270929_neg7, Date)
{
    Date d1( 4827, 9, 29 );
    Date d2 = d1.addMonths( -7 );
    Date expected( 4827, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56960630_8, Date)
{
    Date d1( 5696, 6, 30 );
    Date d2 = d1.addMonths( 8 );
    Date expected( 5697, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51971031_neg8, Date)
{
    Date d1( 5197, 10, 31 );
    Date d2 = d1.addMonths( -8 );
    Date expected( 5197, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_31780531_9, Date)
{
    Date d1( 3178, 5, 31 );
    Date d2 = d1.addMonths( 9 );
    Date expected( 3179, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_30511129_neg9, Date)
{
    Date d1( 3051, 11, 29 );
    Date d2 = d1.addMonths( -9 );
    Date expected( 3051, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35890429_10, Date)
{
    Date d1( 3589, 4, 29 );
    Date d2 = d1.addMonths( 10 );
    Date expected( 3590, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29781230_neg10, Date)
{
    Date d1( 2978, 12, 30 );
    Date d2 = d1.addMonths( -10 );
    Date expected( 2978, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49560330_11, Date)
{
    Date d1( 4956, 3, 30 );
    Date d2 = d1.addMonths( 11 );
    Date expected( 4957, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74540130_neg11, Date)
{
    Date d1( 7454, 1, 30 );
    Date d2 = d1.addMonths( -11 );
    Date expected( 7453, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81640229_12, Date)
{
    Date d1( 8164, 2, 29 );
    Date d2 = d1.addMonths( 12 );
    Date expected( 8165, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_85200229_neg12, Date)
{
    Date d1( 8520, 2, 29 );
    Date d2 = d1.addMonths( -12 );
    Date expected( 8519, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_67840129_13, Date)
{
    Date d1( 6784, 1, 29 );
    Date d2 = d1.addMonths( 13 );
    Date expected( 6785, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66960331_neg13, Date)
{
    Date d1( 6696, 3, 31 );
    Date d2 = d1.addMonths( -13 );
    Date expected( 6695, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}

/* from the 31st into a month with < 31 days */
TEST(add_day_20360831_1, Date)
{
    Date d1( 2036, 8, 31 );
    Date d2 = d1.addMonths( 1 );
    Date expected( 2036, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21080531_neg1, Date)
{
    Date d1( 2108, 5, 31 );
    Date d2 = d1.addMonths( -1 );
    Date expected( 2108, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20830731_2, Date)
{
    Date d1( 2083, 7, 31 );
    Date d2 = d1.addMonths( 2 );
    Date expected( 2083, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21620831_neg2, Date)
{
    Date d1( 2162, 8, 31 );
    Date d2 = d1.addMonths( -2 );
    Date expected( 2162, 6, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20130831_3, Date)
{
    Date d1( 2013, 8, 31 );
    Date d2 = d1.addMonths( 3 );
    Date expected( 2013, 11, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20401231_neg3, Date)
{
    Date d1( 2040, 12, 31 );
    Date d2 = d1.addMonths( -3 );
    Date expected( 2040, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21510731_4, Date)
{
    Date d1( 2151, 7, 31 );
    Date d2 = d1.addMonths( 4 );
    Date expected( 2151, 11, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20300831_neg4, Date)
{
    Date d1( 2030, 8, 31 );
    Date d2 = d1.addMonths( -4 );
    Date expected( 2030, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21140131_5, Date)
{
    Date d1( 2114, 1, 31 );
    Date d2 = d1.addMonths( 5 );
    Date expected( 2114, 6, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21960731_neg5, Date)
{
    Date d1( 2196, 7, 31 );
    Date d2 = d1.addMonths( -5 );
    Date expected( 2196, 2, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21920831_6, Date)
{
    Date d1( 2192, 8, 31 );
    Date d2 = d1.addMonths( 6 );
    Date expected( 2193, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20070331_neg6, Date)
{
    Date d1( 2007, 3, 31 );
    Date d2 = d1.addMonths( -6 );
    Date expected( 2006, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21600731_7, Date)
{
    Date d1( 2160, 7, 31 );
    Date d2 = d1.addMonths( 7 );
    Date expected( 2161, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21900131_neg7, Date)
{
    Date d1( 2190, 1, 31 );
    Date d2 = d1.addMonths( -7 );
    Date expected( 2189, 6, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21981031_8, Date)
{
    Date d1( 2198, 10, 31 );
    Date d2 = d1.addMonths( 8 );
    Date expected( 2199, 6, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20501231_neg8, Date)
{
    Date d1( 2050, 12, 31 );
    Date d2 = d1.addMonths( -8 );
    Date expected( 2050, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21741231_9, Date)
{
    Date d1( 2174, 12, 31 );
    Date d2 = d1.addMonths( 9 );
    Date expected( 2175, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20050831_neg9, Date)
{
    Date d1( 2005, 8, 31 );
    Date d2 = d1.addMonths( -9 );
    Date expected( 2004, 11, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20640831_10, Date)
{
    Date d1( 2064, 8, 31 );
    Date d2 = d1.addMonths( 10 );
    Date expected( 2065, 6, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20620731_neg10, Date)
{
    Date d1( 2062, 7, 31 );
    Date d2 = d1.addMonths( -10 );
    Date expected( 2061, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20070531_11, Date)
{
    Date d1( 2007, 5, 31 );
    Date d2 = d1.addMonths( 11 );
    Date expected( 2008, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20150831_neg11, Date)
{
    Date d1( 2015, 8, 31 );
    Date d2 = d1.addMonths( -11 );
    Date expected( 2014, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20140831_13, Date)
{
    Date d1( 2014, 8, 31 );
    Date d2 = d1.addMonths( 13 );
    Date expected( 2015, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20950331_neg13, Date)
{
    Date d1( 2095, 3, 31 );
    Date d2 = d1.addMonths( -13 );
    Date expected( 2094, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}

/* random between 1700 and 2200 */
TEST(add_day_31400721_1, Date)
{
    Date d1( 3140, 7, 21 );
    Date d2 = d1.addMonths( 1 );
    Date expected( 3140, 8, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_41951212_neg1, Date)
{
    Date d1( 4195, 12, 12 );
    Date d2 = d1.addMonths( -1 );
    Date expected( 4195, 11, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61480628_2, Date)
{
    Date d1( 6148, 6, 28 );
    Date d2 = d1.addMonths( 2 );
    Date expected( 6148, 8, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26000923_neg2, Date)
{
    Date d1( 2600, 9, 23 );
    Date d2 = d1.addMonths( -2 );
    Date expected( 2600, 7, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53271203_3, Date)
{
    Date d1( 5327, 12, 3 );
    Date d2 = d1.addMonths( 3 );
    Date expected( 5328, 3, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23550908_neg3, Date)
{
    Date d1( 2355, 9, 8 );
    Date d2 = d1.addMonths( -3 );
    Date expected( 2355, 6, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_35850508_4, Date)
{
    Date d1( 3585, 5, 8 );
    Date d2 = d1.addMonths( 4 );
    Date expected( 3585, 9, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20391113_neg4, Date)
{
    Date d1( 2039, 11, 13 );
    Date d2 = d1.addMonths( -4 );
    Date expected( 2039, 7, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_93571112_5, Date)
{
    Date d1( 9357, 11, 12 );
    Date d2 = d1.addMonths( 5 );
    Date expected( 9358, 4, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87980806_neg5, Date)
{
    Date d1( 8798, 8, 6 );
    Date d2 = d1.addMonths( -5 );
    Date expected( 8798, 3, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_69980423_6, Date)
{
    Date d1( 6998, 4, 23 );
    Date d2 = d1.addMonths( 6 );
    Date expected( 6998, 10, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87650109_neg6, Date)
{
    Date d1( 8765, 1, 9 );
    Date d2 = d1.addMonths( -6 );
    Date expected( 8764, 7, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_70660627_7, Date)
{
    Date d1( 7066, 6, 27 );
    Date d2 = d1.addMonths( 7 );
    Date expected( 7067, 1, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_20870820_neg7, Date)
{
    Date d1( 2087, 8, 20 );
    Date d2 = d1.addMonths( -7 );
    Date expected( 2087, 1, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_58070312_8, Date)
{
    Date d1( 5807, 3, 12 );
    Date d2 = d1.addMonths( 8 );
    Date expected( 5807, 11, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_62590317_neg8, Date)
{
    Date d1( 6259, 3, 17 );
    Date d2 = d1.addMonths( -8 );
    Date expected( 6258, 7, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78021120_9, Date)
{
    Date d1( 7802, 11, 20 );
    Date d2 = d1.addMonths( 9 );
    Date expected( 7803, 8, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_39540213_neg9, Date)
{
    Date d1( 3954, 2, 13 );
    Date d2 = d1.addMonths( -9 );
    Date expected( 3953, 5, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66511106_10, Date)
{
    Date d1( 6651, 11, 6 );
    Date d2 = d1.addMonths( 10 );
    Date expected( 6652, 9, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45321208_neg10, Date)
{
    Date d1( 4532, 12, 8 );
    Date d2 = d1.addMonths( -10 );
    Date expected( 4532, 2, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_57930702_11, Date)
{
    Date d1( 5793, 7, 2 );
    Date d2 = d1.addMonths( 11 );
    Date expected( 5794, 6, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28231228_neg11, Date)
{
    Date d1( 2823, 12, 28 );
    Date d2 = d1.addMonths( -11 );
    Date expected( 2823, 1, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73420117_12, Date)
{
    Date d1( 7342, 1, 17 );
    Date d2 = d1.addMonths( 12 );
    Date expected( 7343, 1, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75330606_neg12, Date)
{
    Date d1( 7533, 6, 6 );
    Date d2 = d1.addMonths( -12 );
    Date expected( 7532, 6, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_80900223_13, Date)
{
    Date d1( 8090, 2, 23 );
    Date d2 = d1.addMonths( 13 );
    Date expected( 8091, 3, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_31370717_neg13, Date)
{
    Date d1( 3137, 7, 17 );
    Date d2 = d1.addMonths( -13 );
    Date expected( 3136, 6, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27430816_14, Date)
{
    Date d1( 2743, 8, 16 );
    Date d2 = d1.addMonths( 14 );
    Date expected( 2744, 10, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60691011_neg14, Date)
{
    Date d1( 6069, 10, 11 );
    Date d2 = d1.addMonths( -14 );
    Date expected( 6068, 8, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66310424_15, Date)
{
    Date d1( 6631, 4, 24 );
    Date d2 = d1.addMonths( 15 );
    Date expected( 6632, 7, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45730222_neg15, Date)
{
    Date d1( 4573, 2, 22 );
    Date d2 = d1.addMonths( -15 );
    Date expected( 4571, 11, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95850203_16, Date)
{
    Date d1( 9585, 2, 3 );
    Date d2 = d1.addMonths( 16 );
    Date expected( 9586, 6, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_37610508_neg16, Date)
{
    Date d1( 3761, 5, 8 );
    Date d2 = d1.addMonths( -16 );
    Date expected( 3760, 1, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22670711_17, Date)
{
    Date d1( 2267, 7, 11 );
    Date d2 = d1.addMonths( 17 );
    Date expected( 2268, 12, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_84690410_neg17, Date)
{
    Date d1( 8469, 4, 10 );
    Date d2 = d1.addMonths( -17 );
    Date expected( 8467, 11, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78771219_18, Date)
{
    Date d1( 7877, 12, 19 );
    Date d2 = d1.addMonths( 18 );
    Date expected( 7879, 6, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_25100218_neg18, Date)
{
    Date d1( 2510, 2, 18 );
    Date d2 = d1.addMonths( -18 );
    Date expected( 2508, 8, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96190618_19, Date)
{
    Date d1( 9619, 6, 18 );
    Date d2 = d1.addMonths( 19 );
    Date expected( 9621, 1, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_46531218_neg19, Date)
{
    Date d1( 4653, 12, 18 );
    Date d2 = d1.addMonths( -19 );
    Date expected( 4652, 5, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_92510226_20, Date)
{
    Date d1( 9251, 2, 26 );
    Date d2 = d1.addMonths( 20 );
    Date expected( 9252, 10, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_89910905_neg20, Date)
{
    Date d1( 8991, 9, 5 );
    Date d2 = d1.addMonths( -20 );
    Date expected( 8990, 1, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24911125_21, Date)
{
    Date d1( 2491, 11, 25 );
    Date d2 = d1.addMonths( 21 );
    Date expected( 2493, 8, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23620611_neg21, Date)
{
    Date d1( 2362, 6, 11 );
    Date d2 = d1.addMonths( -21 );
    Date expected( 2360, 9, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_30240623_22, Date)
{
    Date d1( 3024, 6, 23 );
    Date d2 = d1.addMonths( 22 );
    Date expected( 3026, 4, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45790803_neg22, Date)
{
    Date d1( 4579, 8, 3 );
    Date d2 = d1.addMonths( -22 );
    Date expected( 4577, 10, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52190320_23, Date)
{
    Date d1( 5219, 3, 20 );
    Date d2 = d1.addMonths( 23 );
    Date expected( 5221, 2, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_31630423_neg23, Date)
{
    Date d1( 3163, 4, 23 );
    Date d2 = d1.addMonths( -23 );
    Date expected( 3161, 5, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71270103_24, Date)
{
    Date d1( 7127, 1, 3 );
    Date d2 = d1.addMonths( 24 );
    Date expected( 7129, 1, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75270313_neg24, Date)
{
    Date d1( 7527, 3, 13 );
    Date d2 = d1.addMonths( -24 );
    Date expected( 7525, 3, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27320802_25, Date)
{
    Date d1( 2732, 8, 2 );
    Date d2 = d1.addMonths( 25 );
    Date expected( 2734, 9, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_54991018_neg25, Date)
{
    Date d1( 5499, 10, 18 );
    Date d2 = d1.addMonths( -25 );
    Date expected( 5497, 9, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_33151209_26, Date)
{
    Date d1( 3315, 12, 9 );
    Date d2 = d1.addMonths( 26 );
    Date expected( 3318, 2, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60220129_neg26, Date)
{
    Date d1( 6022, 1, 29 );
    Date d2 = d1.addMonths( -26 );
    Date expected( 6019, 11, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_59300103_27, Date)
{
    Date d1( 5930, 1, 3 );
    Date d2 = d1.addMonths( 27 );
    Date expected( 5932, 4, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60491119_neg27, Date)
{
    Date d1( 6049, 11, 19 );
    Date d2 = d1.addMonths( -27 );
    Date expected( 6047, 8, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_94570421_28, Date)
{
    Date d1( 9457, 4, 21 );
    Date d2 = d1.addMonths( 28 );
    Date expected( 9459, 8, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23490717_neg28, Date)
{
    Date d1( 2349, 7, 17 );
    Date d2 = d1.addMonths( -28 );
    Date expected( 2347, 3, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96280307_29, Date)
{
    Date d1( 9628, 3, 7 );
    Date d2 = d1.addMonths( 29 );
    Date expected( 9630, 8, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_64550722_neg29, Date)
{
    Date d1( 6455, 7, 22 );
    Date d2 = d1.addMonths( -29 );
    Date expected( 6453, 2, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_39930304_30, Date)
{
    Date d1( 3993, 3, 4 );
    Date d2 = d1.addMonths( 30 );
    Date expected( 3995, 9, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_89190724_neg30, Date)
{
    Date d1( 8919, 7, 24 );
    Date d2 = d1.addMonths( -30 );
    Date expected( 8917, 1, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_25650223_31, Date)
{
    Date d1( 2565, 2, 23 );
    Date d2 = d1.addMonths( 31 );
    Date expected( 2567, 9, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22310120_neg31, Date)
{
    Date d1( 2231, 1, 20 );
    Date d2 = d1.addMonths( -31 );
    Date expected( 2228, 6, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49961014_32, Date)
{
    Date d1( 4996, 10, 14 );
    Date d2 = d1.addMonths( 32 );
    Date expected( 4999, 6, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_83590419_neg32, Date)
{
    Date d1( 8359, 4, 19 );
    Date d2 = d1.addMonths( -32 );
    Date expected( 8356, 8, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32650512_33, Date)
{
    Date d1( 3265, 5, 12 );
    Date d2 = d1.addMonths( 33 );
    Date expected( 3268, 2, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95410626_neg33, Date)
{
    Date d1( 9541, 6, 26 );
    Date d2 = d1.addMonths( -33 );
    Date expected( 9538, 9, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75261219_34, Date)
{
    Date d1( 7526, 12, 19 );
    Date d2 = d1.addMonths( 34 );
    Date expected( 7529, 10, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_48671114_neg34, Date)
{
    Date d1( 4867, 11, 14 );
    Date d2 = d1.addMonths( -34 );
    Date expected( 4865, 1, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96440207_35, Date)
{
    Date d1( 9644, 2, 7 );
    Date d2 = d1.addMonths( 35 );
    Date expected( 9647, 1, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_64691202_neg35, Date)
{
    Date d1( 6469, 12, 2 );
    Date d2 = d1.addMonths( -35 );
    Date expected( 6467, 1, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23630814_36, Date)
{
    Date d1( 2363, 8, 14 );
    Date d2 = d1.addMonths( 36 );
    Date expected( 2366, 8, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_44380502_neg36, Date)
{
    Date d1( 4438, 5, 2 );
    Date d2 = d1.addMonths( -36 );
    Date expected( 4435, 5, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_79850710_37, Date)
{
    Date d1( 7985, 7, 10 );
    Date d2 = d1.addMonths( 37 );
    Date expected( 7988, 8, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82700405_neg37, Date)
{
    Date d1( 8270, 4, 5 );
    Date d2 = d1.addMonths( -37 );
    Date expected( 8267, 3, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32590204_38, Date)
{
    Date d1( 3259, 2, 4 );
    Date d2 = d1.addMonths( 38 );
    Date expected( 3262, 4, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76780614_neg38, Date)
{
    Date d1( 7678, 6, 14 );
    Date d2 = d1.addMonths( -38 );
    Date expected( 7675, 4, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_71871021_39, Date)
{
    Date d1( 7187, 10, 21 );
    Date d2 = d1.addMonths( 39 );
    Date expected( 7191, 1, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_70450331_neg39, Date)
{
    Date d1( 7045, 3, 31 );
    Date d2 = d1.addMonths( -39 );
    Date expected( 7041, 12, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_79380425_40, Date)
{
    Date d1( 7938, 4, 25 );
    Date d2 = d1.addMonths( 40 );
    Date expected( 7941, 8, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_69170919_neg40, Date)
{
    Date d1( 6917, 9, 19 );
    Date d2 = d1.addMonths( -40 );
    Date expected( 6914, 5, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_56290214_41, Date)
{
    Date d1( 5629, 2, 14 );
    Date d2 = d1.addMonths( 41 );
    Date expected( 5632, 7, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_34090314_neg41, Date)
{
    Date d1( 3409, 3, 14 );
    Date d2 = d1.addMonths( -41 );
    Date expected( 3405, 10, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_74251126_42, Date)
{
    Date d1( 7425, 11, 26 );
    Date d2 = d1.addMonths( 42 );
    Date expected( 7429, 5, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_89360810_neg42, Date)
{
    Date d1( 8936, 8, 10 );
    Date d2 = d1.addMonths( -42 );
    Date expected( 8933, 2, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_78730403_43, Date)
{
    Date d1( 7873, 4, 3 );
    Date d2 = d1.addMonths( 43 );
    Date expected( 7876, 11, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51520205_neg43, Date)
{
    Date d1( 5152, 2, 5 );
    Date d2 = d1.addMonths( -43 );
    Date expected( 5148, 7, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_37990830_44, Date)
{
    Date d1( 3799, 8, 30 );
    Date d2 = d1.addMonths( 44 );
    Date expected( 3803, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76241018_neg44, Date)
{
    Date d1( 7624, 10, 18 );
    Date d2 = d1.addMonths( -44 );
    Date expected( 7621, 2, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_61560124_45, Date)
{
    Date d1( 6156, 1, 24 );
    Date d2 = d1.addMonths( 45 );
    Date expected( 6159, 10, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_36870918_neg45, Date)
{
    Date d1( 3687, 9, 18 );
    Date d2 = d1.addMonths( -45 );
    Date expected( 3683, 12, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40160124_46, Date)
{
    Date d1( 4016, 1, 24 );
    Date d2 = d1.addMonths( 46 );
    Date expected( 4019, 11, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21920507_neg46, Date)
{
    Date d1( 2192, 5, 7 );
    Date d2 = d1.addMonths( -46 );
    Date expected( 2188, 7, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_43650121_47, Date)
{
    Date d1( 4365, 1, 21 );
    Date d2 = d1.addMonths( 47 );
    Date expected( 4368, 12, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_83390331_neg47, Date)
{
    Date d1( 8339, 3, 31 );
    Date d2 = d1.addMonths( -47 );
    Date expected( 8335, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_59911226_48, Date)
{
    Date d1( 5991, 12, 26 );
    Date d2 = d1.addMonths( 48 );
    Date expected( 5995, 12, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_72850219_neg48, Date)
{
    Date d1( 7285, 2, 19 );
    Date d2 = d1.addMonths( -48 );
    Date expected( 7281, 2, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_31290422_49, Date)
{
    Date d1( 3129, 4, 22 );
    Date d2 = d1.addMonths( 49 );
    Date expected( 3133, 5, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45330802_neg49, Date)
{
    Date d1( 4533, 8, 2 );
    Date d2 = d1.addMonths( -49 );
    Date expected( 4529, 7, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_23570119_50, Date)
{
    Date d1( 2357, 1, 19 );
    Date d2 = d1.addMonths( 50 );
    Date expected( 2361, 3, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53090111_neg50, Date)
{
    Date d1( 5309, 1, 11 );
    Date d2 = d1.addMonths( -50 );
    Date expected( 5304, 11, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_75210929_51, Date)
{
    Date d1( 7521, 9, 29 );
    Date d2 = d1.addMonths( 51 );
    Date expected( 7525, 12, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_98670208_neg51, Date)
{
    Date d1( 9867, 2, 8 );
    Date d2 = d1.addMonths( -51 );
    Date expected( 9862, 11, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51100402_52, Date)
{
    Date d1( 5110, 4, 2 );
    Date d2 = d1.addMonths( 52 );
    Date expected( 5114, 8, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_43620617_neg52, Date)
{
    Date d1( 4362, 6, 17 );
    Date d2 = d1.addMonths( -52 );
    Date expected( 4358, 2, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82550821_53, Date)
{
    Date d1( 8255, 8, 21 );
    Date d2 = d1.addMonths( 53 );
    Date expected( 8260, 1, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66150514_neg53, Date)
{
    Date d1( 6615, 5, 14 );
    Date d2 = d1.addMonths( -53 );
    Date expected( 6610, 12, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29910917_54, Date)
{
    Date d1( 2991, 9, 17 );
    Date d2 = d1.addMonths( 54 );
    Date expected( 2996, 3, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96021031_neg54, Date)
{
    Date d1( 9602, 10, 31 );
    Date d2 = d1.addMonths( -54 );
    Date expected( 9598, 4, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_55710820_55, Date)
{
    Date d1( 5571, 8, 20 );
    Date d2 = d1.addMonths( 55 );
    Date expected( 5576, 3, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_99220604_neg55, Date)
{
    Date d1( 9922, 6, 4 );
    Date d2 = d1.addMonths( -55 );
    Date expected( 9917, 11, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_84080204_56, Date)
{
    Date d1( 8408, 2, 4 );
    Date d2 = d1.addMonths( 56 );
    Date expected( 8412, 10, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26790414_neg56, Date)
{
    Date d1( 2679, 4, 14 );
    Date d2 = d1.addMonths( -56 );
    Date expected( 2674, 8, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91950821_57, Date)
{
    Date d1( 9195, 8, 21 );
    Date d2 = d1.addMonths( 57 );
    Date expected( 9200, 5, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22681212_neg57, Date)
{
    Date d1( 2268, 12, 12 );
    Date d2 = d1.addMonths( -57 );
    Date expected( 2264, 3, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_49151229_58, Date)
{
    Date d1( 4915, 12, 29 );
    Date d2 = d1.addMonths( 58 );
    Date expected( 4920, 10, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_30750611_neg58, Date)
{
    Date d1( 3075, 6, 11 );
    Date d2 = d1.addMonths( -58 );
    Date expected( 3070, 8, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81591221_59, Date)
{
    Date d1( 8159, 12, 21 );
    Date d2 = d1.addMonths( 59 );
    Date expected( 8164, 11, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_99500222_neg59, Date)
{
    Date d1( 9950, 2, 22 );
    Date d2 = d1.addMonths( -59 );
    Date expected( 9945, 3, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_26191020_60, Date)
{
    Date d1( 2619, 10, 20 );
    Date d2 = d1.addMonths( 60 );
    Date expected( 2624, 10, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24950909_neg60, Date)
{
    Date d1( 2495, 9, 9 );
    Date d2 = d1.addMonths( -60 );
    Date expected( 2490, 9, 9 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_87910327_61, Date)
{
    Date d1( 8791, 3, 27 );
    Date d2 = d1.addMonths( 61 );
    Date expected( 8796, 4, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40191120_neg61, Date)
{
    Date d1( 4019, 11, 20 );
    Date d2 = d1.addMonths( -61 );
    Date expected( 4014, 10, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_95420503_62, Date)
{
    Date d1( 9542, 5, 3 );
    Date d2 = d1.addMonths( 62 );
    Date expected( 9547, 7, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_48901205_neg62, Date)
{
    Date d1( 4890, 12, 5 );
    Date d2 = d1.addMonths( -62 );
    Date expected( 4885, 10, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82490624_63, Date)
{
    Date d1( 8249, 6, 24 );
    Date d2 = d1.addMonths( 63 );
    Date expected( 8254, 9, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_46911118_neg63, Date)
{
    Date d1( 4691, 11, 18 );
    Date d2 = d1.addMonths( -63 );
    Date expected( 4686, 8, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28440928_64, Date)
{
    Date d1( 2844, 9, 28 );
    Date d2 = d1.addMonths( 64 );
    Date expected( 2850, 1, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_21120924_neg64, Date)
{
    Date d1( 2112, 9, 24 );
    Date d2 = d1.addMonths( -64 );
    Date expected( 2107, 5, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_94960406_65, Date)
{
    Date d1( 9496, 4, 6 );
    Date d2 = d1.addMonths( 65 );
    Date expected( 9501, 9, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_88460202_neg65, Date)
{
    Date d1( 8846, 2, 2 );
    Date d2 = d1.addMonths( -65 );
    Date expected( 8840, 9, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97060210_66, Date)
{
    Date d1( 9706, 2, 10 );
    Date d2 = d1.addMonths( 66 );
    Date expected( 9711, 8, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91001222_neg66, Date)
{
    Date d1( 9100, 12, 22 );
    Date d2 = d1.addMonths( -66 );
    Date expected( 9095, 6, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_62070119_67, Date)
{
    Date d1( 6207, 1, 19 );
    Date d2 = d1.addMonths( 67 );
    Date expected( 6212, 8, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_85330503_neg67, Date)
{
    Date d1( 8533, 5, 3 );
    Date d2 = d1.addMonths( -67 );
    Date expected( 8527, 10, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_53220811_68, Date)
{
    Date d1( 5322, 8, 11 );
    Date d2 = d1.addMonths( 68 );
    Date expected( 5328, 4, 11 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24910406_neg68, Date)
{
    Date d1( 2491, 4, 6 );
    Date d2 = d1.addMonths( -68 );
    Date expected( 2485, 8, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_96111014_69, Date)
{
    Date d1( 9611, 10, 14 );
    Date d2 = d1.addMonths( 69 );
    Date expected( 9617, 7, 14 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90750119_neg69, Date)
{
    Date d1( 9075, 1, 19 );
    Date d2 = d1.addMonths( -69 );
    Date expected( 9069, 4, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_91520227_70, Date)
{
    Date d1( 9152, 2, 27 );
    Date d2 = d1.addMonths( 70 );
    Date expected( 9157, 12, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_45360406_neg70, Date)
{
    Date d1( 4536, 4, 6 );
    Date d2 = d1.addMonths( -70 );
    Date expected( 4530, 6, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52700831_71, Date)
{
    Date d1( 5270, 8, 31 );
    Date d2 = d1.addMonths( 71 );
    Date expected( 5276, 7, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60900724_neg71, Date)
{
    Date d1( 6090, 7, 24 );
    Date d2 = d1.addMonths( -71 );
    Date expected( 6084, 8, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_44570616_72, Date)
{
    Date d1( 4457, 6, 16 );
    Date d2 = d1.addMonths( 72 );
    Date expected( 4463, 6, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27500827_neg72, Date)
{
    Date d1( 2750, 8, 27 );
    Date d2 = d1.addMonths( -72 );
    Date expected( 2744, 8, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_99191123_73, Date)
{
    Date d1( 9919, 11, 23 );
    Date d2 = d1.addMonths( 73 );
    Date expected( 9925, 12, 23 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_34280322_neg73, Date)
{
    Date d1( 3428, 3, 22 );
    Date d2 = d1.addMonths( -73 );
    Date expected( 3422, 2, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_60210116_74, Date)
{
    Date d1( 6021, 1, 16 );
    Date d2 = d1.addMonths( 74 );
    Date expected( 6027, 3, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_48000428_neg74, Date)
{
    Date d1( 4800, 4, 28 );
    Date d2 = d1.addMonths( -74 );
    Date expected( 4794, 2, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_27051116_75, Date)
{
    Date d1( 2705, 11, 16 );
    Date d2 = d1.addMonths( 75 );
    Date expected( 2712, 2, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_47610510_neg75, Date)
{
    Date d1( 4761, 5, 10 );
    Date d2 = d1.addMonths( -75 );
    Date expected( 4755, 2, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32841113_76, Date)
{
    Date d1( 3284, 11, 13 );
    Date d2 = d1.addMonths( 76 );
    Date expected( 3291, 3, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_51271227_neg76, Date)
{
    Date d1( 5127, 12, 27 );
    Date d2 = d1.addMonths( -76 );
    Date expected( 5121, 8, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24360701_77, Date)
{
    Date d1( 2436, 7, 1 );
    Date d2 = d1.addMonths( 77 );
    Date expected( 2442, 12, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_39491017_neg77, Date)
{
    Date d1( 3949, 10, 17 );
    Date d2 = d1.addMonths( -77 );
    Date expected( 3943, 5, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_38450916_78, Date)
{
    Date d1( 3845, 9, 16 );
    Date d2 = d1.addMonths( 78 );
    Date expected( 3852, 3, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_83790326_neg78, Date)
{
    Date d1( 8379, 3, 26 );
    Date d2 = d1.addMonths( -78 );
    Date expected( 8372, 9, 26 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_79480908_79, Date)
{
    Date d1( 7948, 9, 8 );
    Date d2 = d1.addMonths( 79 );
    Date expected( 7955, 4, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_59301013_neg79, Date)
{
    Date d1( 5930, 10, 13 );
    Date d2 = d1.addMonths( -79 );
    Date expected( 5924, 3, 13 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90440302_80, Date)
{
    Date d1( 9044, 3, 2 );
    Date d2 = d1.addMonths( 80 );
    Date expected( 9050, 11, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_24091024_neg80, Date)
{
    Date d1( 2409, 10, 24 );
    Date d2 = d1.addMonths( -80 );
    Date expected( 2403, 2, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_25391222_81, Date)
{
    Date d1( 2539, 12, 22 );
    Date d2 = d1.addMonths( 81 );
    Date expected( 2546, 9, 22 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_52320205_neg81, Date)
{
    Date d1( 5232, 2, 5 );
    Date d2 = d1.addMonths( -81 );
    Date expected( 5225, 5, 5 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_80300928_82, Date)
{
    Date d1( 8030, 9, 28 );
    Date d2 = d1.addMonths( 82 );
    Date expected( 8037, 7, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_80730820_neg82, Date)
{
    Date d1( 8073, 8, 20 );
    Date d2 = d1.addMonths( -82 );
    Date expected( 8066, 10, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_28220702_83, Date)
{
    Date d1( 2822, 7, 2 );
    Date d2 = d1.addMonths( 83 );
    Date expected( 2829, 6, 2 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97910829_neg83, Date)
{
    Date d1( 9791, 8, 29 );
    Date d2 = d1.addMonths( -83 );
    Date expected( 9784, 9, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_58460801_84, Date)
{
    Date d1( 5846, 8, 1 );
    Date d2 = d1.addMonths( 84 );
    Date expected( 5853, 8, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_40120217_neg84, Date)
{
    Date d1( 4012, 2, 17 );
    Date d2 = d1.addMonths( -84 );
    Date expected( 4005, 2, 17 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73630204_85, Date)
{
    Date d1( 7363, 2, 4 );
    Date d2 = d1.addMonths( 85 );
    Date expected( 7370, 3, 4 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_37400507_neg85, Date)
{
    Date d1( 3740, 5, 7 );
    Date d2 = d1.addMonths( -85 );
    Date expected( 3733, 4, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_43830112_86, Date)
{
    Date d1( 4383, 1, 12 );
    Date d2 = d1.addMonths( 86 );
    Date expected( 4390, 3, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_44791203_neg86, Date)
{
    Date d1( 4479, 12, 3 );
    Date d2 = d1.addMonths( -86 );
    Date expected( 4472, 10, 3 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_66190721_87, Date)
{
    Date d1( 6619, 7, 21 );
    Date d2 = d1.addMonths( 87 );
    Date expected( 6626, 10, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_22000708_neg87, Date)
{
    Date d1( 2200, 7, 8 );
    Date d2 = d1.addMonths( -87 );
    Date expected( 2193, 4, 8 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_47870518_88, Date)
{
    Date d1( 4787, 5, 18 );
    Date d2 = d1.addMonths( 88 );
    Date expected( 4794, 9, 18 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_41171127_neg88, Date)
{
    Date d1( 4117, 11, 27 );
    Date d2 = d1.addMonths( -88 );
    Date expected( 4110, 7, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81740324_89, Date)
{
    Date d1( 8174, 3, 24 );
    Date d2 = d1.addMonths( 89 );
    Date expected( 8181, 8, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97991027_neg89, Date)
{
    Date d1( 9799, 10, 27 );
    Date d2 = d1.addMonths( -89 );
    Date expected( 9792, 5, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_38911029_90, Date)
{
    Date d1( 3891, 10, 29 );
    Date d2 = d1.addMonths( 90 );
    Date expected( 3899, 4, 29 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_29160228_neg90, Date)
{
    Date d1( 2916, 2, 28 );
    Date d2 = d1.addMonths( -90 );
    Date expected( 2908, 8, 28 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_82960116_91, Date)
{
    Date d1( 8296, 1, 16 );
    Date d2 = d1.addMonths( 91 );
    Date expected( 8303, 8, 16 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_90101206_neg91, Date)
{
    Date d1( 9010, 12, 6 );
    Date d2 = d1.addMonths( -91 );
    Date expected( 9003, 5, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_83510125_92, Date)
{
    Date d1( 8351, 1, 25 );
    Date d2 = d1.addMonths( 92 );
    Date expected( 8358, 9, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_43440227_neg92, Date)
{
    Date d1( 4344, 2, 27 );
    Date d2 = d1.addMonths( -92 );
    Date expected( 4336, 6, 27 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_98640712_93, Date)
{
    Date d1( 9864, 7, 12 );
    Date d2 = d1.addMonths( 93 );
    Date expected( 9872, 4, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97370620_neg93, Date)
{
    Date d1( 9737, 6, 20 );
    Date d2 = d1.addMonths( -93 );
    Date expected( 9729, 9, 20 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_38330615_94, Date)
{
    Date d1( 3833, 6, 15 );
    Date d2 = d1.addMonths( 94 );
    Date expected( 3841, 4, 15 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_42911125_neg94, Date)
{
    Date d1( 4291, 11, 25 );
    Date d2 = d1.addMonths( -94 );
    Date expected( 4284, 1, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_76000607_95, Date)
{
    Date d1( 7600, 6, 7 );
    Date d2 = d1.addMonths( 95 );
    Date expected( 7608, 5, 7 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_84020221_neg95, Date)
{
    Date d1( 8402, 2, 21 );
    Date d2 = d1.addMonths( -95 );
    Date expected( 8394, 3, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_54361206_96, Date)
{
    Date d1( 5436, 12, 6 );
    Date d2 = d1.addMonths( 96 );
    Date expected( 5444, 12, 6 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_68420925_neg96, Date)
{
    Date d1( 6842, 9, 25 );
    Date d2 = d1.addMonths( -96 );
    Date expected( 6834, 9, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_55420519_97, Date)
{
    Date d1( 5542, 5, 19 );
    Date d2 = d1.addMonths( 97 );
    Date expected( 5550, 6, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_81640421_neg97, Date)
{
    Date d1( 8164, 4, 21 );
    Date d2 = d1.addMonths( -97 );
    Date expected( 8156, 3, 21 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_32740225_98, Date)
{
    Date d1( 3274, 2, 25 );
    Date d2 = d1.addMonths( 98 );
    Date expected( 3282, 4, 25 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_42520401_neg98, Date)
{
    Date d1( 4252, 4, 1 );
    Date d2 = d1.addMonths( -98 );
    Date expected( 4244, 2, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_73750410_99, Date)
{
    Date d1( 7375, 4, 10 );
    Date d2 = d1.addMonths( 99 );
    Date expected( 7383, 7, 10 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_97931230_neg99, Date)
{
    Date d1( 9793, 12, 30 );
    Date d2 = d1.addMonths( -99 );
    Date expected( 9785, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_65711024_100, Date)
{
    Date d1( 6571, 10, 24 );
    Date d2 = d1.addMonths( 100 );
    Date expected( 6580, 2, 24 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(add_day_65251219_neg100, Date)
{
    Date d1( 6525, 12, 19 );
    Date d2 = d1.addMonths( -100 );
    Date expected( 6517, 8, 19 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
	actual = d2;
	CHECK_EQUAL( expected, actual )
}
TEST(isLeapYear_44600910, Date)
{
    Date d( 4460, 9, 10 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_48541109, Date)
{
    Date d( 4854, 11, 9 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_34570111, Date)
{
    Date d( 3457, 1, 11 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_54630605, Date)
{
    Date d( 5463, 6, 5 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_03370916, Date)
{
    Date d( 337, 9, 16 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_98841102, Date)
{
    Date d( 9884, 11, 2 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_68450517, Date)
{
    Date d( 6845, 5, 17 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_06030729, Date)
{
    Date d( 603, 7, 29 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_07770419, Date)
{
    Date d( 777, 4, 19 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_82591205, Date)
{
    Date d( 8259, 12, 5 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_72681020, Date)
{
    Date d( 7268, 10, 20 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_68970616, Date)
{
    Date d( 6897, 6, 16 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_83641008, Date)
{
    Date d( 8364, 10, 8 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}


TEST(isLeapYear_17560920, Date)
{
    Date d( 1756, 9, 20 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_18130224, Date)
{
    Date d( 1813, 2, 24 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_18640413, Date)
{
    Date d( 1864, 4, 13 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_17750217, Date)
{
    Date d( 1775, 2, 17 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_18980519, Date)
{
    Date d( 1898, 5, 19 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_17911120, Date)
{
    Date d( 1791, 11, 20 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_18850622, Date)
{
    Date d( 1885, 6, 22 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_18921221, Date)
{
    Date d( 1892, 12, 21 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}


TEST(isLeapYear_19800705, Date)
{
    Date d( 1980, 7, 5 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20760716, Date)
{
    Date d( 2076, 7, 16 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20730415, Date)
{
    Date d( 2073, 4, 15 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20070223, Date)
{
    Date d( 2007, 2, 23 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20740407, Date)
{
    Date d( 2074, 4, 7 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20160823, Date)
{
    Date d( 2016, 8, 23 );
    bool expected = true;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}

TEST(isLeapYear_20180901, Date)
{
    Date d( 2018, 9, 1 );
    bool expected = false;
    bool actual = d.isLeapYear();
    CHECK_EQUAL( expected, actual )
}
TEST(static_isLeapYear_20050519, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2005 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20400216, Date)
{
    bool expected = true;
    Date date; bool actual = date.isLeapYear( 2040 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20220523, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2022 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_19850316, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 1985 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20691008, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2069 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20591221, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2059 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20621108, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2062 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20600108, Date)
{
    bool expected = true;
    Date date; bool actual = date.isLeapYear( 2060 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20540410, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2054 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20201006, Date)
{
    bool expected = true;
    Date date; bool actual = date.isLeapYear( 2020 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20320410, Date)
{
    bool expected = true;
    Date date; bool actual = date.isLeapYear( 2032 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20260418, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2026 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_20310220, Date)
{
    bool expected = false;
    Date date; bool actual = date.isLeapYear( 2031 );
    CHECK_EQUAL( expected, actual )
}

TEST(static_isLeapYear_19960509, Date)
{
    bool expected = true;
    Date date; bool actual = date.isLeapYear( 1996 );
    CHECK_EQUAL( expected, actual )
}

TEST( parse01_failSmallYear, Date )
{
    Date d;
    CHECK_EQUAL( d.getYear(), 1900 );
    CHECK_EQUAL( d.getMonth(), 1 );
    CHECK_EQUAL( d.getDay(), 1 );
    CHECK( ! d.parse( "200-12-25" ) );
    CHECK_EQUAL( d.getYear(), 1900 );
    CHECK_EQUAL( d.getMonth(), 1 );
    CHECK_EQUAL( d.getDay(), 1 );
}

TEST( parse01_okSmallYear, Date )
{
    Date d( 2001, 2, 27 );
    CHECK_EQUAL( d.getYear(), 2001 );
    CHECK_EQUAL( d.getMonth(), 2 );
    CHECK_EQUAL( d.getDay(), 27 );
    CHECK( d.parse( "0010-12-05" ) );
    CHECK_EQUAL( d.getYear(), 10 );
    CHECK_EQUAL( d.getMonth(), 12 );
    CHECK_EQUAL( d.getDay(), 05 );
}

TEST( parse01_failNormalDate, Date )
{
    Date d( 2005, 3, 15 );
    CHECK_EQUAL( d.getYear(), 2005 );
    CHECK_EQUAL( d.getMonth(), 3 );
    CHECK_EQUAL( d.getDay(), 15 );
    CHECK( ! d.parse( "1996-15-05" ) );
    CHECK_EQUAL( d.getYear(), 2005 );
    CHECK_EQUAL( d.getMonth(), 3 );
    CHECK_EQUAL( d.getDay(), 15 );
}

TEST( parse01_okNormalDate, Date )
{
    Date d( 2013, 12, 1 );
    CHECK_EQUAL( d.getYear(), 2013 );
    CHECK_EQUAL( d.getMonth(), 12 );
    CHECK_EQUAL( d.getDay(), 1 );
    CHECK( d.parse( "2016-02-05" ) );
    CHECK_EQUAL( d.getYear(), 2016 );
    CHECK_EQUAL( d.getMonth(), 02 );
    CHECK_EQUAL( d.getDay(), 05 );
}
TEST(addYears01_outofleap, Date)
{
    Date d1( 2060, 2, 29 );
    Date d2 = d1.addYears( 1 );
    Date expected( 2061, 3, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(addYears02_outofleap, Date)
{
    Date d1( 2040, 2, 29 );
    Date d2 = d1.addYears( -3 );
    Date expected( 2037, 3, 1 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(addYears03_outofrange, Date)
{
    Date d1( 9999, 5, 31 );
    Date d2 = d1.addYears( 1 );
    Date expected( 9999, 5, 31 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(addYears04_outofrange, Date)
{
    Date d1( 1, 2, 12 );
    Date d2 = d1.addYears( -1 );
    Date expected( 1, 2, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(addYears05, Date)
{
    Date d1( 2014, 8, 12 );
    Date d2 = d1.addYears( 31 );
    Date expected( 2045, 8, 12 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}

TEST(addYears06, Date)
{
    Date d1( 1996, 9, 30 );
    Date d2 = d1.addYears( -11 );
    Date expected( 1985, 9, 30 );
    Date actual = d1;
    CHECK_EQUAL( expected, actual )
}
TEST( stream01, Date )
{
    Date object( 2014, 8, 22 );
	std::stringstream ss1;
	toStream( ss1, object );
	std::string expected = "2014-08-22";
	std::string actual = ss1.str();
	CHECK_EQUAL( expected, actual )
	object.setValue( 2014, 2, 1 );
	std::stringstream ss2;
	toStream( ss2, object );
	toStream( ss2, object );
	expected = "2014-02-012014-02-01";
	actual = ss2.str();
	CHECK_EQUAL( expected, actual )
	std::stringstream ss3;
	ss3 << object;
	expected = "2014-02-01";
	actual = ss3.str();
	CHECK_EQUAL( expected, actual )
}

TEST( stream02, Date )
{
	Date object( 2001, 03, 25 );
	std::stringstream ss1;
	toStream( ss1, object );
	std::string expected = "2001-03-25";
	std::string actual = ss1.str();
	CHECK_EQUAL( expected, actual )
	object.setValue( 13, 12, 1 );
	std::stringstream ss2;
	ss2 << object << object;
	expected = "0013-12-010013-12-01";
	actual = ss2.str();
	CHECK_EQUAL( expected, actual )
	std::stringstream ss3;
	ss3 << object;
	expected = "0013-12-01";
	actual = ss3.str();
	CHECK_EQUAL( expected, actual )
}

#endif
