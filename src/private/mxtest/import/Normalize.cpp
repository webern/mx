// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/import/Normalize.h"

#include "mxtest/import/CommaSeparatedFields.h"
#include "mxtest/import/DecimalFields.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace mxtest
{

namespace
{

bool isBlankText(mxpugi::xml_node node)
{
    if (node.type() != mxpugi::node_pcdata && node.type() != mxpugi::node_cdata)
    {
        return false;
    }
    const std::string_view text{node.value()};
    return text.find_first_not_of(" \t\r\n") == std::string_view::npos;
}

void stripWhitespaceRecursive(mxpugi::xml_node el)
{
    mxpugi::xml_node child = el.first_child();
    while (child)
    {
        mxpugi::xml_node next = child.next_sibling();
        if (isBlankText(child))
        {
            el.remove_child(child);
        }
        else if (child.type() == mxpugi::node_element)
        {
            stripWhitespaceRecursive(child);
        }
        child = next;
    }
}

void stripZerosRecursive(mxpugi::xml_node el)
{
    bool hasElementChildren = false;
    for (mxpugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == mxpugi::node_element)
        {
            hasElementChildren = true;
            break;
        }
    }
    if (!hasElementChildren && decimalFields.count(el.name()))
    {
        const std::string text = el.text().get();
        if (!text.empty())
        {
            el.text().set(stripZeros(text).c_str());
        }
    }
    for (mxpugi::xml_attribute a : el.attributes())
    {
        if (decimalFields.count(a.name()))
        {
            const std::string value = a.value();
            if (!value.empty())
            {
                a.set_value(stripZeros(value).c_str());
            }
        }
    }
    for (mxpugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == mxpugi::node_element)
        {
            stripZerosRecursive(c);
        }
    }
}

// Splits a comma-separated-text value on ',', trims surrounding whitespace
// from each item, and rejoins with ", " -- the spelling mx's api round trip
// always produces.
std::string respaceCommaSeparatedList(const std::string &value)
{
    std::string out;
    size_t start = 0;
    while (true)
    {
        const size_t comma = value.find(',', start);
        const size_t end = comma == std::string::npos ? value.size() : comma;
        size_t itemBegin = start;
        size_t itemEnd = end;
        while (itemBegin < itemEnd && std::isspace(static_cast<unsigned char>(value[itemBegin])))
        {
            ++itemBegin;
        }
        while (itemEnd > itemBegin && std::isspace(static_cast<unsigned char>(value[itemEnd - 1])))
        {
            --itemEnd;
        }
        if (!out.empty())
        {
            out += ", ";
        }
        out += value.substr(itemBegin, itemEnd - itemBegin);
        if (comma == std::string::npos)
        {
            break;
        }
        start = comma + 1;
    }
    return out;
}

void normalizeCommaSeparatedFieldsRecursive(mxpugi::xml_node el)
{
    for (mxpugi::xml_attribute a : el.attributes())
    {
        if (commaSeparatedFields.count(a.name()))
        {
            const std::string value = a.value();
            if (!value.empty())
            {
                a.set_value(respaceCommaSeparatedList(value).c_str());
            }
        }
    }
    for (mxpugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == mxpugi::node_element)
        {
            normalizeCommaSeparatedFieldsRecursive(c);
        }
    }
}

void sortAttributesRecursive(mxpugi::xml_node el)
{
    std::vector<std::pair<std::string, std::string>> attrs;
    for (mxpugi::xml_attribute a : el.attributes())
    {
        attrs.emplace_back(a.name(), a.value());
    }
    if (attrs.size() > 1)
    {
        std::sort(attrs.begin(), attrs.end());
        while (el.first_attribute())
        {
            el.remove_attribute(el.first_attribute());
        }
        for (const auto &[name, value] : attrs)
        {
            el.append_attribute(name.c_str()).set_value(value.c_str());
        }
    }
    for (mxpugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == mxpugi::node_element)
        {
            sortAttributesRecursive(c);
        }
    }
}

} // namespace

void setRootMusicXmlVersion(mxpugi::xml_document &doc, const char *version)
{
    mxpugi::xml_node root = doc.document_element();
    if (!root)
    {
        return;
    }
    mxpugi::xml_attribute attr = root.attribute("version");
    if (!attr)
    {
        attr = root.append_attribute("version");
    }
    attr.set_value(version);
}

void stripInterElementWhitespace(mxpugi::xml_document &doc)
{
    if (doc.document_element())
    {
        stripWhitespaceRecursive(doc.document_element());
    }
}

void stripZerosFromDecimalFields(mxpugi::xml_document &doc)
{
    if (doc.document_element())
    {
        stripZerosRecursive(doc.document_element());
    }
}

void normalizeCommaSeparatedFields(mxpugi::xml_document &doc)
{
    if (doc.document_element())
    {
        normalizeCommaSeparatedFieldsRecursive(doc.document_element());
    }
}

void sortAttributes(mxpugi::xml_document &doc)
{
    if (doc.document_element())
    {
        sortAttributesRecursive(doc.document_element());
    }
}

} // namespace mxtest
