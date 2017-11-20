#include "cpul/cpulTest.h"
#include "cpul/cpulTestRegistry.h"
#include "cpul/cpulTestResult.h"

Test::Test (const std::string& testName,
            std::string fileName,
            long lineNumber)
:
    myFileName (fileName),
    myLineNumber (lineNumber),
    myName (testName)
{
    TestRegistry::addTest (this);
}

Test::~Test()
{
}

const std::string& Test::getFileName() const
{
    return myFileName;
}

const long& Test::getLineNumber() const
{
    return myLineNumber;
}

const std::string& Test::getName() const
{
    return myName;
}

void Test::run (TestResult& result)
{
//    if ( getName() == "ApiRoundTripTest" &&
//        getFileName().find( "ApiChordSimpleTest.cpp" ) != std::string::npos )
//    {
        runTest (result);
        result.testWasRun();
//    }
}

