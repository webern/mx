// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SoundData.h"

namespace mx
{
namespace core
{
class Sound;
}

namespace impl
{
// core -> api: read the commonly-used scalar attributes of a <sound> element into api::SoundData.
api::SoundData readSoundData(const core::Sound &inSound);

// api -> core: write the api::SoundData scalar attributes onto a core::Sound element.
void writeSoundData(const api::SoundData &inSoundData, core::Sound &outSound);
} // namespace impl
} // namespace mx
