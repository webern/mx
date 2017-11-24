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

void Test::run (TestResult& result )
{
    if ( getDoRunTest() )
    {
        runTest (result);
        result.testWasRun();
    }
}

/// List test name and filename of the tests that you want to
/// run.  If these strings are contained in the actual test/file
/// name, then the test will be run. If the vector is empty,
/// then all tests will be run.
const std::vector<TestIdentifier> Test::runOnlyTheseTests =
{
//    { "roundTripViolaDynamicWrongTime", "FreezingRoundTrip.cpp" },
//    { "roundTripOutput", "FreezingRoundTrip.cpp" },
//    { "roundTripChordDirFile", "FreezingRoundTrip.cpp" },
//    { "chordDirectionPlacement", "FreezingRoundTrip.cpp" },
};

bool Test::getDoRunTest() const
{
    bool doRunTest = runOnlyTheseTests.empty();

    if( !doRunTest )
    {
        const auto findLambda = [&]( const TestIdentifier& inTi )
        {
            const bool isName = getName().find( inTi.name ) != std::string::npos;
            const bool isFile = getFileName().find( inTi.filename ) != std::string::npos;
            return isName && isFile;
        };

        const auto foundIter = std::find_if( std::cbegin( runOnlyTheseTests ), std::cend( runOnlyTheseTests ), findLambda );
        const bool isFound = foundIter != std::cend( runOnlyTheseTests );
        if( isFound )
        {
            doRunTest = true;
        }
        else
        {
            doRunTest = false;
        }
    }

    return doRunTest;
}

