// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
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

using namespace std;
using namespace mx::api;
using namespace mxtest;


TEST( miscFields, NoteData )
{
    ScoreData score;
    score.parts.emplace_back();
    auto& part = score.parts.back();
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];
    voice.notes.emplace_back();
    auto& note = voice.notes.back();
    note.miscData.push_back( "Hello,There" );
    note.miscData.push_back( "Bones" );
    note.miscData.push_back( "Bishop" );

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
    auto& opart = oscore.parts.back();
    auto& omeasure = opart.measures.back();
    auto& ostaff = omeasure.staves.back();
    auto& ovoice = ostaff.voices[0];
    auto& onote = ovoice.notes.back();
    auto& omisc = onote.miscData;
    auto iter = omisc.cbegin();
    const auto end = omisc.cend();

    std::cout << xml << std::endl;

    // assert
    CHECK( iter != end );
    CHECK_EQUAL( "Hello_There", *iter);
    ++iter;

    CHECK( iter != end );
    CHECK_EQUAL( "Bones", *iter);
    ++iter;

    CHECK( iter != end );
    CHECK_EQUAL( "Bishop", *iter);
    ++iter;

    CHECK( iter == end );
}
T_END

TEST( SlurTieNumberLevel, NoteData )
{
    using namespace mx::core;
    Document doc;
    auto sp = doc.getScorePartwise();
    const auto& parts = sp->getPartwisePartSet();
    const auto& part = parts.front();
    const auto& measure = part->getPartwiseMeasureSet().front();
    const auto mdg = measure->getMusicDataGroup();
    const auto mdc = makeMusicDataChoice();
    mdg->addMusicDataChoice( mdc );
    mdc->setChoice( MusicDataChoice::Choice::note );
    const auto note = mdc->getNote();
    note->setHasType(true);
    note->getType()->setValue( NoteTypeValue::quarter );
    note->getNoteChoice()->setChoice( NoteChoice::Choice::normal );
    auto nng = note->getNoteChoice()->getNormalNoteGroup();
    nng->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
    auto pitch = nng->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch();
    const auto notations = makeNotations();
    note->addNotations( notations );
    const auto nc = makeNotationsChoice();
    notations->addNotationsChoice( nc );
    nc->setChoice( NotationsChoice::Choice::tied );
    const auto tied = nc->getTied();
    tied->getAttributes()->type = StartStopContinue::start;

    auto& mgr = DocumentManager::getInstance();
    std::stringstream ss;
    doc.toStream( ss );
    std::stringstream iss{ ss.str() };
    auto id = mgr.createFromStream( iss );
    const auto scoreData = mgr.getData( id );
    mgr.destroyDocument( id );

    const auto& noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto& curveStart = noteData.noteAttachmentData.curveStarts.front();
    CHECK_EQUAL( -1, curveStart.numberLevel );
    CHECK( curveStart.curveType == mx::api::CurveType::tie );
}
T_END

#endif
