// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/SoloOrEnsemble.h"
#include "mx/api/SoundID.h"

#include <string>

namespace mx
{
namespace api
{
// A new playback sound for the part's instrument, from this point in the music onward. This is
// how a part plays a doubling: a flute player who picks up the piccolo keeps the same part, and
// the part's instrument takes on the piccolo sound. The fields mean the same as the matching fields
// of the part's InstrumentData.
//
// A doubling usually changes more than the sound. Show the new instrument's name with
// MeasureData::partNameDisplay, and its transposition with MeasureData::transpositions.
//
// MusicXML calls this <instrument-change>, inside a <sound>. You do not need to say which
// instrument it changes: a part has one instrument, and the change applies to it.
struct InstrumentChangeData
{
    SoundID soundID = SoundID::unspecified;
    SoloOrEnsemble soloOrEnsemble = SoloOrEnsemble::unspecified;

    // The sample library and the sound within it, for playback software that uses them.
    std::string virtualLibrary;
    std::string virtualName;
};

MXAPI_EQUALS_BEGIN(InstrumentChangeData)
MXAPI_EQUALS_MEMBER(soundID)
MXAPI_EQUALS_MEMBER(soloOrEnsemble)
MXAPI_EQUALS_MEMBER(virtualLibrary)
MXAPI_EQUALS_MEMBER(virtualName)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(InstrumentChangeData);
} // namespace api
} // namespace mx
