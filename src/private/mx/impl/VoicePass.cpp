// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/VoicePass.h"

#include <algorithm>
#include <iterator>
#include <set>
#include <string>

namespace mx
{
namespace impl
{
// A note, or the notes of a chord, that a voice plays at one tick. It lasts from its tick until the
// write position moves on after it, which for a chord is the end of its last note.
struct VoicePassUnit
{
    VoicePassVoiceIter voice;
    size_t firstNote = 0;
    size_t noteCount = 0;
    int start = 0;
    int end = 0;
};

const std::vector<api::NoteData> &VoicePass::notes() const
{
    return voices.size() == 1 ? voices.front()->second.notes : mergedNotes;
}

static std::vector<VoicePassUnit> voicePassUnits(VoicePassVoiceIter voice)
{
    std::vector<VoicePassUnit> units;
    const auto &notes = voice->second.notes;

    for (size_t first = 0; first < notes.size();)
    {
        // chord notes at one tick end together, as in MeasureWriter::isAdvanceNeeded
        size_t last = first;
        while (notes.at(last).isChord && last + 1 < notes.size() &&
               notes.at(last + 1).tickTimePosition == notes.at(last).tickTimePosition)
        {
            ++last;
        }

        // as in MeasureWriter::advanceCursorIfNeeded
        const auto &lastNote = notes.at(last);
        const int duration = lastNote.isGrace ? 0 : std::max(lastNote.durationData.durationTimeTicks, 0);
        const int start = notes.at(first).tickTimePosition;
        units.push_back(VoicePassUnit{voice, first, last - first + 1, start, start + duration});
        first = last + 1;
    }

    return units;
}

// The text NoteWriter puts in each note's <voice> element.
static std::string voicePassVoiceText(VoicePassVoiceIter voice)
{
    return voice->second.label.empty() ? std::to_string(voice->first + 1) : voice->second.label;
}

// Whether a staff's voices may share a pass at all. A reader splits the notes between two <backup>s
// into voices by their <voice> text, and a pass can share that stretch of the stream with the passes
// before and after it when no <backup> is needed between them, so every voice of the staff must
// have a text of its own. And every note needs a tick, since a pass puts its notes in tick order.
static bool canStaffShareVoicePasses(const api::StaffData &inStaff)
{
    std::set<std::string> texts;
    for (auto voice = inStaff.voices.cbegin(); voice != inStaff.voices.cend(); ++voice)
    {
        const auto &notes = voice->second.notes;
        const bool isTimed =
            std::none_of(notes.cbegin(), notes.cend(), [](const auto &note) { return note.tickTimePosition < 0; });
        if (!isTimed || !texts.insert(voicePassVoiceText(voice)).second)
        {
            return false;
        }
    }
    return true;
}

// Whether a voice can be written inside the pass before it, filling the pass's gaps instead of
// following it after a <backup>. On success, outMerged holds the units of both in written order.
//
// The voice must start before the pass ends; a voice that starts later is written after it as
// usual. Together, the units must form a single line in time: each starts no earlier than the one
// before it ends. Two voices may not start a unit at the same tick, since the order of the two
// would then be a guess; that keeps a grace note of one voice apart from a note of another. Each
// voice must make its first entrance in key order, so that a reader numbering voices as they
// appear gets them back in the same order.
static bool canJoinVoicePass(const std::vector<VoicePassUnit> &passUnits,
                             const std::vector<VoicePassVoiceIter> &passVoices,
                             const std::vector<VoicePassUnit> &voiceUnits, VoicePassVoiceIter voice,
                             std::vector<VoicePassUnit> &outMerged)
{
    if (passUnits.empty() || voiceUnits.empty())
    {
        return false;
    }

    const auto passEnd = std::max_element(passUnits.cbegin(), passUnits.cend(), [](const auto &a, const auto &b) {
                             return a.end < b.end;
                         })->end;

    if (voiceUnits.front().start >= passEnd)
    {
        return false;
    }

    std::vector<VoicePassUnit> merged;
    merged.reserve(passUnits.size() + voiceUnits.size());
    std::merge(passUnits.cbegin(), passUnits.cend(), voiceUnits.cbegin(), voiceUnits.cend(), std::back_inserter(merged),
               [](const auto &a, const auto &b) { return a.start < b.start; });

    std::vector<VoicePassVoiceIter> entrances;

    for (size_t i = 0; i < merged.size(); ++i)
    {
        const auto &unit = merged.at(i);

        if (i > 0)
        {
            const auto &previous = merged.at(i - 1);
            if (unit.start < previous.end || (unit.start == previous.start && unit.voice != previous.voice))
            {
                return false;
            }
        }

        if (std::find(entrances.cbegin(), entrances.cend(), unit.voice) == entrances.cend())
        {
            entrances.push_back(unit.voice);
        }
    }

    auto voices = passVoices;
    voices.push_back(voice);

    if (entrances != voices)
    {
        return false;
    }

    outMerged = std::move(merged);
    return true;
}

static void finishVoicePass(VoicePass &pass, const std::vector<VoicePassUnit> &units)
{
    for (const auto &unit : units)
    {
        const auto &notes = unit.voice->second.notes;
        for (size_t i = unit.firstNote; i < unit.firstNote + unit.noteCount; ++i)
        {
            if (pass.voices.size() > 1)
            {
                pass.mergedNotes.push_back(notes.at(i));
            }
            pass.written.push_back(VoicePassNote{unit.voice, &notes.at(i)});
        }
    }
}

std::vector<VoicePass> makeVoicePasses(const api::StaffData &inStaff)
{
    std::vector<VoicePass> passes;
    std::vector<VoicePassUnit> passUnits;
    const bool canShare = canStaffShareVoicePasses(inStaff);

    for (auto voice = inStaff.voices.cbegin(); voice != inStaff.voices.cend(); ++voice)
    {
        auto voiceUnits = voicePassUnits(voice);
        std::vector<VoicePassUnit> merged;

        if (canShare && !passes.empty() && canJoinVoicePass(passUnits, passes.back().voices, voiceUnits, voice, merged))
        {
            passes.back().voices.push_back(voice);
            passUnits = std::move(merged);
            continue;
        }

        if (!passes.empty())
        {
            finishVoicePass(passes.back(), passUnits);
        }

        passes.emplace_back();
        passes.back().voices.push_back(voice);
        passUnits = std::move(voiceUnits);
    }

    if (!passes.empty())
    {
        finishVoicePass(passes.back(), passUnits);
    }

    return passes;
}
} // namespace impl
} // namespace mx
