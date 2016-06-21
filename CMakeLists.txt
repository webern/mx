cmake_minimum_required(VERSION 2.8.12.2)
project(Mx)


set(CMAKE_BINARY_DIR "./Build")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SOURCE "./Sourcecode")
file(GLOB_RECURSE SRC_MX_CORE ${SOURCE}/mx/core/*.c* ${SOURCE}/mx/core/*.h*)
file(GLOB_RECURSE SRC_MX_PUGIXML ${SOURCE}/mx/pugixml/*.cpp ${SOURCE}/mx/pugixml/*.hpp)
file(GLOB_RECURSE SRC_MX_UTILITY ${SOURCE}/mx/utility/*.cpp ${SOURCE}/mx/utility/*.h)
file(GLOB_RECURSE SRC_MX_XML ${SOURCE}/mx/xml/*.cpp ${SOURCE}/mx/xml/*.h)

file(GLOB_RECURSE SRC_MX_TEST_CONTROL ${SOURCE}/mxtest/control/*.cpp ${SOURCE}/mxtest/control/*.h)
file(GLOB_RECURSE SRC_MX_TEST_CORE ${SOURCE}/mxtest/core/*.cpp ${SOURCE}/mxtest/core/*.h)
file(GLOB_RECURSE SRC_MX_TEST_UTILITY ${SOURCE}/mxtest/utility/*.cpp ${SOURCE}/mxtest/utility/*.h)
file(GLOB_RECURSE SRC_MX_TEST_XML ${SOURCE}/mxtest/xml/*.cpp ${SOURCE}/mxtest/xml/*.h)
file(GLOB_RECURSE SRC_CPUL ${SOURCE}/cpul/*.cpp ${SOURCE}/cpul/*.h)

include_directories(${SOURCE})

add_compile_options(-std=c++11)

# Mx
add_library(Mx STATIC ${SRC_MX_CORE} ${SRC_MX_PUGIXML} ${SRC_MX_UTILITY} ${SRC_MX_XML})
source_group( "core" FILES ${SRC_MX_CORE} )
source_group( "pugixml" FILES ${SRC_MX_PUGIXML} )
source_group( "utility" FILES ${SRC_MX_UTILITY} )
source_group( "xml" FILES ${SRC_MX_XML} )

# MxTest
add_executable(MxTest ${SRC_MX_TEST_CONTROL} ${SRC_MX_TEST_CORE} ${SRC_MX_TEST_UTILITY} ${SRC_MX_TEST_XML} ${SRC_CPUL})
source_group( "control" FILES ${SRC_MX_TEST_CONTROL} )
source_group( "core" FILES ${SRC_MX_TEST_CORE} )
source_group( "utility" FILES ${SRC_MX_TEST_UTILITY} )
source_group( "xml" FILES ${SRC_MX_TEST_XML} )
source_group( "cpul" FILES ${SRC_CPUL} )
target_link_libraries(MxTest Mx)
