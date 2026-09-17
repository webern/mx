// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Diagnostics.h"
#include "mx/impl/MeasureCursor.h"

#include <functional>
#include <memory>
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

    explicit DiagnosticsContext(api::Diagnostics &diagnostics)
        : myDiagnostics{std::make_shared<std::reference_wrapper<api::Diagnostics>>(diagnostics)}
    {
    }

    void report(api::Severity severity, api::DiagnosticCode code, api::Location location, std::string message) const
    {
        if (myDiagnostics)
        {
            myDiagnostics->get().add(api::Diagnostic{severity, code, std::move(location), std::move(message)});
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
    std::shared_ptr<std::reference_wrapper<api::Diagnostics>> myDiagnostics;
};

// The part, measure and tick of a cursor, for a report that is not about one staff or voice.
inline api::Location measureLocation(const MeasureCursor &cursor)
{
    api::Location location;
    location.partIndex = cursor.partIndex;
    location.measureIndex = cursor.measureIndex;
    location.tickTimePosition = cursor.tickTimePosition;
    return location;
}
} // namespace impl
} // namespace mx
