// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/Fixer.h"

#include <string_view>

namespace mxtest
{
namespace corert
{

namespace
{

std::string sidecarPath(const std::string &inputPath)
{
    const auto dot = inputPath.find_last_of('.');
    if (dot == std::string::npos)
    {
        return {};
    }
    return inputPath.substr(0, dot) + ".fixup.xml";
}

void applyRecursive(const std::vector<Fixup> &fixups, mxpugi::xml_node el)
{
    for (const Fixup &fx : fixups)
    {
        if (fx.type == Fixup::Type::Element)
        {
            if (std::string_view{el.name()} == fx.name)
            {
                // Match the element's direct text (an empty-element leaf
                // matches only an empty fixup value, so intentionally-empty
                // markers sharing a name with a clamped leaf are safe).
                if (std::string_view{el.text().get()} == fx.value)
                {
                    el.text().set(fx.replacementValue.c_str());
                }
            }
        }
        else
        {
            for (mxpugi::xml_attribute a : el.attributes())
            {
                if (std::string_view{a.name()} == fx.name && std::string_view{a.value()} == fx.value)
                {
                    a.set_value(fx.replacementValue.c_str());
                }
            }
        }
    }
    for (mxpugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == mxpugi::node_element)
        {
            applyRecursive(fixups, c);
        }
    }
}

} // namespace

Fixer::Fixer(const std::string &inputFilePath)
{
    const std::string path = sidecarPath(inputFilePath);
    if (path.empty())
    {
        return;
    }
    mxpugi::xml_document doc;
    if (!doc.load_file(path.c_str()))
    {
        return;
    }
    mxpugi::xml_node root = doc.document_element();
    if (!root || std::string_view{root.name()} != "fixups")
    {
        return;
    }
    for (mxpugi::xml_node replace = root.child("replace"); replace; replace = replace.next_sibling("replace"))
    {
        Fixup fx;
        const std::string_view type = replace.child("type").text().get();
        if (type == "element")
        {
            fx.type = Fixup::Type::Element;
        }
        else if (type == "attribute")
        {
            fx.type = Fixup::Type::Attribute;
        }
        else
        {
            continue; // unknown operation flavors are skipped, not errors
        }
        fx.name = replace.child("name").text().get();
        fx.value = replace.child("value").text().get();
        mxpugi::xml_node replacement = replace.child("replacement-value");
        if (fx.name.empty() || !replacement)
        {
            continue;
        }
        fx.replacementValue = replacement.text().get();
        myFixups.push_back(std::move(fx));
    }
}

void Fixer::applyToExpected(mxpugi::xml_document &expected) const
{
    if (myFixups.empty() || !expected.document_element())
    {
        return;
    }
    applyRecursive(myFixups, expected.document_element());
}

} // namespace corert
} // namespace mxtest
