// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteAttachmentData.h"
#include "mx/api/WavyLineData.h"

#include <optional>

namespace mx
{
namespace core
{
class WavyLine;
}

namespace impl
{
// Reads one <wavy-line> element (a child of a note's <ornaments>) into whichever
// start/continue/stop vector on outAttachments matches its type attribute.
void parseWavyLine(const core::WavyLine &inWavyLine, api::NoteAttachmentData &outAttachments);

// The number written is the one the SpannerNumberResolver pass resolved for the object.
core::WavyLine writeWavyLineStart(const api::WavyLineStart &inStart, const std::optional<int> &inResolvedNumber);
core::WavyLine writeWavyLineContinue(const api::WavyLineContinue &inContinue,
                                     const std::optional<int> &inResolvedNumber);
core::WavyLine writeWavyLineStop(const api::WavyLineStop &inStop, const std::optional<int> &inResolvedNumber);
} // namespace impl
} // namespace mx
