#pragma once

#include "cpul/cpulDefines.h"

#define MX_USE_CATCH2

#ifdef USE_VS_TEST
    #include "cpul/cpulVsTest.h"
#elif defined(MX_USE_CATCH2)
    #include "cpul/catch.h"
    #include "cpul/catchDefines.h"
#else
    #include "cpul/cpulTest.h"
    #include "cpul/cpulTestResult.h"
    #include "cpul/cpulFailure.h"
    #include "cpul/cpulTestRegistry.h"
#endif

