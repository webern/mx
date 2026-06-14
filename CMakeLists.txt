cmake_minimum_required(VERSION 3.13)
project(mx CXX)

# The mx::core C++ typed model: pugixml + the generated src/private/mx/core
# (emitted by `python3 -m gen gen/cpp/config.toml`, committed) + its
# hand-written runtime, plus the corert/unit/validate test binaries.
# See AGENTS.md and docs/ai/design/mx-core-plan.md.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

option(MX_CORE_DEV "Build the core roundtrip (corert) test binary against the regenerated mx/core." OFF)

option(MX_API "Build the mx::api/mx::impl product library and its tests (Phase 2)." ON)

option(MX_COVERAGE "Instrument the build for gcov coverage (GCC/Clang only)." OFF)

# Coverage instrumentation. add_compile_options/add_link_options affect every
# target created after this point, so a core-dev + coverage configure only
# instruments the core-dev TUs; nothing else is compiled in that mode. gcovr
# then filters the report to src/private/mx/core/.
if(MX_COVERAGE)
    if(MSVC)
        message(FATAL_ERROR "MX_COVERAGE is not supported with MSVC")
    endif()
    message(STATUS "${PROJECT_NAME}: MX_COVERAGE=ON: building with --coverage")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

set(PRIVATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/private")
set(PUBLIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/include")

# pugixml: the XML layer the generated mx::core consumes directly (plan §2.1).
# Vendored at src/private/pugixml (upstream 1.15, stock pugiconfig).
add_library(pugixml STATIC ${PRIVATE_DIR}/pugixml/pugixml.cpp)
target_include_directories(pugixml PUBLIC ${PRIVATE_DIR})

# mx_core: the generated C++ typed model plus its hand-written runtime
# (Decimal, Result, Error, Lexical, Token, NameToken, OneOrMore, Xml). The
# generated per-type sources live in src/private/mx/core/generated/ (their
# own directory, so generator-owned code is recognizable at a glance) and
# arrive via sources.cmake (emitted by `python3 -m gen gen/cpp/config.toml`).
set(MX_CORE_RUNTIME_SOURCES
    ${PRIVATE_DIR}/mx/core/Decimal.cpp
    ${PRIVATE_DIR}/mx/core/Decimal.h
    ${PRIVATE_DIR}/mx/core/Error.h
    ${PRIVATE_DIR}/mx/core/Lexical.cpp
    ${PRIVATE_DIR}/mx/core/Lexical.h
    ${PRIVATE_DIR}/mx/core/NameToken.cpp
    ${PRIVATE_DIR}/mx/core/NameToken.h
    ${PRIVATE_DIR}/mx/core/OneOrMore.h
    ${PRIVATE_DIR}/mx/core/Result.h
    ${PRIVATE_DIR}/mx/core/Token.cpp
    ${PRIVATE_DIR}/mx/core/Token.h
    ${PRIVATE_DIR}/mx/core/Xml.cpp
    ${PRIVATE_DIR}/mx/core/Xml.h)
set(MX_CORE_GENERATED_SOURCES "")
include(${PRIVATE_DIR}/mx/core/generated/sources.cmake OPTIONAL)

# mx::core::gitSha, captured at BUILD time. A configure-time capture would go
# stale the moment HEAD moved, because an IDE rebuild (Xcode/Ninja/Make) never
# re-runs CMake. So the capture is a build step baked into the project: it
# re-checks git every build and rewrites the TU only when the value changed.
# See cmake/GitSha.cmake. mx_core depends on the target so the file exists
# before it compiles; GENERATED lets CMake accept a path that the first
# configure has not produced yet.
find_package(Git QUIET)
set(MX_GIT_SHA_CPP "${CMAKE_BINARY_DIR}/generated/GitSha.cpp")
add_custom_target(mx_git_sha ALL
    BYPRODUCTS "${MX_GIT_SHA_CPP}"
    COMMAND "${CMAKE_COMMAND}"
        -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
        -DMX_GIT_SHA_SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DMX_GIT_SHA_TEMPLATE=${PRIVATE_DIR}/mx/core/Version.cpp.in
        -DMX_GIT_SHA_OUT=${MX_GIT_SHA_CPP}
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GitSha.cmake"
    COMMENT "Capturing git SHA"
    VERBATIM)
set_source_files_properties("${MX_GIT_SHA_CPP}" PROPERTIES GENERATED TRUE)

add_library(mx_core STATIC
    ${MX_CORE_RUNTIME_SOURCES} ${MX_CORE_GENERATED_SOURCES} ${MX_GIT_SHA_CPP})
add_dependencies(mx_core mx_git_sha)
target_include_directories(mx_core PUBLIC ${PRIVATE_DIR})
target_link_libraries(mx_core PUBLIC pugixml)
target_compile_features(mx_core PUBLIC cxx_std_20)

# mx_core unit tests (Gate 4 construction-safety: clamping, factories,
# structural value shapes; W2 adds the grammar-shape tests). Only built once
# the generator has emitted (the tests exercise generated types).
if(MX_CORE_GENERATED_SOURCES)
    file(GLOB_RECURSE SRC_MXTEST_CORE ${PRIVATE_DIR}/mxtest/core/*.cpp ${PRIVATE_DIR}/mxtest/core/*.h)
    file(GLOB_RECURSE SRC_CPUL_CORE ${PRIVATE_DIR}/cpul/*.cpp ${PRIVATE_DIR}/cpul/*.h)
    add_executable(mxtest-core ${SRC_MXTEST_CORE} ${SRC_CPUL_CORE})
    target_include_directories(mxtest-core PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-core mx_core)
endif()

if(MX_CORE_DEV)

    # corert: round-trips each data/ file through mx::core (pugixml-backed)
    # and compares against a normalized, fixup-adjusted form of the input.
    # Fixer applies .fixup.xml sidecars; cpul is the Catch test runner
    # (main); the import/ helpers normalize whitespace, decimals, and
    # attribute order.
    file(GLOB_RECURSE SRC_CORERT ${PRIVATE_DIR}/mxtest/corert/*.cpp ${PRIVATE_DIR}/mxtest/corert/*.h)
    file(GLOB_RECURSE SRC_CPUL   ${PRIVATE_DIR}/cpul/*.cpp ${PRIVATE_DIR}/cpul/*.h)
    set(SRC_CORERT_HELPERS
        ${PRIVATE_DIR}/mxtest/import/Normalize.cpp
        ${PRIVATE_DIR}/mxtest/import/Normalize.h
        ${PRIVATE_DIR}/mxtest/import/DecimalFields.h)

    # PathRoot.h hands the corert file walker the repo root at configure time.
    file(WRITE ${PRIVATE_DIR}/mxtest/file/PathRoot.h
"// Auto-generated by CMake.
#pragma once
#ifndef MX_REPO_ROOT_PATH
#define MX_REPO_ROOT_PATH \"${CMAKE_CURRENT_SOURCE_DIR}\"
#endif
")

    find_package(Threads)
    add_executable(mxtest-core-dev
        ${SRC_CORERT} ${SRC_CORERT_HELPERS} ${SRC_CPUL})
    target_include_directories(mxtest-core-dev PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-core-dev mx_core ${CMAKE_THREAD_LIBS_INIT})

    # mxtest-validate: the permanent Gate-2 tool (mx-core-plan.md §5.2).
    # Parses every eligible corpus file, serializes the OUTPUT to
    # build/validate-out/, plus the default-constructed Document; `make
    # validate-cpp` then xmllint-validates every output against the 4.0 XSD.
    add_executable(mxtest-validate
        ${PRIVATE_DIR}/mxtest/validate/main.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.h
        ${PRIVATE_DIR}/mxtest/corert/CoreRoundtripImpl.cpp
        ${PRIVATE_DIR}/mxtest/corert/CoreRoundtripImpl.h
        ${PRIVATE_DIR}/mxtest/corert/Fixer.cpp
        ${PRIVATE_DIR}/mxtest/corert/Fixer.h
        ${SRC_CORERT_HELPERS})
    target_include_directories(mxtest-validate PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-validate mx_core)

endif()

# Phase-2 step-0 baseline tooling (mx-impl-port-plan.md §8): the judge that
# scores old-master api-roundtrip captures with the same normalize/compare
# machinery corert uses. The capture driver next to it compiles only against
# old master (71fc402) and is committed as a record; both are excluded from
# the MX_API globs below.
option(MX_BASELINE_JUDGE "Build the Phase-2 step-0 baseline judge tool." OFF)
if(MX_BASELINE_JUDGE)
    add_executable(mxtest-baseline-judge
        ${PRIVATE_DIR}/mxtest/api/baseline/JudgeMain.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.h
        ${PRIVATE_DIR}/mxtest/corert/Fixer.cpp
        ${PRIVATE_DIR}/mxtest/corert/Fixer.h
        ${PRIVATE_DIR}/mxtest/import/Normalize.cpp
        ${PRIVATE_DIR}/mxtest/import/Normalize.h
        ${PRIVATE_DIR}/mxtest/import/DecimalFields.h)
    target_include_directories(mxtest-baseline-judge PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-baseline-judge pugixml)
endif()

# Phase-2 step-2/3 scratch target (mx-impl-port-plan.md §9): compiles the
# already-ported api/impl translation units as a static archive (an archive
# needs no symbol resolution, so the port can be validated TU by TU before
# all 77 files exist). Grows during the step-3 wave; retired when MX_API
# builds the real `mx` library.
option(MX_IMPL_SLICE "Compile the ported mx::impl slice (Phase-2 scratch target)." OFF)
if(MX_IMPL_SLICE)
    add_library(mx_impl_slice STATIC
        ${PRIVATE_DIR}/mx/api/DocumentManager.cpp
        ${PRIVATE_DIR}/mx/api/EncodingData.cpp
        ${PRIVATE_DIR}/mx/api/MarkData.cpp
        ${PRIVATE_DIR}/mx/api/ScoreData.cpp
        ${PRIVATE_DIR}/mx/api/SoundID.cpp
        ${PRIVATE_DIR}/mx/impl/Converter.cpp
        ${PRIVATE_DIR}/mx/impl/Cursor.cpp
        ${PRIVATE_DIR}/mx/impl/EncodingFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/LyricType.cpp
        ${PRIVATE_DIR}/mx/impl/MetronomeReader.cpp
        ${PRIVATE_DIR}/mx/impl/NoteReader.cpp
        ${PRIVATE_DIR}/mx/impl/NoteWriter.cpp
        ${PRIVATE_DIR}/mx/impl/ScoreConversions.cpp
        ${PRIVATE_DIR}/mx/impl/AccidentalMarkFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/ArticulationsFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/OrnamentsFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/TechnicalFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/ArpeggiateFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/FermataFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/LayoutFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/NonArpeggiateFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/PageTextFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/SlideFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/StaffFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/TimeReader.cpp
        ${PRIVATE_DIR}/mx/impl/DynamicsReader.cpp
        ${PRIVATE_DIR}/mx/impl/DynamicsWriter.cpp
        ${PRIVATE_DIR}/mx/impl/TupletReader.cpp
        ${PRIVATE_DIR}/mx/impl/NoteFunctions.cpp
        ${PRIVATE_DIR}/mx/impl/NotationsWriter.cpp
        ${PRIVATE_DIR}/mx/impl/PropertiesWriter.cpp
        ${PRIVATE_DIR}/mx/impl/DirectionReader.cpp
        ${PRIVATE_DIR}/mx/impl/DirectionWriter.cpp
        ${PRIVATE_DIR}/mx/impl/MeasureReader.cpp
        ${PRIVATE_DIR}/mx/impl/MeasureWriter.cpp
        ${PRIVATE_DIR}/mx/impl/PartReader.cpp
        ${PRIVATE_DIR}/mx/impl/PartWriter.cpp
        ${PRIVATE_DIR}/mx/impl/ScoreReader.cpp
        ${PRIVATE_DIR}/mx/impl/ScoreWriter.cpp)
    target_include_directories(mx_impl_slice PRIVATE ${PRIVATE_DIR})
    target_include_directories(mx_impl_slice PUBLIC $<BUILD_INTERFACE:${PUBLIC_DIR}>)
    target_link_libraries(mx_impl_slice PUBLIC mx_core)
endif()

if(MX_API)

    # =========================================================================
    # Phase 2 (the mx::impl port): the mx::api/mx::impl product library and
    # its preserved test suites. These sources are restored from the
    # pre-newgen tree and are NOT expected to compile until mx::impl is
    # reimplemented against the new mx::core and DocumentManager is rewritten
    # on pugixml/mx::core::parse (newgen-integration-plan.md §8). Default OFF;
    # this block exists so the port has a build home from day one.
    # =========================================================================

    message(STATUS "${PROJECT_NAME}: MX_API=ON: Phase-2 mx::api/mx::impl build")

    # PathRoot.h hands the api-test file walker the repo root at configure time.
    file(WRITE ${PRIVATE_DIR}/mxtest/file/PathRoot.h
"// Auto-generated by CMake.
#pragma once
#ifndef MX_REPO_ROOT_PATH
#define MX_REPO_ROOT_PATH \"${CMAKE_CURRENT_SOURCE_DIR}\"
#endif
")

    file(GLOB_RECURSE HEADERS_MX_API ${PUBLIC_DIR}/mx/api/*.h)
    file(GLOB_RECURSE SRC_MX_API ${PRIVATE_DIR}/mx/api/*.cpp ${PRIVATE_DIR}/mx/api/*.h)
    file(GLOB_RECURSE SRC_MX_IMPL ${PRIVATE_DIR}/mx/impl/*.cpp ${PRIVATE_DIR}/mx/impl/*.h)
    file(GLOB_RECURSE SRC_MX_UTILITY ${PRIVATE_DIR}/mx/utility/*.cpp ${PRIVATE_DIR}/mx/utility/*.h)

    add_library(mx STATIC ${HEADERS_MX_API} ${SRC_MX_API} ${SRC_MX_IMPL} ${SRC_MX_UTILITY})
    target_include_directories(mx PRIVATE ${PRIVATE_DIR})
    target_include_directories(mx PUBLIC $<BUILD_INTERFACE:${PUBLIC_DIR}>)
    target_link_libraries(mx PUBLIC mx_core)

    # The preserved api/impl suites (the Phase-2 acceptance gate) plus the
    # harness infrastructure they depend on (file, control, and the old api
    # import harness slated for a pugixml rewrite).
    file(GLOB_RECURSE SRC_MXTEST_API ${PRIVATE_DIR}/mxtest/api/*.cpp ${PRIVATE_DIR}/mxtest/api/*.h)
    # mxtest/api/baseline/ holds the step-0 capture/judge tooling, not suite code.
    list(FILTER SRC_MXTEST_API EXCLUDE REGEX "/mxtest/api/baseline/")
    # CorpusRoundtripMain.cpp has its own main(); it is a separate binary.
    list(FILTER SRC_MXTEST_API EXCLUDE REGEX "/mxtest/api/CorpusRoundtripMain\\.cpp")
    file(GLOB_RECURSE SRC_MXTEST_CONTROL ${PRIVATE_DIR}/mxtest/control/*.cpp ${PRIVATE_DIR}/mxtest/control/*.h)
    file(GLOB_RECURSE SRC_MXTEST_FILE ${PRIVATE_DIR}/mxtest/file/*.cpp ${PRIVATE_DIR}/mxtest/file/*.h)
    file(GLOB_RECURSE SRC_MXTEST_IMPL ${PRIVATE_DIR}/mxtest/impl/*.cpp ${PRIVATE_DIR}/mxtest/impl/*.h)
    file(GLOB_RECURSE SRC_MXTEST_IMPORT ${PRIVATE_DIR}/mxtest/import/*.cpp ${PRIVATE_DIR}/mxtest/import/*.h)
    file(GLOB_RECURSE SRC_CPUL_API ${PRIVATE_DIR}/cpul/*.cpp ${PRIVATE_DIR}/cpul/*.h)

    find_package(Threads)
    add_executable(mxtest
        ${SRC_MXTEST_API}
        ${SRC_MXTEST_CONTROL}
        ${SRC_MXTEST_FILE}
        ${SRC_MXTEST_IMPL}
        ${SRC_MXTEST_IMPORT}
        ${SRC_CPUL_API})
    target_include_directories(mxtest PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest mx ${CMAKE_THREAD_LIBS_INIT})

    # api-only example programs.
    add_executable(mxread ${PRIVATE_DIR}/mx/examples/Read.cpp)
    add_executable(mxwrite ${PRIVATE_DIR}/mx/examples/Write.cpp)
    add_executable(mxhide ${PRIVATE_DIR}/mx/examples/Hide.cpp)
    target_link_libraries(mxread mx ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(mxwrite mx ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(mxhide mx ${CMAKE_THREAD_LIBS_INIT})

    # Corpus api roundtrip harness (mx-impl-port-plan.md §8).
    # Regression mode (CI): mxtest-api-roundtrip regression <dataRoot> <baselineFile>
    # Discovery mode (manual): mxtest-api-roundtrip discovery <dataRoot>
    add_executable(mxtest-api-roundtrip
        ${PRIVATE_DIR}/mxtest/api/CorpusRoundtripMain.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.cpp
        ${PRIVATE_DIR}/mxtest/corert/Compare.h
        ${PRIVATE_DIR}/mxtest/corert/Fixer.cpp
        ${PRIVATE_DIR}/mxtest/corert/Fixer.h
        ${PRIVATE_DIR}/mxtest/import/Normalize.cpp
        ${PRIVATE_DIR}/mxtest/import/Normalize.h
        ${PRIVATE_DIR}/mxtest/import/DecimalFields.h)
    target_include_directories(mxtest-api-roundtrip PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-api-roundtrip mx ${CMAKE_THREAD_LIBS_INIT})

endif()
