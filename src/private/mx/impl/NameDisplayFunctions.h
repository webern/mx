// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

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
// <display-text> runs, with <accidental-text> rendered as "b"/"#". Formatting
// attributes are not preserved -- the api models display names as plain text.
std::string extractDisplayText(const core::NameDisplay &nameDisplay);

// Build a minimal name-display carrying a single <display-text> run. The
// inverse of extractDisplayText for plain-text display names.
core::NameDisplay makeNameDisplay(const std::string &text);
} // namespace impl
} // namespace mx
