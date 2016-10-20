// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/Path.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/LayoutData.h"
#include "mx/core/Document.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/BottomMargin.h"


using namespace std;
using namespace mx::api;

inline ScoreData createExpectedScoreData()
{
    ScoreData scoreData;
    scoreData.workTitle = "RoundTrip_PositionInfo";
    scoreData.parts.emplace_back( PartData{} );
    auto& part = scoreData.parts.at( 0 );
    part.measures.emplace_back( MeasureData{} );
    auto& measure = part.measures.at( 0 );
    measure.staves.emplace_back( StaffData{} );
    auto& staff = measure.staves.at( 0 );
    staff.voices[0] = VoiceData{};
    auto& voice = staff.voices.at( 0 );
    voice.notes.emplace_back( NoteData{} );
    auto& note = voice.notes.at( 0 );
    note.absoluteNoteIndex = 1;
    note.positionData.hasDefaultX = true;
    note.positionData.hasDefaultY = true;
    note.positionData.hasRelativeX = true;
    note.positionData.hasRelativeY = true;
    note.positionData.defaultX = 1.0;
    note.positionData.defaultY = 2.0;
    note.positionData.relativeX = 3.0;
    note.positionData.relativeY = 4.0;
    note.noteAttachmentData.marks.emplace_back( MarkData{} );
    auto& mark = note.noteAttachmentData.marks.at( 0 );
    mark.printData.isColorSpecified = true;
    mark.printData.color.red = 1;
    mark.printData.color.green = 2;
    mark.printData.color.blue = 3;
    mark.printData.fontData.fontFamily.emplace_back( "HELLO" );
    return scoreData;
}

inline ScoreData roundTripTheScore( ScoreData inScoreData )
{
    ScoreData scoreData = createExpectedScoreData();
    auto& docMgr = DocumentManager::getInstance();
    int docId = docMgr.createFromScore( scoreData );
    std::ostringstream oss;
    docMgr.writeToStream( docId, oss );
    docMgr.destroyDocument( docId );
    std::istringstream iss{ oss.str() };
    docId = docMgr.createFromStream( iss );
    auto afterScore = docMgr.getData( docId );
    docMgr.destroyDocument( docId );
    return afterScore;
}

TEST( RoundTrip_PositionInfo, PositionParsing )
{
    ScoreData beforeScore = createExpectedScoreData();
    auto afterScore = roundTripTheScore( beforeScore );
    CHECK( false );
}
T_END

#endif
