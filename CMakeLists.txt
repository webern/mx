cmake_minimum_required(VERSION 3.13)
project(mx CXX)

# The mx::core C++ typed model: the generated src/private/mx/core (emitted by
# `python3 -m gen gen/cpp/config.toml`, committed) + its hand-written runtime
# + the mx::xml seam over the XML library (src/private/mx/xml, backed by mx's
# private copy of pugixml, mxpugi, unless MX_XML_BACKEND says otherwise), plus
# the corert/unit test binaries.
# See AGENTS.md and docs/ai/design/xml-backend.md.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Emscripten disables exception catching by default; mx's internal MX_THROW
# (Throw.h) needs it, so the api boundary can actually catch and convert it.
#
# Emscripten's default wasm stack is 64 KiB. mx's write path is a deep,
# unoptimized (Debug) C++ call chain -- fromScore -> ScoreWriter ->
# PartWriter -> MeasureWriter -> NoteWriter -> NotationsWriter, several of
# which carry their own sizable locals -- and it overflows that default with
# only a little headroom to spare (issue #389 hit this by adding two more
# spanner-writing loops). 1 MiB was measured to leave a comfortable margin
# for further growth in that same call chain.
if(EMSCRIPTEN)
    add_compile_options(-fexceptions)
    add_link_options(-fexceptions -sSTACK_SIZE=1048576)
endif()

option(MX_CORE_DEV "Build the core roundtrip (corert) test binary against the regenerated mx/core." OFF)

option(MX_API "Build the mx::api/mx::impl product library and its tests." ON)

option(MX_COVERAGE "Instrument the build for gcov coverage (GCC/Clang only)." OFF)

# Which XML library sits behind the mx::xml seam (docs/ai/design/xml-backend.md).
#   vendored  mx's private copy of pugixml, compiled under the mxpugi namespace.
#             Always the default; nothing to install and no pugi:: symbol.
#   pugixml   the consumer's own pugixml. MX_PUGIXML_TARGET names its CMake
#             target, which must exist before add_subdirectory(mx). mx's
#             pugixml adapter then includes "pugixml.hpp" and uses the pugi
#             namespace.
#   custom    a backend the consumer wrote: MX_XML_BACKEND_HEADER is the
#             include path of a header that defines struct mx::xml::Backend
#             satisfying the XmlBackend concept (src/private/mx/xml/Concept.h),
#             and MX_XML_BACKEND_TARGET is the CMake target that carries that
#             header's include directory, its sources and the XML library it
#             links. mx's pugixml adapter is not compiled.
# In every mode the test binaries link mxpugi for their own DOM work and hand
# mx::core XML text, so they work whichever backend mx::core uses.
set(MX_XML_BACKEND "vendored" CACHE STRING
    "XML library behind mx::core: vendored (mx's private pugixml), pugixml (your pugixml target, see MX_PUGIXML_TARGET) or custom (your mx::xml::Backend, see MX_XML_BACKEND_HEADER and MX_XML_BACKEND_TARGET).")
set_property(CACHE MX_XML_BACKEND PROPERTY STRINGS vendored pugixml custom)
set(MX_PUGIXML_TARGET "pugixml" CACHE STRING
    "MX_XML_BACKEND=pugixml: the CMake target of your pugixml (upstream pugixml's CMake defines `pugixml`).")
set(MX_XML_BACKEND_HEADER "" CACHE STRING
    "MX_XML_BACKEND=custom: the include path of the header that defines mx::xml::Backend, as it appears inside the quotes of an #include, e.g. myapp/MxXmlBackend.h.")
set(MX_XML_BACKEND_TARGET "" CACHE STRING
    "MX_XML_BACKEND=custom: the CMake target that provides that header's include directory, its sources and the XML library.")

if(MX_XML_BACKEND STREQUAL "vendored")
    # Nothing to check.
elseif(MX_XML_BACKEND STREQUAL "pugixml")
    if(NOT TARGET "${MX_PUGIXML_TARGET}")
        message(FATAL_ERROR
            "MX_XML_BACKEND=pugixml needs a CMake target for your pugixml, and "
            "MX_PUGIXML_TARGET=\"${MX_PUGIXML_TARGET}\" is not a target. Create it "
            "before add_subdirectory(mx), with FetchContent, find_package(pugixml) "
            "or add_subdirectory on a pugixml checkout, or set MX_PUGIXML_TARGET "
            "to the name of the target you have.")
    endif()
elseif(MX_XML_BACKEND STREQUAL "custom")
    if(NOT MX_XML_BACKEND_HEADER OR NOT MX_XML_BACKEND_TARGET)
        message(FATAL_ERROR
            "MX_XML_BACKEND=custom needs both MX_XML_BACKEND_HEADER (the include "
            "path of the header that defines mx::xml::Backend) and "
            "MX_XML_BACKEND_TARGET (the CMake target that carries it). "
            "MX_XML_BACKEND_HEADER=\"${MX_XML_BACKEND_HEADER}\" "
            "MX_XML_BACKEND_TARGET=\"${MX_XML_BACKEND_TARGET}\"")
    endif()
    if(NOT TARGET "${MX_XML_BACKEND_TARGET}")
        message(FATAL_ERROR
            "MX_XML_BACKEND=custom: MX_XML_BACKEND_TARGET=\"${MX_XML_BACKEND_TARGET}\" "
            "is not a CMake target. Create it before add_subdirectory(mx).")
    endif()
else()
    message(FATAL_ERROR
        "MX_XML_BACKEND=\"${MX_XML_BACKEND}\" is not one of: vendored, pugixml, custom.")
endif()
message(STATUS "${PROJECT_NAME}: MX_XML_BACKEND=${MX_XML_BACKEND}")

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

# mxpugi: the XML library behind mx::core's XML seam in the default
# (vendored) mode (mx::core uses pugixml directly). It is pugixml, vendored at
# src/private/mxpugi by scripts/vendor_pugixml.py with its namespace renamed
# from `pugi` to `mxpugi` (stock pugiconfig). The private copy exists so that
# mx does not define any `pugi::` symbol: a consumer that links its own
# pugixml does not get duplicates from mx. Only the adapter in
# src/private/mx/xml/pugixml includes it; the rest of mx::core sees mx::xml
# (docs/ai/design/xml-backend.md). The test binaries use it in every mode, so
# the target always exists; in the other modes it is left out of `all` and
# only builds when a test binary asks for it (building just `mx` skips it).
add_library(mxpugi STATIC ${PRIVATE_DIR}/mxpugi/mxpugi.cpp)
target_include_directories(mxpugi PUBLIC ${PRIVATE_DIR})
if(NOT MX_XML_BACKEND STREQUAL "vendored")
    set_target_properties(mxpugi PROPERTIES EXCLUDE_FROM_ALL ON)
endif()

# mx_core: the generated C++ typed model plus its hand-written runtime
# (Decimal, Result, Error, Lexical, ParseContext, Token, NameToken, Language,
# OneOrMore, Xml) and the mx::xml seam (src/private/mx/xml: the value handles
# in Xml.h, the XmlBackend concept, and the pugixml adapter). The generated
# per-type sources live in
# src/private/mx/core/generated/
# (their own directory, so generator-owned code is recognizable at a glance)
# and arrive via sources.cmake (emitted by `python3 -m gen gen/cpp/config.toml`).
set(MX_CORE_RUNTIME_SOURCES
    ${PRIVATE_DIR}/mx/core/Attribution.cpp
    ${PRIVATE_DIR}/mx/core/Attribution.h
    ${PRIVATE_DIR}/mx/core/Decimal.cpp
    ${PRIVATE_DIR}/mx/core/Decimal.h
    ${PRIVATE_DIR}/mx/core/Error.h
    ${PRIVATE_DIR}/mx/core/IdIntegrity.cpp
    ${PRIVATE_DIR}/mx/core/IdIntegrity.h
    ${PRIVATE_DIR}/mx/core/Language.cpp
    ${PRIVATE_DIR}/mx/core/Language.h
    ${PRIVATE_DIR}/mx/core/Lexical.cpp
    ${PRIVATE_DIR}/mx/core/Lexical.h
    ${PRIVATE_DIR}/mx/core/NameToken.cpp
    ${PRIVATE_DIR}/mx/core/NameToken.h
    ${PRIVATE_DIR}/mx/core/OneOrMore.h
    ${PRIVATE_DIR}/mx/core/ParseContext.cpp
    ${PRIVATE_DIR}/mx/core/ParseContext.h
    ${PRIVATE_DIR}/mx/core/Result.h
    ${PRIVATE_DIR}/mx/core/Token.cpp
    ${PRIVATE_DIR}/mx/core/Token.h
    ${PRIVATE_DIR}/mx/core/Xml.cpp
    ${PRIVATE_DIR}/mx/core/Xml.h
    ${PRIVATE_DIR}/mx/xml/Backend.h
    ${PRIVATE_DIR}/mx/xml/Concept.h
    ${PRIVATE_DIR}/mx/xml/Xml.h)
# The pugixml adapter includes pugixml, so a custom backend leaves it out.
if(NOT MX_XML_BACKEND STREQUAL "custom")
    list(APPEND MX_CORE_RUNTIME_SOURCES
        ${PRIVATE_DIR}/mx/xml/pugixml/Backend.cpp
        ${PRIVATE_DIR}/mx/xml/pugixml/Backend.h)
endif()
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
    COMMENT "Capturing MX git SHA"
    VERBATIM)
set_source_files_properties("${MX_GIT_SHA_CPP}" PROPERTIES GENERATED TRUE)

add_library(mx_core STATIC
    ${MX_CORE_RUNTIME_SOURCES} ${MX_CORE_GENERATED_SOURCES} ${MX_GIT_SHA_CPP})
add_dependencies(mx_core mx_git_sha)
target_include_directories(mx_core PUBLIC ${PRIVATE_DIR})
target_compile_features(mx_core PUBLIC cxx_std_20)

# The backend chosen above. The link and the definitions are PUBLIC because
# mx::core's headers include mx/xml/Xml.h, so everything that compiles against
# mx_core (the mx library, the tests, a consumer) has to see the same backend.
if(MX_XML_BACKEND STREQUAL "vendored")
    target_link_libraries(mx_core PUBLIC mxpugi)
elseif(MX_XML_BACKEND STREQUAL "pugixml")
    target_link_libraries(mx_core PUBLIC ${MX_PUGIXML_TARGET})
    target_compile_definitions(mx_core PUBLIC
        MX_XML_PUGI_NAMESPACE=pugi
        "MX_XML_PUGI_HEADER=\"pugixml.hpp\"")
else()
    target_link_libraries(mx_core PUBLIC ${MX_XML_BACKEND_TARGET})
    target_compile_definitions(mx_core PUBLIC
        "MX_XML_BACKEND_HEADER=\"${MX_XML_BACKEND_HEADER}\"")
endif()

# mx_core unit tests (clamping, factories, structural value shapes,
# grammar-shape tests). Only built once the generator has emitted (the
# tests exercise generated types).
if(MX_CORE_GENERATED_SOURCES)
    file(GLOB_RECURSE SRC_MXTEST_CORE ${PRIVATE_DIR}/mxtest/core/*.cpp ${PRIVATE_DIR}/mxtest/core/*.h)
    file(GLOB_RECURSE SRC_CPUL_CORE ${PRIVATE_DIR}/cpul/*.cpp ${PRIVATE_DIR}/cpul/*.h)
    add_executable(mxtest-core ${SRC_MXTEST_CORE} ${SRC_CPUL_CORE})
    target_include_directories(mxtest-core PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-core mx_core mxpugi)
endif()

if(MX_CORE_DEV)

    # corert: round-trips each data/ file through mx::core and compares
    # against a normalized, fixup-adjusted form of the input. The harness
    # does its own DOM work with mxpugi (linked here on purpose, whatever
    # MX_XML_BACKEND says) and hands mx::core XML text, the way a consumer
    # with a different XML backend would.
    # Fixer applies .fixup.xml sidecars; cpul is the Catch test runner
    # (main); the import/ helpers normalize whitespace, decimals,
    # comma-separated-text spacing, and attribute order.
    file(GLOB_RECURSE SRC_CORERT ${PRIVATE_DIR}/mxtest/corert/*.cpp ${PRIVATE_DIR}/mxtest/corert/*.h)
    file(GLOB_RECURSE SRC_CPUL   ${PRIVATE_DIR}/cpul/*.cpp ${PRIVATE_DIR}/cpul/*.h)
    set(SRC_CORERT_HELPERS
        ${PRIVATE_DIR}/mxtest/import/Normalize.cpp
        ${PRIVATE_DIR}/mxtest/import/Normalize.h
        ${PRIVATE_DIR}/mxtest/import/DecimalFields.h
        ${PRIVATE_DIR}/mxtest/import/CommaSeparatedFields.h)

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
    target_link_libraries(mxtest-core-dev mx_core mxpugi ${CMAKE_THREAD_LIBS_INIT})

endif()


if(MX_API)

    message(STATUS "${PROJECT_NAME}: MX_API=ON: building mx::api/mx::impl")

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

    # The api/impl suites plus the harness infrastructure they depend on
    # (file, control, and the api import harness).
    file(GLOB_RECURSE SRC_MXTEST_API ${PRIVATE_DIR}/mxtest/api/*.cpp ${PRIVATE_DIR}/mxtest/api/*.h)
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
    target_link_libraries(mxtest mx mxpugi ${CMAKE_THREAD_LIBS_INIT})

    # api-only example programs.
    add_executable(mxread ${PRIVATE_DIR}/mx/examples/Read.cpp)
    add_executable(mxwrite ${PRIVATE_DIR}/mx/examples/Write.cpp)
    add_executable(mxhide ${PRIVATE_DIR}/mx/examples/Hide.cpp)
    add_executable(mxiteratenotesandtiming ${PRIVATE_DIR}/mx/examples/IterateNotesAndTiming.cpp)
    target_link_libraries(mxread mx ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(mxwrite mx ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(mxhide mx ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(mxiteratenotesandtiming mx ${CMAKE_THREAD_LIBS_INIT})

    # Corpus api roundtrip harness.
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
        ${PRIVATE_DIR}/mxtest/import/DecimalFields.h
        ${PRIVATE_DIR}/mxtest/import/CommaSeparatedFields.h)
    target_include_directories(mxtest-api-roundtrip PRIVATE ${PRIVATE_DIR})
    target_link_libraries(mxtest-api-roundtrip mx mxpugi ${CMAKE_THREAD_LIBS_INIT})

endif()
