// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/EncodingData.h"
#include "mx/core/generated/Encoding.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/impl/DiagnosticsContext.h"

namespace mx
{
namespace impl
{
void createEncoding(const api::EncodingData &inEncoding, core::ScoreHeaderGroup &header,
                    const DiagnosticsContext &diagnostics);
api::EncodingData createEncoding(const core::Encoding &inEncoding);
} // namespace impl
} // namespace mx
