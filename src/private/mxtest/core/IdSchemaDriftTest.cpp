// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Drift guard for the hand-written identity-attribute table in
// IdIntegrity.cpp. This re-derives the table from the schema and fails while
// the two disagree, so an xs:ID or xs:IDREF added by a later MusicXML version
// gets noticed here. A reference's target element is documentation only, so
// the schema cannot be asked about it and this does not check it.

#include "cpul/cpulTestHarness.h"

#include "mx/core/IdIntegrity.h"

#include "mxtest/file/PathRoot.h"

#include "pugixml.hpp"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>

using namespace mx::core;

// (attribute name, "ID" or "IDREF"), keyed by element name.
using IdSchemaAttributes = std::set<std::pair<std::string, std::string>>;
using IdSchemaByElement = std::map<std::string, IdSchemaAttributes>;

struct IdSchema
{
    std::map<std::string_view, pugi::xml_node> attributeGroups;
    std::map<std::string_view, pugi::xml_node> complexTypes;
};

inline std::string_view idSchemaLocalName(const char *qualified)
{
    const std::string_view name{qualified};
    const auto colon = name.find(':');
    return colon == std::string_view::npos ? name : name.substr(colon + 1);
}

/// The identity attributes a complexType or attributeGroup owns, following
/// attributeGroup references and extension bases. A child element
/// declaration owns whatever it declares, so the walk stops there.
inline void idSchemaCollectAttributes(const IdSchema &schema, pugi::xml_node node, std::set<pugi::xml_node> &visited,
                                      IdSchemaAttributes &out)
{
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
    {
        const std::string_view tag = idSchemaLocalName(child.name());
        if (tag == "element" || tag == "annotation")
        {
            continue;
        }
        if (tag == "attribute")
        {
            const std::string_view type = idSchemaLocalName(child.attribute("type").value());
            if (type == "ID" || type == "IDREF")
            {
                out.emplace(child.attribute("name").value(), std::string{type});
            }
            continue;
        }
        if (tag == "attributeGroup")
        {
            const auto found = schema.attributeGroups.find(idSchemaLocalName(child.attribute("ref").value()));
            if (found != schema.attributeGroups.end() && visited.insert(found->second).second)
            {
                idSchemaCollectAttributes(schema, found->second, visited, out);
            }
            continue;
        }
        if (tag == "extension" || tag == "restriction")
        {
            const auto found = schema.complexTypes.find(idSchemaLocalName(child.attribute("base").value()));
            if (found != schema.complexTypes.end() && visited.insert(found->second).second)
            {
                idSchemaCollectAttributes(schema, found->second, visited, out);
            }
        }
        idSchemaCollectAttributes(schema, child, visited, out);
    }
}

/// The complexType an element declaration uses, named or inline.
inline pugi::xml_node idSchemaTypeOf(const IdSchema &schema, pugi::xml_node element)
{
    const auto named = schema.complexTypes.find(idSchemaLocalName(element.attribute("type").value()));
    if (named != schema.complexTypes.end())
    {
        return named->second;
    }
    for (pugi::xml_node child = element.first_child(); child; child = child.next_sibling())
    {
        if (idSchemaLocalName(child.name()) == "complexType")
        {
            return child;
        }
    }
    return pugi::xml_node{};
}

inline void idSchemaCollectElements(const IdSchema &schema, pugi::xml_node node, IdSchemaByElement &out)
{
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
    {
        if (idSchemaLocalName(child.name()) == "element" && child.attribute("name"))
        {
            if (const pugi::xml_node type = idSchemaTypeOf(schema, child))
            {
                IdSchemaAttributes attributes;
                std::set<pugi::xml_node> visited;
                idSchemaCollectAttributes(schema, type, visited, attributes);
                if (!attributes.empty())
                {
                    IdSchemaAttributes &entry = out[child.attribute("name").value()];
                    entry.insert(attributes.begin(), attributes.end());
                }
            }
        }
        idSchemaCollectElements(schema, child, out);
    }
}

TEST(TableMatchesSchema, IdSchemaDrift)
{
    const std::string schemaPath = std::string{MX_REPO_ROOT_PATH} + "/docs/musicxml-4.0-ed15c23.xsd";
    pugi::xml_document schemaDoc;
    CHECK(schemaDoc.load_file(schemaPath.c_str()));

    IdSchema schema;
    for (pugi::xml_node child = schemaDoc.document_element().first_child(); child; child = child.next_sibling())
    {
        const std::string_view tag = idSchemaLocalName(child.name());
        if (!child.attribute("name"))
        {
            continue;
        }
        if (tag == "attributeGroup")
        {
            schema.attributeGroups.emplace(child.attribute("name").value(), child);
        }
        else if (tag == "complexType")
        {
            schema.complexTypes.emplace(child.attribute("name").value(), child);
        }
    }

    IdSchemaByElement derived;
    idSchemaCollectElements(schema, schemaDoc, derived);
    CHECK(!derived.empty());

    IdSchemaByElement fromTable;
    for (const IdAttribute &entry : idAttributes())
    {
        // One entry per element name: the lookup in IdIntegrity.cpp
        // classifies by name alone, so a second entry would break it.
        CHECK(fromTable[entry.element].empty());
        fromTable[entry.element].emplace(entry.attribute, entry.kind == IdAttributeKind::definition ? "ID" : "IDREF");
    }

    // The schema has to agree that one name means one thing: an element
    // declared in two places must type its identity attribute the same way
    // in both.
    for (const auto &derivedEntry : derived)
    {
        CHECK_EQUAL(std::size_t{1}, derivedEntry.second.size());
    }

    std::string drift;
    for (const auto &derivedEntry : derived)
    {
        const auto found = fromTable.find(derivedEntry.first);
        if (found == fromTable.end())
        {
            drift += "missing from the table: " + derivedEntry.first + "\n";
        }
        else if (found->second != derivedEntry.second)
        {
            drift += "table disagrees with the schema: " + derivedEntry.first + "\n";
        }
    }
    for (const auto &tableEntry : fromTable)
    {
        if (derived.find(tableEntry.first) == derived.end())
        {
            drift += "not in the schema: " + tableEntry.first + "\n";
        }
    }
    CHECK_EQUAL(std::string{}, drift);
}

TEST(EveryReferenceNamesATarget, IdSchemaDrift)
{
    // The schema cannot express the target, so nothing above catches a new
    // reference entry that was left without one.
    for (const IdAttribute &entry : idAttributes())
    {
        const bool hasTarget = !std::string_view{entry.target}.empty();
        CHECK_EQUAL(entry.kind == IdAttributeKind::reference, hasTarget);
    }
}
