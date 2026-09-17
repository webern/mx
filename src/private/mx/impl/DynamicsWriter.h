// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/impl/Converter.h"
#include "mx/impl/DiagnosticsContext.h"
#include "mx/impl/MeasureCursor.h"

namespace mx
{
namespace impl
{
class DynamicsWriter
{
  public:
    DynamicsWriter(const api::MarkData &inMark, MeasureCursor inCursor, DiagnosticsContext diagnostics = {});
    core::Dynamics getDynamics() const;

  private:
    const api::MarkData &myMarkData;
    const MeasureCursor myCursor;
    const Converter myConverter;
    DiagnosticsContext myDiagnostics;
};
} // namespace impl
} // namespace mx
