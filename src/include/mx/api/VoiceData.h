// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteData.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{
class VoiceData
{
  public:
    // What MusicXML calls this voice, the text of the <voice> element on each of its notes.
    // Voices are normally numbered and the api keys them by index, so leave this empty and each
    // voice is written with its own number. Set it when the name is not that number: a part
    // whose voices are numbered straight through its staves instead of restarting on each one,
    // or a program that writes words such as "FirstVoice" rather than numbers.
    std::string label;

    std::vector<NoteData> notes;
};

inline bool operator==(const VoiceData &lhs, const VoiceData &rhs)
{
    return lhs.label == rhs.label && areVectorsEqual(lhs.notes, rhs.notes);
}

MXAPI_NOT_EQUALS_AND_VECTORS(VoiceData);
} // namespace api
} // namespace mx
