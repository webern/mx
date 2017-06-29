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
#include "mx/core/elements/Bass.h"
#include "mx/core/elements/BassAlter.h"
#include "mx/core/elements/BassStep.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Degree.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Frame.h"
#include "mx/core/elements/Function.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/HarmonyChordGroup.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Inversion.h"
#include "mx/core/elements/Kind.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PerMinute.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Root.h"
#include "mx/core/elements/RootAlter.h"
#include "mx/core/elements/RootStep.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"

#include "mxtest/api/ApiChordSimpleScoreData.h"

using namespace std;
using namespace mx::api;
using namespace mx;
using namespace mxtest;

TEST( Save, ChordDataSaveTest )
{
    const auto scoreData = apiChordSimpleScoreData();
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( scoreData );
    const auto doc = mgr.getDocument( docId );
    mgr.destroyDocument( docId );

    const auto scorePartwise = doc->getScorePartwise();
    const auto& partwisePartSet = scorePartwise->getPartwisePartSet();
    const auto& partwisePart = partwisePartSet.front();
    const auto& measureSet = partwisePart->getPartwiseMeasureSet();

    auto measure = measureSet.at( 0 );
    auto mdc = measure->getMusicDataGroup()->getMusicDataChoiceSet().back();
    CHECK( core::MusicDataChoice::Choice::harmony == mdc->getChoice() );
    auto harmony = mdc->getHarmony();
    auto grp = harmony->getHarmonyChordGroupSet().front();
    auto rootStep = grp->getRoot()->getRootStep()->getValue();
    bool hasRootAlter = grp->getRoot()->getHasRootAlter();
    auto rootAlter = grp->getRoot()->getRootAlter()->getValue().getValue();
    auto kind = grp->getKind()->getValue();
    bool hasText = grp->getKind()->getAttributes()->hasText;
    auto text = grp->getKind()->getAttributes()->text.getValue();
    bool hasBass = grp->getHasBass();
    auto bassStep = grp->getBass()->getBassStep()->getValue();
    bool hasBassAlter = grp->getBass()->getHasBassAlter();
    auto bassAlter = grp->getBass()->getBassAlter()->getValue().getValue();
    CHECK( core::StepEnum::c == rootStep );
    CHECK( !hasRootAlter );
    CHECK_DOUBLES_EQUAL( 0.0, rootAlter, 0.01 );
    CHECK( core::KindValue::majorSeventh == kind );
    CHECK( hasText );
    CHECK_EQUAL( "maj7", text );
    CHECK( !hasBass );
    CHECK( core::makeBassStep()->getValue() == bassStep );
    CHECK( !hasBassAlter );
    CHECK_DOUBLES_EQUAL( 0.0, bassAlter, 0.01 );

    measure = measureSet.at( 1 );
    mdc = measure->getMusicDataGroup()->getMusicDataChoiceSet().back();
    CHECK( core::MusicDataChoice::Choice::harmony == mdc->getChoice() );
    harmony = mdc->getHarmony();
    grp = harmony->getHarmonyChordGroupSet().front();
    rootStep = grp->getRoot()->getRootStep()->getValue();
    hasRootAlter = grp->getRoot()->getHasRootAlter();
    rootAlter = grp->getRoot()->getRootAlter()->getValue().getValue();
    kind = grp->getKind()->getValue();
    hasText = grp->getKind()->getAttributes()->hasText;
    text = grp->getKind()->getAttributes()->text.getValue();
    hasBass = grp->getHasBass();
    bassStep = grp->getBass()->getBassStep()->getValue();
    hasBassAlter = grp->getBass()->getHasBassAlter();
    bassAlter = grp->getBass()->getBassAlter()->getValue().getValue();
    CHECK( core::StepEnum::d == rootStep );
    CHECK( !hasRootAlter );
    CHECK_DOUBLES_EQUAL( 0.0, rootAlter, 0.01 );
    CHECK( core::KindValue::minor == kind );
    CHECK( hasText );
    CHECK_EQUAL( "m", text );
    CHECK( hasBass );
    CHECK( core::StepEnum::c == bassStep );
    CHECK( hasBassAlter );
    CHECK_DOUBLES_EQUAL( 1.0, bassAlter, 0.01 );

    measure = measureSet.at( 2 );
    mdc = measure->getMusicDataGroup()->getMusicDataChoiceSet().back();
    CHECK( core::MusicDataChoice::Choice::harmony == mdc->getChoice() );
    harmony = mdc->getHarmony();
    grp = harmony->getHarmonyChordGroupSet().front();
    rootStep = grp->getRoot()->getRootStep()->getValue();
    hasRootAlter = grp->getRoot()->getHasRootAlter();
    rootAlter = grp->getRoot()->getRootAlter()->getValue().getValue();
    kind = grp->getKind()->getValue();
    hasText = grp->getKind()->getAttributes()->hasText;
    text = grp->getKind()->getAttributes()->text.getValue();
    hasBass = grp->getHasBass();
    bassStep = grp->getBass()->getBassStep()->getValue();
    hasBassAlter = grp->getBass()->getHasBassAlter();
    bassAlter = grp->getBass()->getBassAlter()->getValue().getValue();
    CHECK( core::StepEnum::e == rootStep );
    CHECK( hasRootAlter );
    CHECK_DOUBLES_EQUAL( -1.0, rootAlter, 0.01 );
    CHECK( core::KindValue::dominant == kind );
    CHECK( hasText );
    CHECK_EQUAL( "7", text );
    CHECK( !hasBass );
    CHECK( core::makeBassStep()->getValue() == bassStep );
    CHECK( !hasBassAlter );
    CHECK_DOUBLES_EQUAL( 0.0, bassAlter, 0.01 );

    measure = measureSet.at( 3 );
    mdc = measure->getMusicDataGroup()->getMusicDataChoiceSet().back();
    CHECK( core::MusicDataChoice::Choice::harmony == mdc->getChoice() );
    harmony = mdc->getHarmony();
    grp = harmony->getHarmonyChordGroupSet().front();
    rootStep = grp->getRoot()->getRootStep()->getValue();
    hasRootAlter = grp->getRoot()->getHasRootAlter();
    rootAlter = grp->getRoot()->getRootAlter()->getValue().getValue();
    kind = grp->getKind()->getValue();
    hasText = grp->getKind()->getAttributes()->hasText;
    text = grp->getKind()->getAttributes()->text.getValue();
    hasBass = grp->getHasBass();
    bassStep = grp->getBass()->getBassStep()->getValue();
    hasBassAlter = grp->getBass()->getHasBassAlter();
    bassAlter = grp->getBass()->getBassAlter()->getValue().getValue();
    CHECK( core::StepEnum::b == rootStep );
    CHECK( !hasRootAlter );
    CHECK_DOUBLES_EQUAL( 0.0, rootAlter, 0.01 );
    CHECK( core::KindValue::diminishedSeventh == kind );
    CHECK( hasText );
    CHECK_EQUAL( "dim7", text );
    CHECK( !hasBass );
    CHECK( core::makeBassStep()->getValue() == bassStep );
    CHECK( !hasBassAlter );
    CHECK_DOUBLES_EQUAL( 0.0, bassAlter, 0.01 );

    CHECK( harmony->getHasOffset() );
    auto offset = harmony->getOffset();
    auto offsetValue = offset->getValue().getValue();
    CHECK_DOUBLES_EQUAL( 109.0, offsetValue, 0.01 );

}

#endif
