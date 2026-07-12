// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <set>
#include <string>

// The MusicXML comma-separated-text type (font-family is its only user in the
// schema) allows "x,y" or "x, y" on the wire and a parsed-but-unmutated value
// keeps its exact spelling on write. mx's api round trip always re-serializes
// as "x, y", so the corert normalization re-spaces BOTH sides of the
// comparison to that form. The list names every attribute the rule applies
// to.

namespace mxtest
{

const std::set<std::string> commaSeparatedFields = {"font-family"};

} // namespace mxtest
