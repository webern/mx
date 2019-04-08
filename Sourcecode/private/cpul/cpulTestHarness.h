#pragma once

#include "cpul/cpulDefines.h"

#ifdef USE_VS_TEST

    #include "cpul/cpulVsTest.h"

#else

    #include "cpul/cpulTest.h"
    #include "cpul/cpulTestResult.h"
    #include "cpul/cpulFailure.h"
    #include "cpul/cpulTestRegistry.h"

#endif

