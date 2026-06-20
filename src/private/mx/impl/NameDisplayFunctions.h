// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <string>

namespace mx
{
namespace core
{
class NameDisplay;
}
} // namespace mx

namespace mx
{
namespace impl
{
// Best-effort plain-text extraction from a MusicXML name-display element
// (<part-name-display>, <group-name-display>, etc.): the concatenated
// <display-text> runs, with <accidental-text> rendered as "b"/"#".
std::string extractDisplayText(const core::NameDisplay &nameDisplay);

// Reads the formatting (font, color, and print-style position attributes) of a
// name-display element from its first <display-text> run. The api models a
// display name as a single formatted string, so a multi-run display collapses
// to the formatting of its first run.
void extractDisplayFormatting(const core::NameDisplay &nameDisplay, api::PrintData &outPrintData,
                              api::PositionData &outPositionData);

// Build a minimal name-display carrying a single, unformatted <display-text>
// run. The inverse of extractDisplayText for plain-text display names.
core::NameDisplay makeNameDisplay(const std::string &text);

// As makeNameDisplay(text), but also applies the given formatting to the
// <display-text> run. This is how mx emits name formatting at the modern,
// non-deprecated *-display location instead of on the deprecated <part-name>
// attributes (see api/PartData.h for the full rationale).
core::NameDisplay makeNameDisplay(const std::string &text, const api::PrintData &printData,
                                  const api::PositionData &positionData);
} // namespace impl
} // namespace mx
