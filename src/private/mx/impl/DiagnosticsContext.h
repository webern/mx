// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Diagnostics.h"
#include "mx/core/Lexical.h"
#include "mx/impl/MeasureCursor.h"

#include <cmath>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
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

// The part and measure of a cursor, for a report about a whole measure.
inline api::Location measureOnlyLocation(const MeasureCursor &cursor)
{
    api::Location location;
    location.partIndex = cursor.partIndex;
    location.measureIndex = cursor.measureIndex;
    return location;
}

inline std::string formatReportedNumber(double value)
{
    return core::formatDouble(value);
}

inline std::string formatReportedNumber(long long value)
{
    return std::to_string(value);
}

// Reports when a core value mx built to write differs from the api value it was built from, because
// the core type holds a narrower range. Pass the value read back from the built core value.
template <typename T>
bool reportAdjusted(const DiagnosticsContext &diagnostics, api::Location location, const std::string &name, T apiValue,
                    T writtenValue)
{
    const bool isAdjusted =
        std::is_floating_point_v<T> ? std::abs(apiValue - writtenValue) > 1e-9 : apiValue != writtenValue;
    if (isAdjusted)
    {
        using Format = std::conditional_t<std::is_floating_point_v<T>, double, long long>;
        diagnostics.report(api::Severity::warning, api::DiagnosticCode::valueAdjusted, std::move(location),
                           name + " " + formatReportedNumber(static_cast<Format>(apiValue)) +
                               " is out of range; using " + formatReportedNumber(static_cast<Format>(writtenValue)));
    }
    return isAdjusted;
}
} // namespace impl
} // namespace mx
