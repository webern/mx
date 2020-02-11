#define CATCH_CONFIG_MAIN
#include "cpul/catch.h"
#undef CATCH_CONFIG_MAIN

//TODO is there a main function for VSCode Test Framework?
//int main(int argc, const char * argv[])
//{
//#ifdef MX_REPO_ROOT_PATH
//    std::cout << MX_REPO_ROOT_PATH << std::endl;
//#endif
//#ifdef MX_BINARY_OUTPUT_PATH
//    std::cout << MX_BINARY_OUTPUT_PATH << std::endl;
//#endif
//#ifdef DEBUG
//    std::cout << "Debug Mode" << std::endl;
//#endif
//    UNUSED_PARAMETER( argc )
//    UNUSED_PARAMETER( argv )
//    cpul::TestTimer timer;
//    const auto failures = cpulRun( true );
//    timer.report( "Total runtime for all tests" );
//
//    if( failures != 0 )
//    {
//        return 1;
//    }
//
//    return 0;
//}
