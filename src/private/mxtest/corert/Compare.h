// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "pugixml.hpp"

#include <string>

namespace mxtest
{
namespace corert
{

// The harness baseline both sides' root `version` attribute is pinned to
// (a harness choice, not a property of the corpus -- see AGENTS.md).
inline constexpr const char *const kMusicXmlVersionBaseline = "3.0";

struct CompareFailure
{
    bool isFailure = false;
    std::string detail;
};

// The shared normalize-for-comparison pipeline (applied identically to the
// expected and actual documents): pin the root version attribute to the
// harness baseline, strip whitespace-only text nodes, strip trailing zeros
// from decimal fields, re-space comma-separated-text fields, sort attributes
// (must be last).
void normalizeForComparison(pugi::xml_document &doc);

// Depth-first compare: element names, DIRECT text content only (never the
// subtree concatenation), attributes by qualified name with numeric
// equivalence (epsilon < 0.00000001 for floats). Both sides must already be
// normalized.
CompareFailure compareElements(pugi::xml_node expected, pugi::xml_node actual);

} // namespace corert
} // namespace mxtest
