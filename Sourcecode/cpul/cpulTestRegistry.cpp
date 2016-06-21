#include "cpul/cpulTest.h"
#include "cpul/cpulTestResult.h"
#include "cpul/cpulTestRegistry.h"
#include <iostream>

void TestRegistry::addTest (Test *test) 
{
    instance ().add (test);
}

void TestRegistry::runAllTests (TestResult& result) 
{
    instance ().run (result);
}

TestRegistry& TestRegistry::instance () {
    static TestRegistry registry;
    return registry;
}

void TestRegistry::add (Test *test) {
    tests.push_back (test);
}

void TestRegistry::run (TestResult& result) {
    int testCount = 0;
    int errorCount = 0;
    result.startTests ();
    std::vector<Test *>::iterator it;
    for (it = tests.begin (); it != tests.end (); ++it) {
        ++testCount;
        try {
            (*it)->run (result);
        }
        catch (std::exception &e) {
            ++errorCount;
            std::cout << std::endl
                      << (*it)->getFileName()
                      << "(" << (*it)->getLineNumber() << ") : "
                      << "Error: exception "
                      << "'" << e.what() << "'"
                      << " thrown in "
                      << (*it)->getName()
                      << std::endl;
        }
        catch (...) {
            ++errorCount;
            std::cout << std::endl
                      << (*it)->getFileName()
                      << "(" << (*it)->getLineNumber() << ") : "
                      << "Error: unknown exception thrown in "
                      << (*it)->getName()
                      << std::endl;
        }
    }
    result.endTests ();
    int failureCount = result.getFailureCount();
    if (failureCount > 0 || errorCount > 0) std::cout << std::endl;
    std::cout << testCount << " tests, "
              << result.getFailureCount() << " failures, "
              << errorCount << " errors" << std::endl;
}
