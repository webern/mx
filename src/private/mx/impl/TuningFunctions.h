// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Diagnostics.h"
#include "mx/core/generated/TuningGroup.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DiagnosticsContext.h"

#include <string>

namespace mx
{
namespace impl
{

// <scordatura> and <staff-tuning> both hold a tuning-step, an optional tuning-alter and a
// tuning-octave. The api structs that hold them (AccordData, StaffTuningData) name those fields the
// same way, so one template reads and writes both.
template <typename TUNING_DATA_TYPE>
void readTuningGroup(const core::TuningGroup &inTuning, const Converter &inConverter, TUNING_DATA_TYPE &outData)
{
    outData.tuningStep = inConverter.convert(inTuning.tuningStep());
    if (inTuning.tuningAlter().has_value())
    {
        const auto semitonesAndCents =
            Converter::convertToSemitonesAndCents(static_cast<double>(inTuning.tuningAlter()->value().value()));
        outData.tuningAlter = semitonesAndCents.first;
        outData.tuningCents = semitonesAndCents.second;
    }
    outData.tuningOctave = inTuning.tuningOctave().value();
}

// inElementName names the element for diagnostics, e.g. "accord" gives "accord tuning-octave 12 is out of
// range; using 9".
template <typename TUNING_DATA_TYPE>
core::TuningGroup makeTuningGroup(const TUNING_DATA_TYPE &inData, const Converter &inConverter,
                                  const DiagnosticsContext &inDiagnostics, const api::Location &inLocation,
                                  const std::string &inElementName)
{
    core::TuningGroup tuning{};
    if (inData.tuningStep == api::Step::unspecified)
    {
        inDiagnostics.report(api::Severity::warning, api::DiagnosticCode::missingValueDefaulted, inLocation,
                             inElementName + " tuning-step is unspecified; using C");
    }
    tuning.setTuningStep(inConverter.convert(inData.tuningStep));
    if (inData.tuningAlter != 0 || inData.tuningCents != 0.0)
    {
        tuning.setTuningAlter(
            core::Semitones{core::Decimal{Converter::convertToAlter(inData.tuningAlter, inData.tuningCents)}});
    }
    const core::Octave tuningOctave{inData.tuningOctave};
    reportAdjusted(inDiagnostics, inLocation, inElementName + " tuning-octave", inData.tuningOctave,
                   tuningOctave.value());
    tuning.setTuningOctave(tuningOctave);
    return tuning;
}

} // namespace impl
} // namespace mx
