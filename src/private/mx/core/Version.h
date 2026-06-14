// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model. Never contains
// generated content (regen-safe split, see docs/ai/design/mx-core-plan.md §2.6).

#pragma once

namespace mx::core
{

/// The git commit the library was built from, e.g. "b987bfa16671", with a
/// "-dirty" suffix when the working tree had uncommitted changes, or "unknown"
/// when built outside a git checkout (e.g. an exported source archive).
/// Captured at build time rather than CMake-configure time, so the value stays
/// correct even when an IDE rebuild bypasses the Makefile and never re-runs
/// CMake (cmake/GitSha.cmake). This declaration never changes, so including it
/// costs nothing; only the generated definition's TU recompiles on a new SHA.
extern const char *const gitSha;

} // namespace mx::core
