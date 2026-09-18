// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/IdIntegrity.h"

#include "mx/core/Lexical.h"
#include "mx/core/Token.h"
#include "mx/core/Xml.h"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace mx::core
{

const IdAttributeKind kIdDefines = IdAttributeKind::definition;
const IdAttributeKind kIdRefers = IdAttributeKind::reference;

// xs:ID. All but the last four come from the optional-unique-id attribute
// group; bookmark, player, score-instrument and score-part declare their own
// and require it.
const IdAttribute kIdAttributes[] = {
    {"accidental-mark", "id", kIdDefines, ""},
    {"accordion-registration", "id", kIdDefines, ""},
    {"arpeggiate", "id", kIdDefines, ""},
    {"articulations", "id", kIdDefines, ""},
    {"barline", "id", kIdDefines, ""},
    {"beam", "id", kIdDefines, ""},
    {"bracket", "id", kIdDefines, ""},
    {"clef", "id", kIdDefines, ""},
    {"coda", "id", kIdDefines, ""},
    {"credit", "id", kIdDefines, ""},
    {"credit-image", "id", kIdDefines, ""},
    {"credit-symbol", "id", kIdDefines, ""},
    {"credit-words", "id", kIdDefines, ""},
    {"damp", "id", kIdDefines, ""},
    {"damp-all", "id", kIdDefines, ""},
    {"dashes", "id", kIdDefines, ""},
    {"direction", "id", kIdDefines, ""},
    {"direction-type", "id", kIdDefines, ""},
    {"dynamics", "id", kIdDefines, ""},
    {"eyeglasses", "id", kIdDefines, ""},
    {"fermata", "id", kIdDefines, ""},
    {"figured-bass", "id", kIdDefines, ""},
    {"for-part", "id", kIdDefines, ""},
    {"frame", "id", kIdDefines, ""},
    {"glissando", "id", kIdDefines, ""},
    {"grouping", "id", kIdDefines, ""},
    {"harmony", "id", kIdDefines, ""},
    {"harp-pedals", "id", kIdDefines, ""},
    {"image", "id", kIdDefines, ""},
    {"key", "id", kIdDefines, ""},
    {"lyric", "id", kIdDefines, ""},
    {"measure", "id", kIdDefines, ""},
    {"measure-style", "id", kIdDefines, ""},
    {"metronome", "id", kIdDefines, ""},
    {"non-arpeggiate", "id", kIdDefines, ""},
    {"notations", "id", kIdDefines, ""},
    {"note", "id", kIdDefines, ""},
    {"octave-shift", "id", kIdDefines, ""},
    {"ornaments", "id", kIdDefines, ""},
    {"other-direction", "id", kIdDefines, ""},
    {"other-notation", "id", kIdDefines, ""},
    {"pedal", "id", kIdDefines, ""},
    {"percussion", "id", kIdDefines, ""},
    {"principal-voice", "id", kIdDefines, ""},
    {"print", "id", kIdDefines, ""},
    {"rehearsal", "id", kIdDefines, ""},
    {"scordatura", "id", kIdDefines, ""},
    {"segno", "id", kIdDefines, ""},
    {"slide", "id", kIdDefines, ""},
    {"slur", "id", kIdDefines, ""},
    {"sound", "id", kIdDefines, ""},
    {"staff-divide", "id", kIdDefines, ""},
    {"string-mute", "id", kIdDefines, ""},
    {"symbol", "id", kIdDefines, ""},
    {"technical", "id", kIdDefines, ""},
    {"tied", "id", kIdDefines, ""},
    {"time", "id", kIdDefines, ""},
    {"transpose", "id", kIdDefines, ""},
    {"tuplet", "id", kIdDefines, ""},
    {"wedge", "id", kIdDefines, ""},
    {"words", "id", kIdDefines, ""},
    {"bookmark", "id", kIdDefines, ""},
    {"player", "id", kIdDefines, ""},
    {"score-instrument", "id", kIdDefines, ""},
    {"score-part", "id", kIdDefines, ""},

    // xs:IDREF, with the element each one has to name. The schema says so in
    // its documentation; nothing in it enforces the target.
    {"assess", "player", kIdRefers, "player"},
    {"instrument", "id", kIdRefers, "score-instrument"},
    {"instrument-change", "id", kIdRefers, "score-instrument"},
    {"instrument-link", "id", kIdRefers, "score-instrument"},
    {"midi-device", "id", kIdRefers, "score-instrument"},
    {"midi-instrument", "id", kIdRefers, "score-instrument"},
    {"other-listen", "player", kIdRefers, "player"},
    {"other-listening", "player", kIdRefers, "player"},
    {"part", "id", kIdRefers, "score-part"},
    {"play", "id", kIdRefers, "score-instrument"},
    {"sync", "player", kIdRefers, "player"},
    {"wait", "player", kIdRefers, "player"},
};

using IdDefinitions = std::map<std::string, pugi::xml_node>;

// An element name is enough to classify: no MusicXML element declares an
// identity attribute one way in one place and the other way somewhere else.
// IdSchemaDriftTest checks that against the schema.
const IdAttribute *classifyIdAttribute(const char *elementName)
{
    static const std::unordered_map<std::string_view, const IdAttribute *> byName = [] {
        std::unordered_map<std::string_view, const IdAttribute *> out;
        for (const IdAttribute &entry : kIdAttributes)
        {
            out.emplace(entry.element, &entry);
        }
        return out;
    }();
    const auto found = byName.find(std::string_view{elementName});
    return found == byName.end() ? nullptr : found->second;
}

/// The value the parsed document will hold. On the way out this is the
/// identity: a serialized tree holds Token values, which are repaired
/// already.
std::string repairedIdValue(const char *text)
{
    return Token::parse(text).value();
}

/// Visits every element carrying an identity attribute, in document order.
template <typename Fn> void forEachIdAttribute(pugi::xml_node node, const Fn &fn)
{
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
    {
        if (child.type() != pugi::node_element)
        {
            continue;
        }
        if (const IdAttribute *entry = classifyIdAttribute(child.name()))
        {
            if (pugi::xml_attribute attribute = child.attribute(entry->attribute))
            {
                fn(child, attribute, *entry);
            }
        }
        forEachIdAttribute(child, fn);
    }
}

void reportDanglingIdReferences(pugi::xml_node root, const IdDefinitions &definitions, const DiagnosticHandler &handler)
{
    forEachIdAttribute(root, [&](pugi::xml_node el, pugi::xml_attribute attribute, const IdAttribute &entry) {
        if (entry.kind != IdAttributeKind::reference)
        {
            return;
        }
        const std::string value = repairedIdValue(attribute.value());
        std::string message = "ID reference \"";
        message += value;
        message += "\" in attribute \"";
        message += entry.attribute;
        message += "\" ";
        const auto found = definitions.find(value);
        if (found == definitions.end())
        {
            message += "does not match an ID in the document";
        }
        else if (std::string_view{found->second.name()} != entry.target)
        {
            message += "names a <";
            message += found->second.name();
            message += ">, not a <";
            message += entry.target;
            message += ">";
        }
        else
        {
            return;
        }
        handler(Diagnostic{DiagnosticCode::danglingIdReference, nodePath(el), std::move(message)});
    });
}

std::string freshIdValue(const std::string &base, const std::set<std::string> &taken)
{
    for (int suffix = 2;; ++suffix)
    {
        std::string candidate = base + "-" + formatInt(suffix);
        if (taken.find(candidate) == taken.end())
        {
            return candidate;
        }
    }
}

std::span<const IdAttribute> idAttributes()
{
    return std::span<const IdAttribute>{kIdAttributes};
}

void checkIds(const pugi::xml_document &doc, const DiagnosticHandler &handler)
{
    if (!handler)
    {
        // Reading changes nothing, so there is nothing to do when nobody is
        // listening.
        return;
    }

    IdDefinitions definitions;
    forEachIdAttribute(doc, [&](pugi::xml_node el, pugi::xml_attribute attribute, const IdAttribute &entry) {
        if (entry.kind != IdAttributeKind::definition)
        {
            return;
        }
        const auto [found, inserted] = definitions.emplace(repairedIdValue(attribute.value()), el);
        if (!inserted)
        {
            std::string message = "duplicate ID \"";
            message += found->first;
            message += "\"; ";
            message += nodePath(found->second);
            message += " already uses it";
            handler(Diagnostic{DiagnosticCode::duplicateId, nodePath(el), std::move(message)});
        }
    });

    reportDanglingIdReferences(doc, definitions, handler);
}

void repairIds(pugi::xml_document &doc, const DiagnosticHandler &handler)
{
    // Collect every id first, so a rename cannot land on one that appears
    // further down the document.
    std::set<std::string> taken;
    forEachIdAttribute(doc, [&](pugi::xml_node, pugi::xml_attribute attribute, const IdAttribute &entry) {
        if (entry.kind == IdAttributeKind::definition)
        {
            taken.insert(repairedIdValue(attribute.value()));
        }
    });

    IdDefinitions definitions;
    forEachIdAttribute(doc, [&](pugi::xml_node el, pugi::xml_attribute attribute, const IdAttribute &entry) {
        if (entry.kind != IdAttributeKind::definition)
        {
            return;
        }
        const std::string value = repairedIdValue(attribute.value());
        if (definitions.emplace(value, el).second)
        {
            return;
        }
        const std::string replacement = freshIdValue(value, taken);
        attribute.set_value(replacement.c_str());
        taken.insert(replacement);
        definitions.emplace(replacement, el);
        if (handler)
        {
            std::string message = "duplicate ID \"";
            message += value;
            message += "\"; renamed to \"";
            message += replacement;
            message += "\"";
            handler(Diagnostic{DiagnosticCode::duplicateId, nodePath(el), std::move(message)});
        }
    });

    if (handler)
    {
        reportDanglingIdReferences(doc, definitions, handler);
    }
}

} // namespace mx::core
