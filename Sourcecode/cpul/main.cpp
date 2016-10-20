#include <iostream>
#include "cpul/cpulRun.h"
#include "cpul/cpulTestTimer.h"
#include "mxtest/api/RoundTrip.h"


int main(int argc, const char * argv[])
{
    //MxRtiTest::rtiMain();
    UNUSED_PARAMETER( argc )
    UNUSED_PARAMETER( argv )
    cpul::TestTimer timer;
    cpulRun( true );
    MxTest::roundTrip();
    timer.report( "Total runtime for all tests" );
    return 0;
}

