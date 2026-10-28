// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SoundData.h"
#include "mx/impl/DiagnosticsContext.h"

#include <string>

namespace mx
{
namespace core
{
class Sound;
}

namespace impl
{
// core -> api: read a <sound> element into api::SoundData. instrumentId is the id of the part's
// instrument; an <instrument-change> for any other instrument is reported and not read.
api::SoundData readSoundData(const core::Sound &inSound, const std::string &instrumentId,
                             const DiagnosticsContext &diagnostics, const api::Location &location);

// api -> core: write api::SoundData onto a core::Sound element. An instrument change is written
// for instrumentId, the id of the part's instrument.
void writeSoundData(const api::SoundData &inSoundData, core::Sound &outSound, const std::string &instrumentId,
                    const DiagnosticsContext &diagnostics, const api::Location &location);
} // namespace impl
} // namespace mx
