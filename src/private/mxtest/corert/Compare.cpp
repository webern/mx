// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/Compare.h"

#include "mxtest/import/Normalize.h"

#include <cmath>
#include <optional>
#include <string_view>

namespace mxtest
{
namespace corert
{

namespace
{

// Numeric equivalence: if both strings parse as numbers, compare values
// instead of spellings. Float comparison uses epsilon < 0.00000001.
std::optional<long double> parseLongDouble(const std::string &str)
{
    try
    {
        size_t pos = 0;
        const long double val = std::stold(str, &pos);
        if (pos != str.size() || std::isnan(val) || std::isinf(val))
        {
            return std::nullopt;
        }
        return val;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

bool isEquivalent(const std::string &left, const std::string &right)
{
    if (left == right)
    {
        return true;
    }
    const auto l = parseLongDouble(left);
    const auto r = parseLongDouble(right);
    return l && r && std::fabs(*l - *r) < 0.00000001L;
}

std::string nodePathString(pugi::xml_node node)
{
    std::string path;
    for (pugi::xml_node n = node; n && n.type() == pugi::node_element; n = n.parent())
    {
        path.insert(0, std::string{"/"} + n.name());
    }
    return path.empty() ? "/" : path;
}

} // namespace

void normalizeForComparison(pugi::xml_document &doc)
{
    setRootMusicXmlVersion(doc, kMusicXmlVersionBaseline);
    stripInterElementWhitespace(doc);
    stripZerosFromDecimalFields(doc);
    normalizeCommaSeparatedFields(doc);
    sortAttributes(doc); // must be last
}

CompareFailure compareElements(pugi::xml_node expected, pugi::xml_node actual)
{
    CompareFailure fail;

    if (std::string_view{expected.name()} != actual.name())
    {
        fail.isFailure = true;
        fail.detail = "element name mismatch at " + nodePathString(expected) + ": expected '" + expected.name() +
                      "', actual '" + actual.name() + "'";
        return fail;
    }

    const std::string expectedText = expected.text().get();
    const std::string actualText = actual.text().get();
    if (!isEquivalent(expectedText, actualText))
    {
        fail.isFailure = true;
        fail.detail = "text mismatch at " + nodePathString(expected) + ": expected '" + expectedText + "', actual '" +
                      actualText + "'";
        return fail;
    }

    pugi::xml_attribute ea = expected.first_attribute();
    pugi::xml_attribute aa = actual.first_attribute();
    while (ea && aa)
    {
        if (std::string_view{ea.name()} != aa.name() || !isEquivalent(ea.value(), aa.value()))
        {
            fail.isFailure = true;
            fail.detail = "attribute mismatch at " + nodePathString(expected) + ": expected '" + ea.name() + "=" +
                          ea.value() + "', actual '" + aa.name() + "=" + aa.value() + "'";
            return fail;
        }
        ea = ea.next_attribute();
        aa = aa.next_attribute();
    }
    if (ea || aa)
    {
        fail.isFailure = true;
        fail.detail = "attribute count mismatch at " + nodePathString(expected);
        return fail;
    }

    pugi::xml_node ec = expected.first_child();
    pugi::xml_node ac = actual.first_child();
    while (true)
    {
        while (ec && ec.type() != pugi::node_element)
        {
            ec = ec.next_sibling();
        }
        while (ac && ac.type() != pugi::node_element)
        {
            ac = ac.next_sibling();
        }
        if (!ec || !ac)
        {
            break;
        }
        const auto childFail = compareElements(ec, ac);
        if (childFail.isFailure)
        {
            return childFail;
        }
        ec = ec.next_sibling();
        ac = ac.next_sibling();
    }
    if (ec || ac)
    {
        fail.isFailure = true;
        fail.detail = "child count mismatch at " + nodePathString(expected);
        return fail;
    }

    return fail;
}

} // namespace corert
} // namespace mxtest
