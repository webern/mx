// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/api/TestHelpers.h"
#include "mx/core/Document.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/FullNoteGroup.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
    constexpr const char* const fileName = "testChord.xml";
}


TEST( chordLoadNotes, ChordApi )
{
    const auto scoreData = mxtest::MxFileRepository::loadFile( fileName );
    CHECK_EQUAL( 1, scoreData.parts.size() );
    const auto& part = scoreData.parts.front();
    CHECK_EQUAL( 32, part.measures.size() )
    const auto& measure = part.measures.front();
    CHECK_EQUAL( 1, measure.staves.size() );
    const auto& staff = measure.staves.front();
    CHECK_EQUAL( 1, staff.voices.size() );
    const auto& voiceIter = *staff.voices.cbegin();
    CHECK_EQUAL( 0, voiceIter.first )
    const auto& voice = voiceIter.second;
    CHECK_EQUAL( 5, voice.notes.size() );
    auto noteIter = voice.notes.cbegin();
    auto note = *noteIter;
    
    // first note of chord
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::a == note.pitchData.step );
    CHECK_EQUAL( 4, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // second note of chord
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::c == note.pitchData.step );
    CHECK_EQUAL( 5, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // third note of chord
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::e == note.pitchData.step );
    CHECK_EQUAL( 5, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // quarter rest on beat 2
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 1, note.tickTimePosition );
    CHECK( note.isRest );
    CHECK( !note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // half rest on beat 3
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 2, note.tickTimePosition );
    CHECK( note.isRest );
    CHECK( !note.isChord );
    CHECK_EQUAL( 2, note.durationData.durationTimeTicks );
    CHECK( DurationName::half == note.durationData.durationName );
}
T_END

TEST( chordSaveNotes, ChordApi )
{
    const auto originalData = mxtest::MxFileRepository::loadFile( fileName );
    auto& docMgr = DocumentManager::getInstance();
    const int savedDocId = docMgr.createFromScore( originalData );
    const auto scoreData = docMgr.getData( savedDocId );
    docMgr.destroyDocument( savedDocId );
    
    CHECK_EQUAL( 1, scoreData.parts.size() );
    const auto& part = scoreData.parts.front();
    CHECK_EQUAL( 32, part.measures.size() )
    const auto& measure = part.measures.front();
    CHECK_EQUAL( 1, measure.staves.size() );
    const auto& staff = measure.staves.front();
    CHECK_EQUAL( 1, staff.voices.size() );
    const auto& voiceIter = *staff.voices.cbegin();
    CHECK_EQUAL( 0, voiceIter.first )
    const auto& voice = voiceIter.second;
    CHECK_EQUAL( 5, voice.notes.size() );
    auto noteIter = voice.notes.cbegin();
    auto note = *noteIter;
    
    // first note of chord
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::a == note.pitchData.step );
    CHECK_EQUAL( 4, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // second note of chord
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::c == note.pitchData.step );
    CHECK_EQUAL( 5, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // third note of chord
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 0, note.tickTimePosition );
    CHECK( !note.isRest );
    CHECK( note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( Step::e == note.pitchData.step );
    CHECK_EQUAL( 5, note.pitchData.octave );
    CHECK_EQUAL( 0, note.pitchData.alter );
    CHECK( Stem::down == note.stem );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // quarter rest on beat 2
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 1, note.tickTimePosition );
    CHECK( note.isRest );
    CHECK( !note.isChord );
    CHECK_EQUAL( 1, note.durationData.durationTimeTicks );
    CHECK( DurationName::quarter == note.durationData.durationName );
    
    // half rest on beat 3
    ++noteIter;
    note = *noteIter;
    CHECK_EQUAL( 2, note.tickTimePosition );
    CHECK( note.isRest );
    CHECK( !note.isChord );
    CHECK_EQUAL( 2, note.durationData.durationTimeTicks );
    CHECK( DurationName::half == note.durationData.durationName );
}
T_END

TEST( KompChordBug_PIVOTAL_147058063, ChordApi )
{
    ScoreData originalScore{};
    originalScore.parts.emplace_back();
    auto& originalPart = originalScore.parts.back();
    originalPart.measures.emplace_back();
    auto& originalMeasure = originalPart.measures.back();

    originalMeasure.staves.emplace_back();
    StaffData* originalStaffPtr = &originalMeasure.staves.back();
    ClefData trebleClef{};
    trebleClef.setTreble();
    trebleClef.tickTimePosition = 0;
    originalStaffPtr->clefs.push_back(trebleClef);

    originalMeasure.staves.emplace_back();
    originalStaffPtr = &originalMeasure.staves.back();
    ClefData bassClef{};
    bassClef.setBass();
    bassClef.tickTimePosition = 0;
    originalStaffPtr->clefs.push_back(bassClef);

    originalMeasure.timeSignature = TimeSignatureData{};
    originalMeasure.timeSignature.isImplicit = false;

    originalStaffPtr = &(*originalMeasure.staves.begin());
    NoteData note{};
    note.durationData.durationTimeTicks = originalScore.ticksPerQuarter * 4;
    note.durationData.durationName = DurationName::whole;
    note.pitchData.octave = 4;
    note.pitchData.step = Step::c;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    note.pitchData.step = Step::e;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    note.pitchData.step = Step::g;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    originalStaffPtr = &( *( originalMeasure.staves.begin() + 1) );
    note.pitchData.octave = 2;
    note.pitchData.step = Step::c;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    note.pitchData.octave = 2;
    note.pitchData.step = Step::e;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    note.pitchData.octave = 2;
    note.pitchData.step = Step::g;
    note.isChord = true;
    originalStaffPtr->voices[0].notes.push_back(note);

    auto& docMgr = DocumentManager::getInstance();
    const auto docID = docMgr.createFromScore( originalScore );
    const auto documentPtr = docMgr.getDocument( docID );
    docMgr.destroyDocument( docID );
    const auto scorePartwisePtr = documentPtr->getScorePartwise();
    const auto xml = mxtest::toXml( originalScore );
    const auto savedScore = mxtest::fromXml( xml );
    const auto& savedPart = savedScore.parts.at(0);
    const auto& savedMeasure = savedPart.measures.at(0);
    const StaffData* savedStaffPtr = &( *( savedMeasure.staves.cbegin() ) );
    const NoteData* savedNotePtr = nullptr;

    // check that all notes are chorded in treble clef
    savedNotePtr = &savedStaffPtr->voices.at(0).notes.at(0);
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );
    savedNotePtr = &savedStaffPtr->voices.at(0).notes.at(1);
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );
    savedNotePtr = &savedStaffPtr->voices.at(0).notes.at(2);
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );

    // check that all notes are chorded in bass clef
    savedStaffPtr = &( *( savedMeasure.staves.cbegin() + 1 ) );
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );
    savedNotePtr = &savedStaffPtr->voices.at(0).notes.at(1);
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );
    savedNotePtr = &savedStaffPtr->voices.at(0).notes.at(2);
    CHECK( savedNotePtr->isChord );
    CHECK_EQUAL( 0, savedNotePtr->tickTimePosition );

    const auto& partwisePartSet = scorePartwisePtr->getPartwisePartSet();
    const auto& partwisePart = partwisePartSet.at( 0 );
    const auto& partwiseMeasureSet = partwisePart->getPartwiseMeasureSet();
    const auto& partwiseMeasure = partwiseMeasureSet.at( 0 );
    const auto& mdg = partwiseMeasure->getMusicDataGroup();
    const auto& musicDataChoiceSet = mdg->getMusicDataChoiceSet();

    int noteIndex = 0;
    for( const auto& musicDataChoice : musicDataChoiceSet )
    {
        if( musicDataChoice->getChoice() != mx::core::MusicDataChoice::Choice::note )
        {
            continue;
        }

        const auto mxNote = musicDataChoice->getNote();
        const auto mxNormal = mxNote->getNoteChoice()->getNormalNoteGroup();
        const auto mxFullNoteGroup = mxNormal->getFullNoteGroup();
        const bool hasChordTag = mxFullNoteGroup->getHasChord();

        if( noteIndex == 0 || noteIndex == 3 )
        {
            CHECK( !hasChordTag );
        }
        else
        {
            CHECK( hasChordTag );
        }
        ++noteIndex;
    }

    CHECK_EQUAL( 6, noteIndex );
}
T_END;

#endif
