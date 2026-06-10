cmake_minimum_required(VERSION 3.13)
project(mx-corert-c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(LibXml2 REQUIRED)

# Repo root is passed in at configure time so the test runner can find data/.
if(NOT DEFINED MX_REPO_ROOT)
    get_filename_component(MX_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
endif()

# The generated MusicXML model (mx/ is the generator's output directory; the
# manifest is emitted alongside the sources so the build never globs).
include(mx/sources.cmake)
add_library(mx-c STATIC ${MX_GENERATED_SOURCES})
target_include_directories(mx-c PUBLIC mx ${LIBXML2_INCLUDE_DIRS})
target_link_libraries(mx-c ${LIBXML2_LIBRARIES})

add_executable(corert-c
    src/main.c
    src/discover.c
    src/fixer.c
    src/normalize.c
    src/compare.c
    src/roundtrip.c)

target_include_directories(corert-c PRIVATE ${LIBXML2_INCLUDE_DIRS} src)
target_link_libraries(corert-c mx-c ${LIBXML2_LIBRARIES})
target_compile_definitions(corert-c PRIVATE MX_REPO_ROOT="${MX_REPO_ROOT}")

# Generated-value-type smoke test: wire -> typed -> wire across the leniency
# policies (mirrors the Go target's TestValueSmoke).
add_executable(values-smoke src/values_smoke.c)
target_link_libraries(values-smoke mx-c)
