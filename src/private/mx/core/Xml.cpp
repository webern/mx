// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Xml.h"

#include "mx/core/Error.h"
#include "mx/core/Lexical.h"

namespace mx::core
{

xml::Node firstElement(xml::Node parent)
{
    for (xml::Node n = parent.firstChild(); n; n = n.nextSibling())
    {
        if (n.type() == xml::NodeType::element)
        {
            return n;
        }
    }
    return {};
}

xml::Node nextElement(xml::Node node)
{
    for (xml::Node n = node.nextSibling(); n; n = n.nextSibling())
    {
        if (n.type() == xml::NodeType::element)
        {
            return n;
        }
    }
    return {};
}

bool cursorIs(xml::Node node, std::string_view name)
{
    return node && std::string_view{node.name()} == name;
}

const char *childText(xml::Node el)
{
    return el.text();
}

bool isNamespaceDecl(std::string_view attribute_name)
{
    if (attribute_name == "xmlns")
    {
        return true;
    }
    return attribute_name.size() > 6 && attribute_name.substr(0, 6) == "xmlns:";
}

// XML 1.0 forbids the C0 control characters other than tab/LF/CR; an XML
// writer would escape them as character references that are THEMSELVES
// illegal (&#01;), producing a non-well-formed document. Every generated
// write funnels through the helpers below, so dropping the illegal
// characters here makes them unrepresentable through any string-typed
// setter (the deterministic-repair policy applied at the
// serialization door).
bool xmlIsIllegalChar(char c) noexcept
{
    const auto u = static_cast<unsigned char>(c);
    return u < 0x20 && c != '\t' && c != '\n' && c != '\r';
}

bool xmlNeedsSanitize(std::string_view text) noexcept
{
    for (const char c : text)
    {
        if (xmlIsIllegalChar(c))
        {
            return true;
        }
    }
    return false;
}

std::string xmlSanitized(std::string_view text)
{
    std::string out;
    out.reserve(text.size());
    for (const char c : text)
    {
        if (!xmlIsIllegalChar(c))
        {
            out.push_back(c);
        }
    }
    return out;
}

void setAttribute(xml::Node el, const char *name, std::string_view value)
{
    if (xmlNeedsSanitize(value))
    {
        el.appendAttribute(name).setValue(xmlSanitized(value));
        return;
    }
    el.appendAttribute(name).setValue(value);
}

void writeTextElement(xml::Node parent, const char *tag, std::string_view text)
{
    setText(parent.appendChild(tag), text);
}

void setText(xml::Node el, std::string_view text)
{
    if (text.empty())
    {
        return;
    }
    if (xmlNeedsSanitize(text))
    {
        el.appendChild(xml::NodeType::pcdata).setValue(xmlSanitized(text));
        return;
    }
    el.appendChild(xml::NodeType::pcdata).setValue(text);
}

std::string nodePath(xml::Node node)
{
    std::string path;
    for (xml::Node n = node; n && n.type() == xml::NodeType::element; n = n.parent())
    {
        int ordinal = 1;
        bool repeated = false;
        for (xml::Node s = n.previousSibling(); s; s = s.previousSibling())
        {
            if (s.type() == xml::NodeType::element && std::string_view{s.name()} == n.name())
            {
                ++ordinal;
                repeated = true;
            }
        }
        if (!repeated)
        {
            for (xml::Node s = n.nextSibling(); s; s = s.nextSibling())
            {
                if (s.type() == xml::NodeType::element && std::string_view{s.name()} == n.name())
                {
                    repeated = true;
                    break;
                }
            }
        }
        std::string segment = "/";
        segment += n.name();
        if (repeated)
        {
            segment += "[";
            segment += formatInt(ordinal);
            segment += "]";
        }
        path.insert(0, segment);
    }
    return path.empty() ? "/" : path;
}

[[noreturn]] void throwUnknownElement(xml::Node el)
{
    throw ParseError{
        Error{ErrorCode::unknownElement, nodePath(el), std::string{"unknown element <"} + el.name() + ">"}};
}

[[noreturn]] void throwMisplacedElement(xml::Node el)
{
    throw ParseError{
        Error{ErrorCode::wrongElementOrder, nodePath(el), std::string{"element <"} + el.name() + "> is out of order"}};
}

[[noreturn]] void throwUnknownAttribute(xml::Node el, const char *attribute_name)
{
    throw ParseError{Error{ErrorCode::unknownAttribute, nodePath(el),
                           std::string{"unknown attribute \""} + attribute_name + "\" on <" + el.name() + ">"}};
}

[[noreturn]] void throwMissingElement(xml::Node parent, const char *missing)
{
    throw ParseError{Error{ErrorCode::missingRequiredElement, nodePath(parent),
                           std::string{"missing required element <"} + missing + "> in <" + parent.name() + ">"}};
}

[[noreturn]] void throwMissingOrMisplaced(xml::Node parent, xml::Node cursor, const char *missing)
{
    for (xml::Node n = cursor; n; n = n.nextSibling())
    {
        if (n.type() == xml::NodeType::element && std::string_view{n.name()} == missing)
        {
            throwMisplacedElement(n);
        }
    }
    throwMissingElement(parent, missing);
}

[[noreturn]] void throwMissingAttribute(xml::Node el, const char *attribute_name)
{
    throw ParseError{
        Error{ErrorCode::missingRequiredAttribute, nodePath(el),
              std::string{"missing required attribute \""} + attribute_name + "\" on <" + el.name() + ">"}};
}

[[noreturn]] void throwTooManyElements(xml::Node el)
{
    throw ParseError{
        Error{ErrorCode::tooManyElements, nodePath(el), std::string{"too many <"} + el.name() + "> elements"}};
}

} // namespace mx::core
