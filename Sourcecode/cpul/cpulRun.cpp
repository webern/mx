#include "cpul/cpulRun.h"


void cpulRun( bool runTests, bool holdConsole )
{
    UNUSED_PARAMETER( runTests )
    UNUSED_PARAMETER( holdConsole )

#ifndef USE_VS_TEST
    if ( runTests )
    {
        // random number generator used in some tests
        srand(::time_t(NULL));
        
        TestResult tr;
        TestRegistry::runAllTests(tr);
        
        // force console screen to hold
        if ( holdConsole )
        {
            char ch;
            std::cin >> ch;
        }
    }
#endif
}