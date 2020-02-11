#pragma once

#include "cpul/catch.h"
#include <cmath>

#define TEST( a, b ) TEST_CASE( #a, #b )

#define CHECK_EQUAL( expected, actual ) CHECK( (expected) == (actual) );

#define CHECK_DOUBLES_EQUAL( expected, actual, epsilon ) \
  CHECK( \
    std::abs( static_cast<long double>( expected ) - static_cast<long double>( actual ) ) < ( static_cast<long double>( epsilon ) \
  ) \
);

#define CHECK_WITH_MESSAGE( trueStatement, msg ) \
{ \
    if(!(#trueStatement)) \
    { \
        FAIL( (#msg) ); \
    } \
}

#define T_END