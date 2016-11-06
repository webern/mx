// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"

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

#endif
