// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/ParseContext.h"

#include "mx/core/Xml.h"

#include <utility>

namespace mx::core
{

ParseContext::ParseContext(DiagnosticHandler handler) : m_handler{std::move(handler)}
{
}

void ParseContext::report(Diagnostic diagnostic) const
{
    if (m_handler)
    {
        m_handler(diagnostic);
    }
}

void reportValueRepair(const ParseContext &context, ValueParseOutcome outcome, pugi::xml_node el, const char *attribute,
                       std::string_view text, std::string_view replacement)
{
    const bool invalid = outcome == ValueParseOutcome::invalid;
    std::string message = invalid ? "invalid value \"" : "value \"";
    message += text;
    message += "\"";
    if (attribute)
    {
        message += " in attribute \"";
        message += attribute;
        message += "\"";
    }
    message += invalid ? "; using \"" : " adjusted to \"";
    message += replacement;
    message += "\"";
    context.report(Diagnostic{invalid ? DiagnosticCode::invalidValue : DiagnosticCode::valueAdjusted, nodePath(el),
                              std::move(message)});
}

void reportAttributeDefaulted(const ParseContext &context, pugi::xml_node el, const char *attribute,
                              std::string_view value)
{
    std::string message = "missing attribute \"";
    message += attribute;
    message += "\"; using \"";
    message += value;
    message += "\"";
    context.report(Diagnostic{DiagnosticCode::missingValueDefaulted, nodePath(el), std::move(message)});
}

int parseIntegerValue(std::string_view text, const ParseContext &context, pugi::xml_node el, const char *attribute)
{
    int value = 0;
    if (!tryParseInt(text, value))
    {
        reportValueRepair(context, ValueParseOutcome::invalid, el, attribute, text, formatInt(value));
    }
    return value;
}

Decimal parseDecimalValue(std::string_view text, const ParseContext &context, pugi::xml_node el, const char *attribute)
{
    Decimal value;
    if (!Decimal::tryParse(text, value))
    {
        reportValueRepair(context, ValueParseOutcome::invalid, el, attribute, text, value.toString());
    }
    return value;
}

} // namespace mx::core
