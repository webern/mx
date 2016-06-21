#pragma once
#include <cmath>

#define T_END };  \
      \


#define TEST( functionName, className ) \
    \
using namespace Microsoft::VisualStudio::TestTools::UnitTesting; \
    \
[TestClass] \
    \
public ref class Test##className##functionName \
{ \
public: \
\
    [TestMethod] \
	[TestProperty("Project","Lyre")] \
	[TestProperty("Class",#className)] \
    void className##_##functionName####() \

#undef CHECK
#ifndef CHECK
#define CHECK(statementToCheck) \
    Assert::IsTrue(!(!(##statementToCheck))); 
#endif

#ifndef CHECK_EQUAL
#define CHECK_EQUAL( expected, actual ) \
    Assert::IsTrue( (##expected ) == (##actual ) ); 
#endif

#define CHECK_DOUBLES_EQUAL(expected,actual,epsilon) \
    CHECK( std::abs((##expected)-(##actual))<(##epsilon) )

#define CHECK_FAIL( text ) \
    CHECK( false )
