cmake_minimum_required(VERSION 2.8.12.2)
project(Mx)


set(CMAKE_BINARY_DIR "./Build")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SOURCE "./Sourcecode")
file(GLOB_RECURSE SRC_MX_API ${SOURCE}/mx/api/*.cpp ${SOURCE}/mx/api/*.h)
file(GLOB_RECURSE SRC_MX_CORE ${SOURCE}/mx/core/*.c* ${SOURCE}/mx/core/*.h*)
file(GLOB_RECURSE SRC_MX_IMPL ${SOURCE}/mx/impl/*.cpp ${SOURCE}/mx/impl/*.h)
file(GLOB_RECURSE SRC_MX_PUGIXML ${SOURCE}/mx/pugixml/*.cpp ${SOURCE}/mx/pugixml/*.hpp)
file(GLOB_RECURSE SRC_MX_UTILITY ${SOURCE}/mx/utility/*.cpp ${SOURCE}/mx/utility/*.h)
file(GLOB_RECURSE SRC_MX_XML ${SOURCE}/mx/xml/*.cpp ${SOURCE}/mx/xml/*.h)

file(GLOB_RECURSE SRC_MX_TEST_API ${SOURCE}/mxtest/api/*.cpp ${SOURCE}/mxtest/api/*.h)
file(GLOB_RECURSE SRC_MX_TEST_CONTROL ${SOURCE}/mxtest/control/*.cpp ${SOURCE}/mxtest/control/*.h)
file(GLOB_RECURSE SRC_MX_TEST_IMPL ${SOURCE}/mxtest/impl/*.cpp ${SOURCE}/mxtest/impl/*.h)
file(GLOB_RECURSE SRC_MX_TEST_CORE ${SOURCE}/mxtest/core/*.cpp ${SOURCE}/mxtest/core/*.h)
file(GLOB_RECURSE SRC_MX_TEST_IMPORT ${SOURCE}/mxtest/import/*.cpp ${SOURCE}/mxtest/import/*.h)
file(GLOB_RECURSE SRC_MX_TEST_UTILITY ${SOURCE}/mxtest/utility/*.cpp ${SOURCE}/mxtest/utility/*.h)
file(GLOB_RECURSE SRC_MX_TEST_XML ${SOURCE}/mxtest/xml/*.cpp ${SOURCE}/mxtest/xml/*.h)
file(GLOB_RECURSE SRC_CPUL ${SOURCE}/cpul/*.cpp ${SOURCE}/cpul/*.h)

include_directories(${SOURCE})

IF(!MSVC)
    add_compile_options(-std=c++11)
ENDIF()

# Mx
add_library(Mx STATIC ${SRC_MX_API} ${SRC_MX_CORE} ${SRC_MX_IMPL} ${SRC_MX_PUGIXML} ${SRC_MX_UTILITY} ${SRC_MX_XML})
source_group( "api" FILES ${SRC_MX_API} )
source_group( "core" FILES ${SRC_MX_CORE} )
source_group( "impl" FILES ${SRC_MX_IMPL} )
source_group( "pugixml" FILES ${SRC_MX_PUGIXML} )
source_group( "utility" FILES ${SRC_MX_UTILITY} )
source_group( "xml" FILES ${SRC_MX_XML} )

# MxTest
add_executable(MxTest ${SRC_MX_TEST_API} ${SRC_MX_TEST_CONTROL} ${SRC_MX_TEST_CORE} ${SRC_MX_TEST_IMPL} ${SRC_MX_TEST_IMPORT} ${SRC_MX_TEST_UTILITY} ${SRC_MX_TEST_XML} ${SRC_CPUL})
source_group( "api" FILES ${SRC_MX_TEST_API} )
source_group( "control" FILES ${SRC_MX_TEST_CONTROL} )
source_group( "core" FILES ${SRC_MX_TEST_CORE} )
source_group( "impl" FILES ${SRC_MX_TEST_IMPL} )
source_group( "import" FILES ${SRC_MX_TEST_IMPORT} )
source_group( "utility" FILES ${SRC_MX_TEST_UTILITY} )
source_group( "xml" FILES ${SRC_MX_TEST_XML} )
source_group( "cpul" FILES ${SRC_CPUL} )
target_link_libraries(MxTest Mx)


#macro(add_cppunit_test target)
#  add_library(${target} SHARED ${ARGN})
#  set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/FIstdafx.h /I\"$(VCInstallDir)UnitTest/include\"")
#  set_target_properties(${target} PROPERTIES LINK_FLAGS "-LIBPATH:\"$(VCInstallDir)UnitTest/lib\"")
#  set_target_properties(${target} PROPERTIES PREFIX ${TEST_NAMESPACE} IMPORT_PREFIX ${TEST_NAMESPACE} ${PROPERTIES_VS_SCC})
#  include_directories(${CMAKE_SOURCE_DIR}/Common/Test)
#endmacro(add_cppunit_test)


# MxTestVstf - doesn't work, hard to find an example
#set(PROPERTIES_VS_SCC VS_SCC_AUXPATH SAK VS_SCC_LOCALPATH SAK VS_SCC_PROJECTNAME SAK VS_SCC_PROVIDER SAK)
#add_library(MxTestVstf STATIC ${SRC_MX_TEST_CONTROL} ${SRC_MX_TEST_CORE} ${SRC_MX_TEST_UTILITY} ${SRC_MX_TEST_XML} ${SRC_CPUL})
#source_group( "control" FILES ${SRC_MX_TEST_CONTROL} )
#source_group( "core" FILES ${SRC_MX_TEST_CORE} )
#source_group( "utility" FILES ${SRC_MX_TEST_UTILITY} )
#source_group( "xml" FILES ${SRC_MX_TEST_XML} )
#source_group( "cpul" FILES ${SRC_CPUL} )
#set_target_properties(MxTestVstf PROPERTIES COMPILE_FLAGS "/I\"$(VCInstallDir)UnitTest/include\"")
#set_target_properties(MxTestVstf PROPERTIES LINK_FLAGS "-LIBPATH:\"$(VCInstallDir)UnitTest/lib\"")
#target_link_libraries(MxTest Mx)