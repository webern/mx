// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.
//
// Everything mx::core does with XML goes through mx::xml (the seam over the
// XML library, see mx/xml/Xml.h). This header includes it in full so that a
// generated source gets the complete types from the one include.

#pragma once

#include "mx/xml/Xml.h"

#include <string>
#include <string_view>

namespace mx::core
{

// Element-walking helpers the generated strict in-order parsers use. A
// "cursor" is the current candidate sibling element (or a null node at the
// end); text and comment nodes are not content (MusicXML has no mixed
// content) and are skipped.

xml::Node firstElement(xml::Node parent);

xml::Node nextElement(xml::Node node);

bool cursorIs(xml::Node node, std::string_view name);

/// The element's DIRECT text content (first pcdata/cdata child), never null.
const char *childText(xml::Node el);

/// True for XML namespace declarations (`xmlns`, `xmlns:*`): they are
/// document plumbing, not schema attributes; the strict attribute loop
/// skips them and the Document round-trips the root's declarations.
bool isNamespaceDecl(std::string_view attribute_name);

// Writing helpers.

void setAttribute(xml::Node el, const char *name, std::string_view value);

void writeTextElement(xml::Node parent, const char *tag, std::string_view text);

void setText(xml::Node el, std::string_view text);

/// The slash path of a node for error messages, with 1-based ordinals among
/// same-name siblings: /score-partwise/part[1]/measure[3]/note[2].
std::string nodePath(xml::Node node);

// Error throwers for the generated parsers (internal throw, converted to
// Result at the document boundary -- plan §2.5).

[[noreturn]] void throwUnknownElement(xml::Node el);

[[noreturn]] void throwMisplacedElement(xml::Node el);

[[noreturn]] void throwUnknownAttribute(xml::Node el, const char *attribute_name);

[[noreturn]] void throwMissingElement(xml::Node parent, const char *missing);

/// A required element was not at the cursor: wrongElementOrder when it
/// occurs among the remaining siblings (it exists, out of order),
/// missingRequiredElement otherwise.
[[noreturn]] void throwMissingOrMisplaced(xml::Node parent, xml::Node cursor, const char *missing);

[[noreturn]] void throwMissingAttribute(xml::Node el, const char *attribute_name);

[[noreturn]] void throwTooManyElements(xml::Node el);

} // namespace mx::core
