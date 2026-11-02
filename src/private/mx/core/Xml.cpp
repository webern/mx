// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Xml.h"

#include "mx/core/Error.h"
#include "mx/core/Lexical.h"

namespace mx::core
{

mxpugi::xml_node firstElement(mxpugi::xml_node parent)
{
    for (mxpugi::xml_node n = parent.first_child(); n; n = n.next_sibling())
    {
        if (n.type() == mxpugi::node_element)
        {
            return n;
        }
    }
    return {};
}

mxpugi::xml_node nextElement(mxpugi::xml_node node)
{
    for (mxpugi::xml_node n = node.next_sibling(); n; n = n.next_sibling())
    {
        if (n.type() == mxpugi::node_element)
        {
            return n;
        }
    }
    return {};
}

bool cursorIs(mxpugi::xml_node node, std::string_view name)
{
    return node && std::string_view{node.name()} == name;
}

const char *childText(mxpugi::xml_node el)
{
    return el.text().get();
}

bool isNamespaceDecl(std::string_view attribute_name)
{
    if (attribute_name == "xmlns")
    {
        return true;
    }
    return attribute_name.size() > 6 && attribute_name.substr(0, 6) == "xmlns:";
}

namespace
{

// XML 1.0 forbids the C0 control characters other than tab/LF/CR; pugixml
// would escape them as character references that are THEMSELVES illegal
// (&#01;), producing a non-well-formed document. Every generated write
// funnels through the helpers below, so dropping the illegal characters
// here makes them unrepresentable through any string-typed setter (the
// §2.2.2 deterministic-repair policy applied at the serialization door).
bool isIllegalXmlChar(char c) noexcept
{
    const auto u = static_cast<unsigned char>(c);
    return u < 0x20 && c != '\t' && c != '\n' && c != '\r';
}

bool needsXmlSanitize(std::string_view text) noexcept
{
    for (const char c : text)
    {
        if (isIllegalXmlChar(c))
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
        if (!isIllegalXmlChar(c))
        {
            out.push_back(c);
        }
    }
    return out;
}

} // namespace

void setAttribute(mxpugi::xml_node el, const char *name, std::string_view value)
{
    if (needsXmlSanitize(value))
    {
        const std::string clean = xmlSanitized(value);
        el.append_attribute(name).set_value(clean.data(), clean.size());
        return;
    }
    el.append_attribute(name).set_value(value.data(), value.size());
}

void writeTextElement(mxpugi::xml_node parent, const char *tag, std::string_view text)
{
    setText(parent.append_child(tag), text);
}

void setText(mxpugi::xml_node el, std::string_view text)
{
    if (text.empty())
    {
        return;
    }
    if (needsXmlSanitize(text))
    {
        const std::string clean = xmlSanitized(text);
        el.append_child(mxpugi::node_pcdata).set_value(clean.data(), clean.size());
        return;
    }
    el.append_child(mxpugi::node_pcdata).set_value(text.data(), text.size());
}

std::string nodePath(mxpugi::xml_node node)
{
    std::string path;
    for (mxpugi::xml_node n = node; n && n.type() == mxpugi::node_element; n = n.parent())
    {
        int ordinal = 1;
        bool repeated = false;
        for (mxpugi::xml_node s = n.previous_sibling(); s; s = s.previous_sibling())
        {
            if (s.type() == mxpugi::node_element && std::string_view{s.name()} == n.name())
            {
                ++ordinal;
                repeated = true;
            }
        }
        if (!repeated)
        {
            for (mxpugi::xml_node s = n.next_sibling(); s; s = s.next_sibling())
            {
                if (s.type() == mxpugi::node_element && std::string_view{s.name()} == n.name())
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

[[noreturn]] void throwUnknownElement(mxpugi::xml_node el)
{
    throw ParseError{
        Error{ErrorCode::unknownElement, nodePath(el), std::string{"unknown element <"} + el.name() + ">"}};
}

[[noreturn]] void throwMisplacedElement(mxpugi::xml_node el)
{
    throw ParseError{
        Error{ErrorCode::wrongElementOrder, nodePath(el), std::string{"element <"} + el.name() + "> is out of order"}};
}

[[noreturn]] void throwUnknownAttribute(mxpugi::xml_node el, const char *attribute_name)
{
    throw ParseError{Error{ErrorCode::unknownAttribute, nodePath(el),
                           std::string{"unknown attribute \""} + attribute_name + "\" on <" + el.name() + ">"}};
}

[[noreturn]] void throwMissingElement(mxpugi::xml_node parent, const char *missing)
{
    throw ParseError{Error{ErrorCode::missingRequiredElement, nodePath(parent),
                           std::string{"missing required element <"} + missing + "> in <" + parent.name() + ">"}};
}

[[noreturn]] void throwMissingOrMisplaced(mxpugi::xml_node parent, mxpugi::xml_node cursor, const char *missing)
{
    for (mxpugi::xml_node n = cursor; n; n = n.next_sibling())
    {
        if (n.type() == mxpugi::node_element && std::string_view{n.name()} == missing)
        {
            throwMisplacedElement(n);
        }
    }
    throwMissingElement(parent, missing);
}

[[noreturn]] void throwMissingAttribute(mxpugi::xml_node el, const char *attribute_name)
{
    throw ParseError{
        Error{ErrorCode::missingRequiredAttribute, nodePath(el),
              std::string{"missing required attribute \""} + attribute_name + "\" on <" + el.name() + ">"}};
}

[[noreturn]] void throwTooManyElements(mxpugi::xml_node el)
{
    throw ParseError{
        Error{ErrorCode::tooManyElements, nodePath(el), std::string{"too many <"} + el.name() + "> elements"}};
}

} // namespace mx::core
