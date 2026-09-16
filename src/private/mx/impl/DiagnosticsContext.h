// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Diagnostics.h"
#include "mx/impl/MeasureCursor.h"

#include <string>
#include <utility>

namespace mx
{
namespace impl
{
class DiagnosticsContext
{
  public:
    DiagnosticsContext() = default;

    explicit DiagnosticsContext(api::Diagnostics &diagnostics) : myDiagnostics{&diagnostics}
    {
    }

    void report(api::Severity severity, api::DiagnosticCode code, api::Location location, std::string message) const
    {
        if (myDiagnostics)
        {
            myDiagnostics->add(api::Diagnostic{severity, code, std::move(location), std::move(message)});
        }
    }

    void report(api::Severity severity, api::DiagnosticCode code, const MeasureCursor &cursor,
                std::string message) const
    {
        api::Location location;
        location.partIndex = cursor.partIndex;
        location.measureIndex = cursor.measureIndex;
        location.staffIndex = cursor.staffIndex;
        location.voiceIndex = cursor.voiceIndex;
        location.tickTimePosition = cursor.tickTimePosition;
        report(severity, code, std::move(location), std::move(message));
    }

  private:
    api::Diagnostics *myDiagnostics = nullptr;
};
} // namespace impl
} // namespace mx
