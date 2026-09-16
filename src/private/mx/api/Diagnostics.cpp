// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Diagnostics.h"

#include "mx/api/LocationFormatting.h"

#include <utility>

namespace mx
{
namespace api
{
Diagnostics::Diagnostics(DiagnosticHandler handler) : myHandler{std::move(handler)}
{
}

void Diagnostics::add(Diagnostic diagnostic)
{
    myDiagnostics.push_back(std::move(diagnostic));
    if (myHandler)
    {
        myHandler(myDiagnostics.back());
    }
}

std::span<const Diagnostic> Diagnostics::all() const noexcept
{
    return myDiagnostics;
}

bool Diagnostics::hasSeverityOrWorse(Severity threshold) const noexcept
{
    for (const auto &diagnostic : myDiagnostics)
    {
        if (diagnostic.severity >= threshold)
        {
            return true;
        }
    }
    return false;
}

std::string formatDiagnostic(const Diagnostic &diagnostic)
{
    const char *severityName = "warning";
    switch (diagnostic.severity)
    {
    case Severity::info:
        severityName = "info";
        break;
    case Severity::warning:
        severityName = "warning";
        break;
    case Severity::error:
        severityName = "error";
        break;
    }

    std::string text{"mx: "};
    text += severityName;
    appendFormattedLocationAndMessage(text, diagnostic.location, diagnostic.message);
    return text;
}
} // namespace api
} // namespace mx
