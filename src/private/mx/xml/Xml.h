// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// The XML seam. mx::core parses from and serializes into these three types
// and nothing else, so the XML library underneath is a build-time choice
// (Backend.h makes it). Node and Attribute are small value handles into a
// tree that a Document owns; copying one copies the reference, the way
// copying a pointer does. Every member is an inline forwarder to one static
// function of the Backend. An optimizing build inlines the whole chain and
// pays nothing for the indirection; a debug build pays a call per layer.
//
// The surface is what mx::core, mx::api and the tests use. It is not a
// general XML library and does not expose the backend's own types, so code
// on this side of the seam cannot come to depend on one backend.

#pragma once

#include "mx/xml/Backend.h"
#include "mx/xml/Concept.h"

#include <cstddef>
#include <iosfwd>
#include <iterator>
#include <string_view>

namespace mx::xml
{

static_assert(XmlBackend<Backend>,
              "the selected XML backend does not satisfy mx::xml::XmlBackend; see mx/xml/Concept.h");

class Node;
class NodeRange;
class AttributeRange;

class Attribute
{
  public:
    /// The null attribute.
    Attribute() = default;

    explicit operator bool() const
    {
        return !Backend::attributeIsNull(m_handle);
    }

    /// Never null; "" for the null attribute.
    const char *name() const
    {
        return Backend::attributeName(m_handle);
    }

    /// Never null; "" for the null attribute.
    const char *value() const
    {
        return Backend::attributeValue(m_handle);
    }

    void setValue(std::string_view value)
    {
        Backend::setAttributeValue(m_handle, value.data(), value.size());
    }

    /// The next attribute of the same element, or the null attribute.
    Attribute nextAttribute() const
    {
        return Attribute{Backend::nextAttribute(m_handle)};
    }

    bool operator==(const Attribute &) const = default;

  private:
    friend class Node;

    explicit Attribute(Backend::AttributeHandle handle) : m_handle{handle}
    {
    }

    Backend::AttributeHandle m_handle{};
};

class Node
{
  public:
    /// The null node.
    Node() = default;

    explicit operator bool() const
    {
        return !Backend::nodeIsNull(m_handle);
    }

    bool empty() const
    {
        return Backend::nodeIsNull(m_handle);
    }

    NodeType type() const
    {
        return Backend::nodeType(m_handle);
    }

    /// An element's tag. Never null; "" for other kinds of node.
    const char *name() const
    {
        return Backend::nodeName(m_handle);
    }

    /// The text of a pcdata, cdata, comment or doctype node. Never null; ""
    /// for an element.
    const char *value() const
    {
        return Backend::nodeValue(m_handle);
    }

    void setValue(std::string_view value)
    {
        Backend::setNodeValue(m_handle, value.data(), value.size());
    }

    /// An element's direct text: the value of its first pcdata or cdata
    /// child. Never null; "" when there is none.
    const char *text() const
    {
        return Backend::nodeText(m_handle);
    }

    Node firstChild() const
    {
        return Node{Backend::firstChild(m_handle)};
    }

    Node nextSibling() const
    {
        return Node{Backend::nextSibling(m_handle)};
    }

    Node previousSibling() const
    {
        return Node{Backend::previousSibling(m_handle)};
    }

    Node parent() const
    {
        return Node{Backend::parent(m_handle)};
    }

    /// The first child element with this tag, or the null node.
    Node child(const char *name) const
    {
        return Node{Backend::child(m_handle, name)};
    }

    Attribute attribute(const char *name) const
    {
        return Attribute{Backend::attribute(m_handle, name)};
    }

    Attribute firstAttribute() const
    {
        return Attribute{Backend::firstAttribute(m_handle)};
    }

    /// Every child, of every kind, for range-for.
    NodeRange children() const;

    /// Every attribute, for range-for.
    AttributeRange attributes() const;

    /// Appends an element with this tag and returns it.
    Node appendChild(const char *name)
    {
        return Node{Backend::appendChild(m_handle, name)};
    }

    /// Appends a node of this kind (pcdata, declaration, doctype, ...) and
    /// returns it.
    Node appendChild(NodeType type)
    {
        return Node{Backend::appendChild(m_handle, type)};
    }

    Attribute appendAttribute(const char *name)
    {
        return Attribute{Backend::appendAttribute(m_handle, name)};
    }

    bool operator==(const Node &) const = default;

  private:
    friend class Document;

    explicit Node(Backend::NodeHandle handle) : m_handle{handle}
    {
    }

    Backend::NodeHandle m_handle{};
};

/// Forward iterator over siblings; the end is the null node.
class NodeIterator
{
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Node;
    using difference_type = std::ptrdiff_t;
    using pointer = const Node *;
    using reference = const Node &;

    NodeIterator() = default;

    explicit NodeIterator(Node node) : m_node{node}
    {
    }

    reference operator*() const
    {
        return m_node;
    }

    pointer operator->() const
    {
        return &m_node;
    }

    NodeIterator &operator++()
    {
        m_node = m_node.nextSibling();
        return *this;
    }

    NodeIterator operator++(int)
    {
        const NodeIterator before = *this;
        ++*this;
        return before;
    }

    bool operator==(const NodeIterator &) const = default;

  private:
    Node m_node;
};

class NodeRange
{
  public:
    explicit NodeRange(Node parent) : m_parent{parent}
    {
    }

    NodeIterator begin() const
    {
        return NodeIterator{m_parent.firstChild()};
    }

    NodeIterator end() const
    {
        return NodeIterator{};
    }

  private:
    Node m_parent;
};

/// Forward iterator over an element's attributes; the end is the null
/// attribute.
class AttributeIterator
{
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Attribute;
    using difference_type = std::ptrdiff_t;
    using pointer = const Attribute *;
    using reference = const Attribute &;

    AttributeIterator() = default;

    explicit AttributeIterator(Attribute attribute) : m_attribute{attribute}
    {
    }

    reference operator*() const
    {
        return m_attribute;
    }

    pointer operator->() const
    {
        return &m_attribute;
    }

    AttributeIterator &operator++()
    {
        m_attribute = m_attribute.nextAttribute();
        return *this;
    }

    AttributeIterator operator++(int)
    {
        const AttributeIterator before = *this;
        ++*this;
        return before;
    }

    bool operator==(const AttributeIterator &) const = default;

  private:
    Attribute m_attribute;
};

class AttributeRange
{
  public:
    explicit AttributeRange(Node element) : m_element{element}
    {
    }

    AttributeIterator begin() const
    {
        return AttributeIterator{m_element.firstAttribute()};
    }

    AttributeIterator end() const
    {
        return AttributeIterator{};
    }

  private:
    Node m_element;
};

inline NodeRange Node::children() const
{
    return NodeRange{*this};
}

inline AttributeRange Node::attributes() const
{
    return AttributeRange{*this};
}

/// Owns an XML tree. Starts out empty; each load replaces the tree.
class Document
{
  public:
    Document() = default;
    Document(const Document &) = delete;
    Document &operator=(const Document &) = delete;
    Document(Document &&) = default;
    Document &operator=(Document &&) = default;
    ~Document() = default;

    /// Empties the tree.
    void reset()
    {
        Backend::reset(m_handle);
    }

    /// The document node: the parent of the declaration, the doctype and
    /// the root element, so it is where a serializer appends them.
    Node root() const
    {
        return Node{Backend::root(m_handle)};
    }

    /// The root element, or the null node when the document has none.
    Node documentElement() const
    {
        return Node{Backend::documentElement(m_handle)};
    }

    ParseResult loadFile(const char *path)
    {
        return Backend::loadFile(m_handle, path);
    }

    ParseResult loadString(std::string_view xml)
    {
        return Backend::loadString(m_handle, xml);
    }

    ParseResult load(std::istream &in)
    {
        return Backend::load(m_handle, in);
    }

    /// False when the file could not be written.
    bool saveFile(const char *path, const char *indent = "  ", SaveFlags flags = SaveFlags::none) const
    {
        return Backend::saveFile(m_handle, path, indent, flags);
    }

    void save(std::ostream &out, const char *indent = "  ", SaveFlags flags = SaveFlags::none) const
    {
        Backend::save(m_handle, out, indent, flags);
    }

  private:
    Backend::DocumentHandle m_handle{};
};

} // namespace mx::xml
