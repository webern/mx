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
    valueAdjusted,         // a value was changed to one that MusicXML can represent
    unmatchedSpanner,      // a spanner endpoint had no matching endpoint
    invalidValue,          // a value could not be read, so a default was used
    missingValueDefaulted, // a required value was missing, so a default was used
    droppedData,           // data could not be read or written, so it was left out
    duplicateId,           // two elements claimed one ID, which must be unique
    danglingIdReference    // an ID reference did not name the ID it should have
};

// A non-fatal problem noticed while producing a score or MusicXML document.
struct Diagnostic
{
    Severity severity = Severity::warning;               // effect on the translated value
    DiagnosticCode code = DiagnosticCode::valueAdjusted; // kind of recovery performed
    Location location;                                   // source or output score position, when known
    std::string message;                                 // human-readable description
};

// Receives a diagnostic while a score or MusicXML document is translated.
using DiagnosticHandler = std::function<void(const Diagnostic &)>;

// Collects diagnostics from one or more translations. A handler, when supplied,
// runs as each diagnostic is added.
class Diagnostics
{
  public:
    Diagnostics() = default;
    explicit Diagnostics(DiagnosticHandler handler);

    // Adds a diagnostic to this collector and calls its handler.
    void add(Diagnostic diagnostic);

    // All diagnostics collected so far, in the order they were reported.
    std::span<const Diagnostic> all() const noexcept;

    // True when the collection contains this severity or a more severe one.
    bool hasSeverityOrWorse(Severity threshold) const noexcept;

  private:
    std::vector<Diagnostic> myDiagnostics;
    DiagnosticHandler myHandler;
};

// Writes a diagnostic on one line for logs and test output.
std::string formatDiagnostic(const Diagnostic &diagnostic);
} // namespace api
} // namespace mx
