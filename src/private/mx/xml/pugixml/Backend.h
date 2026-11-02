// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// The pugixml backend for mx::xml. Each function is a one-line call into
// pugixml; the handles in mx/xml/Xml.h forward to them. Two macros say which
// pugixml to compile against: mx's vendored copy under the mxpugi namespace
// by default, or a stock pugixml when the build defines MX_XML_PUGI_HEADER
// as "pugixml.hpp" and MX_XML_PUGI_NAMESPACE as pugi. Nothing here depends
// on the vendored copy in particular: any pugixml from 1.9 on works, built in
// its default char mode with the STL parts enabled (streams and std::string).

#pragma once

#include "mx/xml/Concept.h"

#ifndef MX_XML_PUGI_HEADER
#define MX_XML_PUGI_HEADER "mxpugi/mxpugi.hpp"
#endif

#ifndef MX_XML_PUGI_NAMESPACE
#define MX_XML_PUGI_NAMESPACE mxpugi
#endif

#include MX_XML_PUGI_HEADER

#include <cstddef>
#include <iosfwd>
#include <string>
#include <string_view>

namespace mx::xml
{

// The one place the namespace macro is spelled out.
namespace pugixml_ns = MX_XML_PUGI_NAMESPACE;

struct Backend
{
    using NodeHandle = pugixml_ns::xml_node;
    using AttributeHandle = pugixml_ns::xml_attribute;
    using DocumentHandle = pugixml_ns::xml_document;

    // Nodes.

    static bool nodeIsNull(NodeHandle node)
    {
        return !node;
    }

    static NodeType nodeType(NodeHandle node)
    {
        switch (node.type())
        {
        case pugixml_ns::node_document:
            return NodeType::document;
        case pugixml_ns::node_element:
            return NodeType::element;
        case pugixml_ns::node_pcdata:
            return NodeType::pcdata;
        case pugixml_ns::node_cdata:
            return NodeType::cdata;
        case pugixml_ns::node_comment:
            return NodeType::comment;
        case pugixml_ns::node_pi:
            return NodeType::processingInstruction;
        case pugixml_ns::node_declaration:
            return NodeType::declaration;
        case pugixml_ns::node_doctype:
            return NodeType::doctype;
        case pugixml_ns::node_null:
            break;
        }
        return NodeType::null;
    }

    static const char *nodeName(NodeHandle node)
    {
        return node.name();
    }

    static const char *nodeValue(NodeHandle node)
    {
        return node.value();
    }

    static void setNodeValue(NodeHandle node, const char *text, std::size_t size)
    {
        setPugiValue(node, text, size);
    }

    static const char *nodeText(NodeHandle node)
    {
        return node.text().get();
    }

    static NodeHandle firstChild(NodeHandle node)
    {
        return node.first_child();
    }

    static NodeHandle nextSibling(NodeHandle node)
    {
        return node.next_sibling();
    }

    static NodeHandle previousSibling(NodeHandle node)
    {
        return node.previous_sibling();
    }

    static NodeHandle parent(NodeHandle node)
    {
        return node.parent();
    }

    static NodeHandle child(NodeHandle node, const char *name)
    {
        return node.child(name);
    }

    static AttributeHandle attribute(NodeHandle node, const char *name)
    {
        return node.attribute(name);
    }

    static AttributeHandle firstAttribute(NodeHandle node)
    {
        return node.first_attribute();
    }

    static NodeHandle appendChild(NodeHandle node, const char *name)
    {
        return node.append_child(name);
    }

    static NodeHandle appendChild(NodeHandle node, NodeType type)
    {
        return node.append_child(toPugiType(type));
    }

    static AttributeHandle appendAttribute(NodeHandle node, const char *name)
    {
        return node.append_attribute(name);
    }

    // Attributes.

    static bool attributeIsNull(AttributeHandle attribute)
    {
        return !attribute;
    }

    static const char *attributeName(AttributeHandle attribute)
    {
        return attribute.name();
    }

    static const char *attributeValue(AttributeHandle attribute)
    {
        return attribute.value();
    }

    static void setAttributeValue(AttributeHandle attribute, const char *text, std::size_t size)
    {
        setPugiValue(attribute, text, size);
    }

    static AttributeHandle nextAttribute(AttributeHandle attribute)
    {
        return attribute.next_attribute();
    }

    // Documents.

    static void reset(DocumentHandle &document)
    {
        document.reset();
    }

    // A pugixml document is itself the document node.
    static NodeHandle root(const DocumentHandle &document)
    {
        return document;
    }

    static NodeHandle documentElement(const DocumentHandle &document)
    {
        return document.document_element();
    }

    static ParseResult loadFile(DocumentHandle &document, const char *path);

    static ParseResult loadString(DocumentHandle &document, std::string_view xml);

    static ParseResult load(DocumentHandle &document, std::istream &in);

    static bool saveFile(const DocumentHandle &document, const char *path, const char *indent, SaveFlags flags);

    static void save(const DocumentHandle &document, std::ostream &out, const char *indent, SaveFlags flags);

  private:
    static pugixml_ns::xml_node_type toPugiType(NodeType type)
    {
        switch (type)
        {
        case NodeType::document:
            return pugixml_ns::node_document;
        case NodeType::element:
            return pugixml_ns::node_element;
        case NodeType::pcdata:
            return pugixml_ns::node_pcdata;
        case NodeType::cdata:
            return pugixml_ns::node_cdata;
        case NodeType::comment:
            return pugixml_ns::node_comment;
        case NodeType::processingInstruction:
            return pugixml_ns::node_pi;
        case NodeType::declaration:
            return pugixml_ns::node_declaration;
        case NodeType::doctype:
            return pugixml_ns::node_doctype;
        case NodeType::null:
            break;
        }
        return pugixml_ns::node_null;
    }

    // The sized set_value overload arrived in pugixml 1.13. Older releases
    // get a copy so the text is terminated.
    template <typename Handle> static void setPugiValue(Handle handle, const char *text, std::size_t size)
    {
#if PUGIXML_VERSION >= 1130
        handle.set_value(text, size);
#else
        handle.set_value(std::string{text ? text : "", size}.c_str());
#endif
    }
};

} // namespace mx::xml
