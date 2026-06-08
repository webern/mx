cmake_minimum_required(VERSION 3.13)
project(mx-corert-c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(LibXml2 REQUIRED)

# Repo root is passed in at configure time so the test runner can find data/.
if(NOT DEFINED MX_REPO_ROOT)
    get_filename_component(MX_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
endif()

add_executable(corert-c
    src/main.c
    src/discover.c
    src/fixer.c
    src/normalize.c
    src/compare.c
    src/roundtrip.c
    src/stub.c)

target_include_directories(corert-c PRIVATE ${LIBXML2_INCLUDE_DIRS} src)
target_link_libraries(corert-c ${LIBXML2_LIBRARIES})
target_compile_definitions(corert-c PRIVATE MX_REPO_ROOT="${MX_REPO_ROOT}")
