#include "cpul/cpulRun.h"


int cpulRun( bool runTests, bool holdConsole )
{
    UNUSED_PARAMETER( runTests )
    UNUSED_PARAMETER( holdConsole )
    int failures = 0;
    
#ifndef USE_VS_TEST
    if ( runTests )
    {
        // random number generator used in some tests
        srand(::time_t(NULL));
        
        TestResult tr;
        failures = TestRegistry::runAllTests(tr);
        
        // force console screen to hold
        if ( holdConsole )
        {
            char ch;
            std::cin >> ch;
        }
    }
#endif
    
    return failures;
}
