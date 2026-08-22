// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// The Swift package build's definition of mx::core::gitSha. CMake builds generate the real
// value at build time (cmake/GitSha.cmake renders Version.cpp.in into the build tree) and list
// mx_core's sources explicitly, so they never compile this file. The Swift package build globs
// src/ and has no code-generation step, so without this fallback the Mx product carries an
// undefined symbol that surfaces only when a consumer links an executable. "unknown" is the
// value Version.h already documents for builds outside the git/CMake machinery.
//
// Do not add this file to any CMake source list: the linker would see two definitions.

#include "mx/core/Version.h"

namespace mx::core
{

const char *const gitSha = "unknown";

} // namespace mx::core
