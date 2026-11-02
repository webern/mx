# A small consumer project that builds mx against a stock pugixml instead of
# mx's private copy (MX_XML_BACKEND=pugixml). `make backend-pugixml-test`
# configures it into build/backend-pugixml, builds it, runs the core suites
# and checks that libmx_core.a references pugi:: and not mxpugi::. CI runs
# that target; a consumer who wants this mode can copy the shape below.
#
# MX_API is off: the backend lives in mx_core, and the core suites are the
# ones that run here, so building the api library and its tests would only
# lengthen the job.
#
# FetchContent_MakeAvailable needs CMake 3.14.
cmake_minimum_required(VERSION 3.14)
project(mx-backend-pugixml CXX)

# Upstream pugixml's CMake defines `pugixml`, an INTERFACE target over
# pugixml-static (or pugixml-shared with BUILD_SHARED_LIBS). That is the name
# mx looks for by default (MX_PUGIXML_TARGET), so nothing else is needed.
include(FetchContent)
FetchContent_Declare(pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.16
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(pugixml)

# The pugixml target has to exist before mx is added; mx's configure fails
# otherwise. Cache + FORCE so the value wins over any stale cache entry.
set(MX_XML_BACKEND pugixml CACHE STRING "" FORCE)
set(MX_CORE_DEV ON CACHE BOOL "" FORCE)
set(MX_API OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. mx)
