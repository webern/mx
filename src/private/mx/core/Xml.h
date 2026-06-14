// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include "pugixml/pugixml.hpp"

#include <string>
#include <string_view>

namespace mx::core
{

// Element-walking helpers the generated strict in-order parsers use. A
// "cursor" is the current candidate sibling element (or a null node at the
// end); text and comment nodes are not content (MusicXML has no mixed
// content) and are skipped.

pugi::xml_node firstElement(pugi::xml_node parent);

pugi::xml_node nextElement(pugi::xml_node node);

bool cursorIs(pugi::xml_node node, std::string_view name);

/// The element's DIRECT text content (first pcdata/cdata child), never null.
const char *childText(pugi::xml_node el);

/// True for XML namespace declarations (`xmlns`, `xmlns:*`): they are
/// document plumbing, not schema attributes; the strict attribute loop
/// skips them and the Document round-trips the root's declarations.
bool isNamespaceDecl(std::string_view attribute_name);

// Writing helpers.

void setAttribute(pugi::xml_node el, const char *name, std::string_view value);

void writeTextElement(pugi::xml_node parent, const char *tag, std::string_view text);

void setText(pugi::xml_node el, std::string_view text);

/// The slash path of a node for error messages, with 1-based ordinals among
/// same-name siblings: /score-partwise/part[1]/measure[3]/note[2].
std::string nodePath(pugi::xml_node node);

// Error throwers for the generated parsers (internal throw, converted to
// Result at the document boundary -- plan §2.5).

[[noreturn]] void throwUnknownElement(pugi::xml_node el);

[[noreturn]] void throwMisplacedElement(pugi::xml_node el);

[[noreturn]] void throwUnknownAttribute(pugi::xml_node el, const char *attribute_name);

[[noreturn]] void throwMissingElement(pugi::xml_node parent, const char *missing);

/// A required element was not at the cursor: wrongElementOrder when it
/// occurs among the remaining siblings (it exists, out of order),
/// missingRequiredElement otherwise.
[[noreturn]] void throwMissingOrMisplaced(pugi::xml_node parent, pugi::xml_node cursor, const char *missing);

[[noreturn]] void throwMissingAttribute(pugi::xml_node el, const char *attribute_name);

[[noreturn]] void throwTooManyElements(pugi::xml_node el);

} // namespace mx::core
