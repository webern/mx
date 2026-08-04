// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"

using namespace mx::api;

// ScoreData::sort() orders by tick position alone, so everything sharing a tick
// compares equal. The members of a chord all sit at one tick, and MusicXML
// encodes a chord by omitting <chord> from its first note, so their given order
// decides how the chord is spelled on the wire. An unstable sort would let the
// standard library implementation pick that order -- which it did: libc++ left
// these notes alone while libstdc++ permuted them, so ly32d_Arpeggio round-tripped
// on macOS and failed on Linux.
//
// The run is deliberately longer than the threshold at which an introsort stops
// insertion-sorting and starts partitioning, since a short run can come out
// ordered by luck.
TEST(sortKeepsSameTickNotesInOrder, ScoreDataSort)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &voice = measure.staves.back().voices[0];

    constexpr int chordSize = 64;

    for (int i = 0; i < chordSize; ++i)
    {
        voice.notes.emplace_back();
        auto &note = voice.notes.back();
        note.isChord = true;
        note.tickTimePosition = 0;
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 4;
        // octave is the identity we check for; step alone would repeat
        note.pitchData.octave = i;
    }

    score.sort();

    REQUIRE(voice.notes.size() == static_cast<size_t>(chordSize));

    for (int i = 0; i < chordSize; ++i)
    {
        CHECK(voice.notes.at(static_cast<size_t>(i)).pitchData.octave == i);
    }
}

T_END;

// The same guarantee for a voice that really does need reordering. The notes are
// interleaved across four ticks so the input is not already in order, which is
// what it takes to make an unstable sort actually permute the equal runs -- a
// long run that is already in order can survive std::sort by luck.
TEST(sortOrdersTicksAndKeepsEachTickInOrder, ScoreDataSort)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &voice = measure.staves.back().voices[0];

    constexpr int noteCount = 64;
    constexpr int tickCount = 4;
    const auto tickOf = [](int index) { return (index * 7) % tickCount; };

    for (int i = 0; i < noteCount; ++i)
    {
        voice.notes.emplace_back();
        auto &note = voice.notes.back();
        note.tickTimePosition = tickOf(i);
        note.durationData.durationName = DurationName::quarter;
        note.durationData.durationTimeTicks = 4;
        // octave carries the note's original index, so the given order is readable
        // back out of the sorted result
        note.pitchData.octave = i;
    }

    score.sort();

    // ticks ascending; within a tick, the indices in the order they were given
    std::vector<int> expectedOctaves;
    for (int tick = 0; tick < tickCount; ++tick)
    {
        for (int i = 0; i < noteCount; ++i)
        {
            if (tickOf(i) == tick)
            {
                expectedOctaves.push_back(i);
            }
        }
    }

    REQUIRE(voice.notes.size() == expectedOctaves.size());

    for (size_t i = 0; i < expectedOctaves.size(); ++i)
    {
        CHECK(voice.notes.at(i).pitchData.octave == expectedOctaves.at(i));
    }
}

T_END;

#endif
