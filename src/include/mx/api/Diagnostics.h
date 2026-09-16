// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Result.h"

#include <functional>
#include <span>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// How a successful translation affected the resulting score or document.
enum class Severity
{
    info,    // noteworthy normalization without meaningful loss
    warning, // adjusted data; the result remains usable
    error    // omitted data; the operation still produced a value
};

// The recoverable decision reported by a diagnostic.
enum class DiagnosticCode
{
    valueAdjusted,
    unmatchedSpanner
};

// A non-fatal problem noticed while producing a value.
struct Diagnostic
{
    Severity severity = Severity::warning;
    DiagnosticCode code = DiagnosticCode::valueAdjusted;
    Location location;
    std::string message;
};

using DiagnosticHandler = std::function<void(const Diagnostic &)>;

// Collects diagnostics from one or more translations. A handler, when supplied,
// runs as each diagnostic is added.
class Diagnostics
{
  public:
    Diagnostics() = default;
    explicit Diagnostics(DiagnosticHandler handler);

    void add(Diagnostic diagnostic);
    std::span<const Diagnostic> all() const noexcept;
    bool hasSeverityOrWorse(Severity threshold) const noexcept;

  private:
    std::vector<Diagnostic> myDiagnostics;
    DiagnosticHandler myHandler;
};

// Writes a diagnostic on one line for logs and test output.
std::string formatDiagnostic(const Diagnostic &diagnostic);
} // namespace api
} // namespace mx
