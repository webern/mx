// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/testFramework.h"
#include "mxtest/api/RoundTrip.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/Document.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Pedal.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST( forwardRepeat, MeasureData )
{
    const auto expectedBarlineType = mx::api::BarlineType::heavyLight;
    const bool isRepeatExpected = true;

    ScoreData score;
    score.parts.emplace_back();
    auto& part = score.parts.back();
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];
    voice.notes.emplace_back();
    measure.barlines.emplace_back();
    auto& barlineData = measure.barlines.back();
    barlineData.barlineType = mx::api::BarlineType::heavyLight;
    barlineData.repeat = true;

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{ xml };
    docId = mgr.createFromStream( iss );
    auto oscore = mgr.getData(docId);

    // get the data after the round trip
    const auto& opart = oscore.parts.back();
    const auto& omeasure = opart.measures.back();
    const auto& obarlines = omeasure.barlines;

    CHECK_EQUAL( 1, obarlines.size() );

    const auto& barline = obarlines.front();

    CHECK( expectedBarlineType == barline.barlineType );
    CHECK( isRepeatExpected == barline.repeat );

}
T_END;

TEST( backwardRepeat, MeasureData )
{
    const auto expectedBarlineType = mx::api::BarlineType::lightHeavy;
    const bool isRepeatExpected = true;

    ScoreData score;
    score.parts.emplace_back();
    auto& part = score.parts.back();
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];
    voice.notes.emplace_back();
    measure.barlines.emplace_back();
    auto& barlineData = measure.barlines.back();
    barlineData.barlineType = expectedBarlineType;
    barlineData.tickTimePosition = 480;
    barlineData.repeat = true;

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{ xml };
    docId = mgr.createFromStream( iss );
    auto oscore = mgr.getData(docId);

    // get the data after the round trip
    const auto& opart = oscore.parts.back();
    const auto& omeasure = opart.measures.back();
    const auto& obarlines = omeasure.barlines;

    CHECK_EQUAL( 1, obarlines.size() );

    const auto& barline = obarlines.front();

    CHECK( expectedBarlineType == barline.barlineType );
    CHECK( isRepeatExpected == barline.repeat );
    
}
T_END;

#endif
