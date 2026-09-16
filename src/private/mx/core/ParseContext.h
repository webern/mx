// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include "mx/core/Decimal.h"
#include "mx/core/Lexical.h"

#include "pugixml.hpp"

#include <functional>
#include <string>
#include <string_view>

namespace mx::core
{

/// The repairs the lenient parsers make on import. The parsed document is
/// still valid; a diagnostic tells the caller what was changed.
enum class DiagnosticCode
{
    invalidValue,
    valueAdjusted,
    missingValueDefaulted,
};

struct Diagnostic
{
    DiagnosticCode code;
    std::string path; // e.g. /score-partwise/part[1]/measure[3]/note[2]
    std::string message;
};

using DiagnosticHandler = std::function<void(const Diagnostic &)>;

/// Passed down through the generated parsers so they can report repairs. A
/// default-constructed context discards every report.
class ParseContext
{
  public:
    ParseContext() = default;
    explicit ParseContext(DiagnosticHandler handler);

    void report(Diagnostic diagnostic) const;

  private:
    DiagnosticHandler m_handler;
};

/// Reports that the text of `el`, or of its `attribute` when that is not
/// null, was replaced by `replacement`.
void reportValueRepair(const ParseContext &context, ValueParseOutcome outcome, pugi::xml_node el, const char *attribute,
                       std::string_view text, std::string_view replacement);

void reportAttributeDefaulted(const ParseContext &context, pugi::xml_node el, const char *attribute,
                              std::string_view value);

/// The lenient parse of a value type, reporting any repair it made. A type
/// without an outcome-reporting parse is parsed silently.
template <typename T>
T parseValue(std::string_view text, const ParseContext &context, pugi::xml_node el, const char *attribute)
{
    if constexpr (requires(std::string_view s, ValueParseOutcome &o) { T::parse(s, o); })
    {
        ValueParseOutcome outcome = ValueParseOutcome::valid;
        T value = T::parse(text, outcome);
        if (outcome != ValueParseOutcome::valid)
        {
            reportValueRepair(context, outcome, el, attribute, text, std::string{value.toString()});
        }
        return value;
    }
    else
    {
        return T::parse(text);
    }
}

int parseIntegerValue(std::string_view text, const ParseContext &context, pugi::xml_node el, const char *attribute);

Decimal parseDecimalValue(std::string_view text, const ParseContext &context, pugi::xml_node el, const char *attribute);

} // namespace mx::core
