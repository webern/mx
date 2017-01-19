// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/MusicDataChoice.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
//    constexpr const char* const fileName = "testAccidentals1.xml";
}


TEST( chordTest, Chords )
{
    ScoreData score;
    score.ticksPerQuarter = 240;
    score.parts.emplace_back(PartData{});
    auto& part = score.parts.back();
    part.measures.emplace_back(MeasureData{});
    auto& measure = part.measures.back();
    measure.staves.emplace_back(StaffData{});
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0] = VoiceData{};
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
    
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore(score);
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    auto doc = mgr.getDocument(docId);
    mgr.destroyDocument(docId);
    auto xml = ss.str();
    std::cout << xml << std::endl;
    
    using namespace mx::core;
    
    auto scorePartwise = doc->getScorePartwise();
    auto partPtr = scorePartwise->getPartwisePartSet().front();
    const auto& measures = partPtr->getPartwiseMeasureSet();
    auto measureIter = measures.cbegin();
    auto measurePtr = *measureIter;

    auto mdg = measurePtr->getMusicDataGroup();
    auto mdcSet = mdg->getMusicDataChoiceSet();
    
    std::vector<NotePtr> notes;
    for (const auto& mdc : mdcSet)
    {
        if (mdc->getChoice() == MusicDataChoice::Choice::note)
        {
            notes.push_back(mdc->getNote());
        }
    }
    
    // first note
    auto notesIter = notes.cbegin();
    auto note = (*notesIter);
    CHECK( !note->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );
    
    // second note
    ++notesIter;
    note = (*notesIter);
    CHECK( note->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );
    
//    notes.clear();
//    ++measureIter;
//    measurePtr = *measureIter;
//    
//    mdg = measurePtr->getMusicDataGroup();
//    mdcSet = mdg->getMusicDataChoiceSet();
//    
//    for (const auto& mdc : mdcSet)
//    {
//        if (mdc->getChoice() == MusicDataChoice::Choice::note)
//        {
//            notes.push_back(mdc->getNote());
//        }
//    }
    
    // third note
    ++notesIter;
    note = (*notesIter);
    CHECK( !note->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );
    
    // fourth note
    ++notesIter;
    note = (*notesIter);
    CHECK( note->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );
    
    for (const auto& mdc : mdcSet)
    {
        if (mdc->getChoice() == MusicDataChoice::Choice::note)
        {
            notes.push_back(mdc->getNote());
        }
        CHECK( mdc->getChoice() != MusicDataChoice::Choice::forward )
    }
}
T_END

#endif
