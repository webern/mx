// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/FullNoteGroup.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NormalNoteGroup.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/NoteChoice.h"
#include "mxtest/file/MxFileRepository.h"
#include <sstream>
#include <vector>

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
//    constexpr const char* const fileName = "testAccidentals1.xml";
}

TEST(chordTest, Chords)
{
    ScoreData score;
    score.ticksPerQuarter = 240;
    score.parts.emplace_back(PartData{});
    auto &part = score.parts.back();
    part.measures.emplace_back(MeasureData{});
    auto &measure = part.measures.back();
    measure.staves.emplace_back(StaffData{});
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0] = VoiceData{};
    voice.notes.emplace_back(NoteData{});
    auto noteP = &voice.notes.back();
    noteP->durationData.durationName = DurationName::eighth;
    noteP->pitchData.step = Step::c;
    noteP->isChord = true;
    noteP->tickTimePosition = 0;
    noteP->durationData.durationTimeTicks = 120;
    noteP->beams.emplace_back(Beam::begin);

    voice.notes.emplace_back(NoteData{});
    noteP = &voice.notes.back();
    noteP->durationData.durationName = DurationName::eighth;
    noteP->pitchData.step = Step::d;
    noteP->isChord = true;
    noteP->tickTimePosition = 0;
    noteP->durationData.durationTimeTicks = 120;
    //    noteP->beams.emplace_back(Beam::begin);

    voice.notes.emplace_back(NoteData{});
    noteP = &voice.notes.back();
    noteP->durationData.durationName = DurationName::eighth;
    noteP->pitchData.step = Step::e;
    noteP->isChord = true;
    noteP->tickTimePosition = 120;
    noteP->beams.emplace_back(Beam::end);
    noteP->durationData.durationTimeTicks = 120;

    voice.notes.emplace_back(NoteData{});
    noteP = &voice.notes.back();
    noteP->durationData.durationName = DurationName::eighth;
    noteP->pitchData.step = Step::f;
    noteP->isChord = true;
    noteP->tickTimePosition = 120;
    noteP->durationData.durationTimeTicks = 120;
    //    noteP->beams.emplace_back(Beam::end);

    auto docIdResult = fromScore(score);
    REQUIRE(docIdResult.ok());
    const auto doc = std::move(docIdResult).value();
    std::stringstream ss;
    doc.writeToStream(ss);

    REQUIRE(doc.getCoreDocument().isScorePartwise());
    const auto &scorePartwise = doc.getCoreDocument().asScorePartwise();
    const auto parts = scorePartwise.part();
    REQUIRE(!parts.empty());
    const auto &firstPart = parts[0];
    const auto measures = firstPart.measure();
    REQUIRE(!measures.empty());
    const auto &firstMeasure = measures[0];
    const auto &mdcSet = firstMeasure.musicData();

    // Collect only note choices
    std::vector<const mx::core::Note *> notes;
    for (const auto &mdc : mdcSet)
    {
        if (mdc.isNote())
        {
            notes.push_back(&mdc.asNote());
        }
    }

    // first note
    REQUIRE(notes.size() >= 1);
    auto notesIter = notes.cbegin();
    const mx::core::Note *note = *notesIter;
    REQUIRE(note->choice().isNormalNoteGroup());
    CHECK(!note->choice().asNormalNoteGroup().fullNote().chord());

    // second note
    ++notesIter;
    REQUIRE(notesIter != notes.cend());
    note = *notesIter;
    REQUIRE(note->choice().isNormalNoteGroup());
    CHECK(note->choice().asNormalNoteGroup().fullNote().chord());

    // third note
    ++notesIter;
    REQUIRE(notesIter != notes.cend());
    note = *notesIter;
    REQUIRE(note->choice().isNormalNoteGroup());
    CHECK(!note->choice().asNormalNoteGroup().fullNote().chord());

    // fourth note
    ++notesIter;
    REQUIRE(notesIter != notes.cend());
    note = *notesIter;
    REQUIRE(note->choice().isNormalNoteGroup());
    CHECK(note->choice().asNormalNoteGroup().fullNote().chord());

    // Also verify no forward elements in the measure
    for (const auto &mdc : mdcSet)
    {
        CHECK(!mdc.isForward());
    }
}

T_END

#endif
