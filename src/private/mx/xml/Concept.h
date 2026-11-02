// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// The contract an XML backend has to satisfy. mx::core reads and writes XML
// only through the handles in mx/xml/Xml.h, and those handles forward every
// operation to a static function of one Backend struct. This file says what
// that struct has to provide. The pugixml adapter in pugixml/Backend.h is the
// one mx ships; a build that prefers another XML library supplies a struct
// that satisfies XmlBackend and points MX_XML_BACKEND_HEADER at it. See
// docs/ai/design/xml-backend.md.

#pragma once

#include <concepts>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <string_view>
#include <type_traits>

namespace mx::xml
{

/// The kinds of node an XML tree holds. mx::core creates elements, text, the
/// declaration and the doctype, and skips every other kind when it reads.
enum class NodeType
{
    null,
    document,
    element,
    pcdata,
    cdata,
    comment,
    processingInstruction,
    declaration,
    doctype,
};

/// Why a load failed. mx::api tells the file-level failures apart from a
/// document that would not parse, so those are the only distinctions made;
/// every parse failure is `malformed`.
enum class ParseStatus
{
    ok,
    fileNotFound,
    ioError,
    outOfMemory,
    malformed,
};

struct ParseResult
{
    ParseStatus status = ParseStatus::ok;

    /// Byte offset into the input where parsing stopped, when the backend
    /// knows it; 0 otherwise.
    std::ptrdiff_t offset = 0;

    /// A short reason in the backend's own words, for error messages.
    std::string description;

    explicit operator bool() const noexcept
    {
        return status == ParseStatus::ok;
    }
};

/// Options for writing a document out. Combine with `|`.
enum class SaveFlags : unsigned
{
    none = 0,

    /// Write the tree without indentation or line breaks between nodes.
    raw = 1u << 0,

    /// Do not add an XML declaration when the tree holds none.
    noDeclaration = 1u << 1,
};

constexpr SaveFlags operator|(SaveFlags a, SaveFlags b) noexcept
{
    return static_cast<SaveFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

constexpr bool hasFlag(SaveFlags flags, SaveFlags flag) noexcept
{
    return (static_cast<unsigned>(flags) & static_cast<unsigned>(flag)) != 0;
}

/// What a backend provides. The three handle types and the static functions
/// below are the whole of it.
///
/// NodeHandle and AttributeHandle are small values that refer into a tree a
/// DocumentHandle owns: copying one copies the reference, not the node, and
/// a default-constructed handle is the null handle (nodeIsNull /
/// attributeIsNull return true for it). A DocumentHandle starts out as an
/// empty document, is never copied, and can be moved.
///
/// Strings come back as `const char *` owned by the tree, never null (the
/// empty string stands for "none"), and stay valid until the node changes
/// or the document resets. Strings go in as a pointer and a length, because
/// callers hold std::string_view.
template <typename B>
concept XmlBackend =
    std::is_trivially_copyable_v<typename B::NodeHandle> && std::default_initializable<typename B::NodeHandle> &&
    std::equality_comparable<typename B::NodeHandle> && std::is_trivially_copyable_v<typename B::AttributeHandle> &&
    std::default_initializable<typename B::AttributeHandle> && std::equality_comparable<typename B::AttributeHandle> &&
    std::default_initializable<typename B::DocumentHandle> &&
    std::is_move_constructible_v<typename B::DocumentHandle> && std::is_move_assignable_v<typename B::DocumentHandle> &&
    requires(const typename B::NodeHandle node, const typename B::AttributeHandle attribute,
             typename B::DocumentHandle &document, const typename B::DocumentHandle &constDocument, const char *name,
             const char *text, std::size_t size, std::string_view view, std::istream &in, std::ostream &out,
             NodeType type, SaveFlags flags) {
        // Nodes.
        { B::nodeIsNull(node) } -> std::same_as<bool>;
        { B::nodeType(node) } -> std::same_as<NodeType>;
        // An element's tag; "" for the other kinds.
        { B::nodeName(node) } -> std::same_as<const char *>;
        // The text of a pcdata, cdata, comment or doctype node; "" for an element.
        { B::nodeValue(node) } -> std::same_as<const char *>;
        { B::setNodeValue(node, text, size) } -> std::same_as<void>;
        // An element's direct text: the value of its first pcdata or cdata child, "" when there is none.
        { B::nodeText(node) } -> std::same_as<const char *>;
        { B::firstChild(node) } -> std::same_as<typename B::NodeHandle>;
        { B::nextSibling(node) } -> std::same_as<typename B::NodeHandle>;
        { B::previousSibling(node) } -> std::same_as<typename B::NodeHandle>;
        { B::parent(node) } -> std::same_as<typename B::NodeHandle>;
        // The first child element with this tag, or null.
        { B::child(node, name) } -> std::same_as<typename B::NodeHandle>;
        { B::attribute(node, name) } -> std::same_as<typename B::AttributeHandle>;
        { B::firstAttribute(node) } -> std::same_as<typename B::AttributeHandle>;
        // Appends an element with this tag and returns it.
        { B::appendChild(node, name) } -> std::same_as<typename B::NodeHandle>;
        // Appends a node of this kind (pcdata, declaration, doctype, ...) and returns it.
        { B::appendChild(node, type) } -> std::same_as<typename B::NodeHandle>;
        { B::appendAttribute(node, name) } -> std::same_as<typename B::AttributeHandle>;

        // Attributes.
        { B::attributeIsNull(attribute) } -> std::same_as<bool>;
        { B::attributeName(attribute) } -> std::same_as<const char *>;
        { B::attributeValue(attribute) } -> std::same_as<const char *>;
        { B::setAttributeValue(attribute, text, size) } -> std::same_as<void>;
        { B::nextAttribute(attribute) } -> std::same_as<typename B::AttributeHandle>;

        // Documents.
        // Empties the tree.
        { B::reset(document) } -> std::same_as<void>;
        // The document node: the parent of the declaration, the doctype and the root element.
        { B::root(constDocument) } -> std::same_as<typename B::NodeHandle>;
        // The root element, or null when the document has none.
        { B::documentElement(constDocument) } -> std::same_as<typename B::NodeHandle>;
        // Each load replaces the tree. The DOCTYPE has to be kept as a node: mx reads and writes it.
        { B::loadFile(document, name) } -> std::same_as<ParseResult>;
        { B::loadString(document, view) } -> std::same_as<ParseResult>;
        { B::load(document, in) } -> std::same_as<ParseResult>;
        // `indent` is the string written per nesting level unless the flags say raw.
        { B::saveFile(constDocument, name, text, flags) } -> std::same_as<bool>;
        { B::save(constDocument, out, text, flags) } -> std::same_as<void>;
    };

} // namespace mx::xml
