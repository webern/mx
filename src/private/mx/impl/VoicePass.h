// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/StaffData.h"

#include <map>
#include <vector>

namespace mx
{
namespace impl
{
using VoicePassVoiceIter = std::map<int, api::VoiceData>::const_iterator;

// One note of a pass and the voice it belongs to. note points into the ScoreData being written, so
// anything that looks a note's parts up by address (SpannerResolver) finds them.
struct VoicePassNote
{
    VoicePassVoiceIter voice;
    const api::NoteData *note = nullptr;
};

// Voices of one staff that are written as one stream, in time order, with no <backup> inside it.
// Most passes hold one voice; makeVoicePasses says when a pass holds more.
struct VoicePass
{
    // the pass's voices, in key order
    std::vector<VoicePassVoiceIter> voices;

    // the notes in the order they are written
    std::vector<VoicePassNote> written;

    // Copies of the notes in the order they are written, when the pass has more than one voice. They
    // are for stepping through the stream by tick; a note that is written or looked up by address
    // comes from written.
    std::vector<api::NoteData> mergedNotes;

    // The notes in the order they are written: the voice's own notes when the pass has one voice.
    const std::vector<api::NoteData> &notes() const;
};

// Groups a staff's voices, in key order, into the passes MeasureWriter writes them in. Each voice
// joins the pass before it when it fits into that pass's gaps; otherwise it starts a pass of its
// own. MusicXML reads a change of <voice> text between two notes as a change of voice, so a voice
// that only plays while another rests can be written in that rest without a <forward> and a
// <backup>.
std::vector<VoicePass> makeVoicePasses(const api::StaffData &inStaff);
} // namespace impl
} // namespace mx
