// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_API_TESTS

    #include "cpul/testFramework.h"
    #include "mxtest/api/RoundTrip.h"
    #include "mx/api/DocumentManager.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

static constexpr const char* const fname = "Bombe.xml";

inline ScoreData getBombe()
{
    const std::string fileName{ fname };
    const std::string path{ MxFileRepository::getFullPath( fileName ) };
    auto& docMgr = mx::api::DocumentManager::getInstance();
    auto docId = docMgr.createFromFile( path );
    auto scoreData = docMgr.getData( docId );
    docMgr.destroyDocument( docId );
    return scoreData;
}

inline NoteData getBombeNote( int measureIndex, int partIndex, int staffIndex, int voiceIndex, int noteIndex )
{
    auto scoreData = getBombe();
    const auto& part = scoreData.parts.at( partIndex );
    const auto& measure = part.measures.at( measureIndex );
    const auto& staff = measure.staves.at( staffIndex );
    const auto& voice = staff.voices.at( voiceIndex );
    const auto& note = voice.notes.at( noteIndex );
    return note;
}

TEST( x, Bombe )
{

    const int measureIndex = 0;
    const int partIndex = 1;
    const int staffIndex = 0;
    const int voiceIndex = 0;
    const int noteIndex = 1;

    const auto noteData = getBombeNote( measureIndex, partIndex, staffIndex, voiceIndex, noteIndex );
    CHECK_EQUAL( 336, noteData.tickTimePosition );

}

T_END

#endif
