// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "pugixml.hpp"

namespace mxtest
{

// The corert normalization pipeline pieces (AGENTS.md "Normalization
// pipeline"), applied to BOTH the expected and the actual document so the
// comparison sees canonical-against-canonical input.

// Set the root `version` attribute to the harness baseline.
void setRootMusicXmlVersion(pugi::xml_document &doc, const char *version);

// Remove whitespace-only text nodes from every element: pretty-printing
// indentation is not content (MusicXML has no mixed content), and the rule
// applies to both sides, so it stays symmetric.
void stripInterElementWhitespace(pugi::xml_document &doc);

// Strip trailing zeros from the decimal fields listed in DecimalFields.h
// (element text and attribute values alike).
void stripZerosFromDecimalFields(pugi::xml_document &doc);

// Re-space comma-separated-text fields (e.g. font-family) to "item, item":
// the MusicXML comma-separated-text type allows "x,y" or "x, y" on the wire
// and preserves whatever spelling it was parsed with, but mx's api round trip
// always re-serializes as "x, y". Normalizing both sides to the same spacing
// keeps the comparison about content, not incidental whitespace. The field
// list lives in CommaSeparatedFields.h.
void normalizeCommaSeparatedFields(pugi::xml_document &doc);

// Sort every element's attributes alphabetically by QUALIFIED name. Must
// run last.
void sortAttributes(pugi::xml_document &doc);

} // namespace mxtest
