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


TEST( tremolos, NoteData )
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
    auto& attachments = note.noteAttachmentData;
    auto& marks = attachments.marks;

    MarkData mark{ MarkType::tremoloSingleOne };
    marks.emplace_back( mark );
    mark = MarkData{ MarkType::tremoloSingleTwo };
    marks.emplace_back( mark );
    mark = MarkData{ MarkType::tremoloSingleThree };
    marks.emplace_back( mark );
    mark = MarkData{ MarkType::tremoloSingleFour };
    marks.emplace_back( mark );
    mark = MarkData{ MarkType::tremoloSingleFive };
    marks.emplace_back( mark );


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
    const auto& ostaff = omeasure.staves.back();
    const auto& ovoice = ostaff.voices.at(0);
    const auto& onote = ovoice.notes.back();
    const auto& omarks = onote.noteAttachmentData.marks;
    auto markIter = omarks.cbegin();
    const auto markEnd = marks.cend();

    for (int i = 1; i <= 5; ++i, ++markIter)
    {
        CHECK( markIter != markEnd );
        const auto& markData = *markIter;
        CHECK_EQUAL( i, numTremoloSlashes( markData.markType ) );
    }
}
T_END

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

    //std::cout << xml << std::endl;

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

TEST( SlurTieNumberLevelA, NoteData )
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

TEST( SlurTieNumberLevelB, NoteData )
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
    tied->getAttributes()->type = StartStopContinue::continue_;

    auto& mgr = DocumentManager::getInstance();
    std::stringstream ss;
    doc.toStream( ss );
    std::stringstream iss{ ss.str() };
    auto id = mgr.createFromStream( iss );
    const auto scoreData = mgr.getData( id );
    mgr.destroyDocument( id );

    const auto& noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto& curveContinue = noteData.noteAttachmentData.curveContinuations.front();
    CHECK_EQUAL( -1, curveContinue.numberLevel );
    CHECK( curveContinue.curveType == mx::api::CurveType::tie );
}
T_END

TEST( SlurTieNumberLevelC, NoteData )
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
    tied->getAttributes()->type = StartStopContinue::stop;

    auto& mgr = DocumentManager::getInstance();
    std::stringstream ss;
    doc.toStream( ss );
    std::stringstream iss{ ss.str() };
    auto id = mgr.createFromStream( iss );
    const auto scoreData = mgr.getData( id );
    mgr.destroyDocument( id );

    const auto& noteData = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes.front();
    const auto& curveStop = noteData.noteAttachmentData.curveStops.front();
    CHECK_EQUAL( -1, curveStop.numberLevel );
    CHECK( curveStop.curveType == mx::api::CurveType::tie );
}
T_END

TEST( ornaments, NoteData )
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

    note.noteAttachmentData.marks.emplace_back( Placement::above, MarkType::trillMark );
    note.noteAttachmentData.marks.back().positionData.isDefaultXSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultX = 123.0;

    note.noteAttachmentData.marks.emplace_back( Placement::above, MarkType::wavyLine );
    note.noteAttachmentData.marks.back().positionData.isDefaultYSpecified = true;
    note.noteAttachmentData.marks.back().positionData.defaultY = -456.0;

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
    auto& oattachments = onote.noteAttachmentData;
    auto& omarks = oattachments.marks;
    auto oIter = omarks.cbegin();

    auto md = *oIter;
    CHECK( md.markType == MarkType::trillMark );
    CHECK( md.positionData.isDefaultXSpecified );
    CHECK( !md.positionData.isDefaultYSpecified );
    CHECK_DOUBLES_EQUAL( 123.0, md.positionData.defaultX, 0.00001 );

    ++oIter;
    md = *oIter;
    CHECK( md.markType == MarkType::wavyLine );
    CHECK( !md.positionData.isDefaultXSpecified );
    CHECK( md.positionData.isDefaultYSpecified );
    CHECK_DOUBLES_EQUAL( -456.0, md.positionData.defaultY, 0.00001 );
}
T_END

TEST( pedalStart, NoteData )
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

    staff.directions.emplace_back();
    auto& direction = staff.directions.back();
    direction.marks.emplace_back( Placement::below, MarkType::pedal );
    direction.tickTimePosition = 7;

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
    auto& odirections = ostaff.directions;
    auto& odirection = odirections.back();
    auto& omark = odirection.marks.back();

    CHECK( omark.markType == MarkType::pedal );
    CHECK_EQUAL( odirection.tickTimePosition, odirection.tickTimePosition );
}
T_END

TEST( pedalStop, NoteData )
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

    staff.directions.emplace_back();
    auto& direction = staff.directions.back();
    direction.marks.emplace_back( Placement::below, MarkType::damp );
    direction.tickTimePosition = 70342;

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
    auto& odirections = ostaff.directions;
    auto& odirection = odirections.back();
    auto& omark = odirection.marks.back();

    CHECK( omark.markType == MarkType::damp );
    CHECK_EQUAL( odirection.tickTimePosition, odirection.tickTimePosition );
}
T_END

TEST( directionOrder, NoteData )
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 120;
    auto& part = score.parts.back();
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];

    NoteData note;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 120;
    note.tickTimePosition = 0;
    voice.notes.push_back( note );

    note.tickTimePosition = 120;
    note.pitchData.step = Step::d;
    voice.notes.push_back( note );

    note.tickTimePosition = 240;
    note.pitchData.step = Step::e;
    voice.notes.push_back( note );

    note.tickTimePosition = 360;
    note.pitchData.step = Step::e;
    voice.notes.push_back( note );

    DirectionData direction;
    direction.tickTimePosition = 0;
    MarkData mark{ Placement::below, MarkType::damp };
    mark.tickTimePosition = direction.tickTimePosition;
    direction.marks.push_back( mark );
    staff.directions.push_back( direction );

    direction.tickTimePosition = 120;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    direction.tickTimePosition = 240;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    direction.tickTimePosition = 360;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    direction.tickTimePosition = 480;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    auto docPtr = mgr.getDocument( docId );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);

    const auto& partwise = docPtr->getScorePartwise();
    const auto& partwisePartSet = partwise->getPartwisePartSet();
    const auto& partwisePart = partwisePartSet.front();
    const auto& partwiseMeasure = partwisePart->getPartwiseMeasureSet().front();
    const auto& mdcSet = partwiseMeasure->getMusicDataGroup()->getMusicDataChoiceSet();

    auto iter = mdcSet.cbegin();
    auto mdc = *iter;
    auto elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::properties == elementType );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::direction == elementType );
    auto dirElement = mdc->getDirection();
    CHECK( !dirElement->getHasOffset() );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::note == elementType );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::direction == elementType );
    dirElement = mdc->getDirection();
    CHECK( !dirElement->getHasOffset() );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::note == elementType );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::direction == elementType );
    dirElement = mdc->getDirection();
    CHECK( !dirElement->getHasOffset() );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::note == elementType );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::direction == elementType );
    dirElement = mdc->getDirection();
    CHECK( !dirElement->getHasOffset() );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::note == elementType );

    ++iter;
    mdc = *iter;
    elementType = mdc->getChoice();
    CHECK( mx::core::MusicDataChoice::Choice::direction == elementType );
    dirElement = mdc->getDirection();
    CHECK( !dirElement->getHasOffset() );
}
T_END

TEST( directionOrderRoundTrip, NoteData )
{
    ScoreData score;
    score.parts.emplace_back();
    score.ticksPerQuarter = 120;
    auto& part = score.parts.back();
    part.uniqueId = "BONES";
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];

    NoteData note;
    note.userRequestedVoiceNumber = 1;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = 120;
    note.tickTimePosition = 0;
    voice.notes.push_back( note );

    note.tickTimePosition = 120;
    note.pitchData.step = Step::d;
    voice.notes.push_back( note );

    note.tickTimePosition = 240;
    note.pitchData.step = Step::e;
    voice.notes.push_back( note );

    note.tickTimePosition = 360;
    note.pitchData.step = Step::e;
    voice.notes.push_back( note );

    auto placement = Placement::above;
    DirectionData direction;
    direction.placement = placement;
    MarkData mark{ direction.placement, MarkType::damp };
    direction.tickTimePosition = 0;
    mark.tickTimePosition = direction.tickTimePosition;
    direction.marks.push_back( mark );
    staff.directions.push_back( direction );

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = false;
    direction.marks.back().positionData.defaultX = 0.0;
    direction.marks.back().positionData.isDefaultYSpecified = true;
    direction.marks.back().positionData.defaultY = 1.1;
    direction.tickTimePosition = 120;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = true;
    direction.marks.back().positionData.defaultX = 2.2;
    direction.marks.back().positionData.isDefaultYSpecified = false;
    direction.marks.back().positionData.defaultY = 0.0;
    direction.tickTimePosition = 240;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = true;
    direction.marks.back().positionData.defaultX = 3.3;
    direction.marks.back().positionData.isDefaultYSpecified = true;
    direction.marks.back().positionData.defaultY = 3.3;
    direction.tickTimePosition = 360;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    placement = Placement::unspecified;
    direction.placement = placement;
    direction.marks.back().positionData.isDefaultXSpecified = false;
    direction.marks.back().positionData.defaultX = 0.0;
    direction.marks.back().positionData.isDefaultYSpecified = false;
    direction.marks.back().positionData.defaultY = 0.0;
    direction.marks.back().positionData.placement = placement;
    direction.tickTimePosition = 480;
    direction.marks.back().tickTimePosition = direction.tickTimePosition;
    staff.directions.push_back( direction );

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    auto docPtr = mgr.getDocument( docId );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{ xml };
    docId = mgr.createFromStream( iss );
    auto oscore = mgr.getData(docId);
    CHECK( score == oscore )

}
T_END

#endif
