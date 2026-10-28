// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/InstrumentChangeData.h"
#include "mx/impl/DiagnosticsContext.h"

namespace mx
{
namespace core
{
class VirtualInstrumentDataGroup;
}

namespace impl
{
// core -> api: the <instrument-sound>, <solo>/<ensemble> and <virtual-instrument> children that
// <score-instrument> and <instrument-change> share. An <instrument-sound> that names no known
// sound is reported and not read.
api::InstrumentChangeData readVirtualInstrumentData(const core::VirtualInstrumentDataGroup &inGroup,
                                                    const DiagnosticsContext &diagnostics,
                                                    const api::Location &location);

// api -> core: the inverse of readVirtualInstrumentData. A field left unspecified or empty does not
// write an element.
core::VirtualInstrumentDataGroup writeVirtualInstrumentData(const api::InstrumentChangeData &inData);
} // namespace impl
} // namespace mx
