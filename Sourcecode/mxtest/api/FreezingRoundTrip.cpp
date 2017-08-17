// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mxtest/api/RoundTrip.h"

// TODO - replace with specific includes
#include "mx/core/Elements.h"

using namespace std;
using namespace mx::api;
using namespace mx::core;
using namespace mxtest;

namespace
{
    constexpr const char* const fileName = "freezing.xml";
}


TEST( roundTripOutput, Freezing )
{
    const auto scoreData = mxtest::MxFileRepository::loadFile( fileName );
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    auto& docMgr = DocumentManager::getInstance();
    const auto docId = docMgr.createFromFile( filePath );
    docMgr.writeToFile( docId, "./freezing_before.xml" );
    docMgr.destroyDocument( docId );
    const auto apiDocId = docMgr.createFromScore( scoreData );
    docMgr.writeToFile( apiDocId, "./freezing_after.xml" );
    docMgr.destroyDocument( apiDocId );
}
T_END

TEST( roundTripViolaDynamicWrongTime, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const int partIndex = 0;
    const int measureIndex = 7;

    const auto& originalPart = originalScore->getPartwisePartSet().at( partIndex );
    const auto& originalMeasure = originalPart->getPartwiseMeasureSet().at( measureIndex );
    const auto& originalMdcSet = originalMeasure->getMusicDataGroup()->getMusicDataChoiceSet();
    auto originalMdcIter = originalMdcSet.cbegin();
    const auto originalMdcEnd = originalMdcSet.cend();

    while( ( originalMdcIter != originalMdcEnd ) && ( (*originalMdcIter)->getChoice() != MusicDataChoice::Choice::note ) )
    {
        ++originalMdcIter;
    }

    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*originalMdcIter)->getChoice() );
    auto originalCurrentNote = (*originalMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( !originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    ++originalMdcIter;
    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*originalMdcIter)->getChoice() );
    originalCurrentNote = (*originalMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    ++originalMdcIter;
    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::direction == (*originalMdcIter)->getChoice() );
    auto originalDirection = (*originalMdcIter)->getDirection();
    CHECK( DirectionType::Choice::dynamics == originalDirection->getDirectionTypeSet().front()->getChoice() );
    auto originalDynamics = originalDirection->getDirectionTypeSet().front()->getDynamicsSet().front();
    auto originalDynamicsValue = originalDynamics->getValue();
    CHECK( DynamicsEnum::pp == originalDynamicsValue.getValue() );

    const auto& savedPart = savedScore->getPartwisePartSet().at( partIndex );
    const auto& savedMeasure = savedPart->getPartwiseMeasureSet().at( measureIndex );
    const auto& savedMdcSet = savedMeasure->getMusicDataGroup()->getMusicDataChoiceSet();
    auto savedMdcIter = savedMdcSet.cbegin();
    const auto savedMdcEnd = savedMdcSet.cend();

    CHECK( savedMdcIter != savedMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*savedMdcIter)->getChoice() );
    auto savedCurrentNote = (*savedMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( !savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    ++savedMdcIter;
    CHECK( savedMdcIter != savedMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*savedMdcIter)->getChoice() );
    savedCurrentNote = (*savedMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    ++savedMdcIter;
    CHECK( savedMdcIter != savedMdcEnd );
    CHECK( MusicDataChoice::Choice::direction == (*savedMdcIter)->getChoice() );
    auto savedDirection = (*savedMdcIter)->getDirection();
    CHECK( DirectionType::Choice::dynamics == savedDirection->getDirectionTypeSet().front()->getChoice() );
    auto savedDynamics = savedDirection->getDirectionTypeSet().front()->getDynamicsSet().front();
    auto savedDynamicsValue = savedDynamics->getValue();
    CHECK( DynamicsEnum::pp == savedDynamicsValue.getValue() );

}
T_END

TEST( missingMusicXMLVersion, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    CHECK( originalScore->getAttributes()->hasVersion == savedScore->getAttributes()->hasVersion );
}

TEST( HasDefaultsHasAppearance, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const auto originalHasDefaults = originalScore->getScoreHeaderGroup()->getHasDefaults();
    const auto savedHasDefaults = savedScore->getScoreHeaderGroup()->getHasDefaults();
    CHECK( originalHasDefaults == savedHasDefaults );

    const auto originalHasAppearance = originalScore->getScoreHeaderGroup()->getDefaults()->getHasAppearance();
    const auto savedHasAppearance = savedScore->getScoreHeaderGroup()->getDefaults()->getHasAppearance();
    CHECK( originalHasAppearance == savedHasAppearance );
}

TEST( appearanceLineWidths, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const auto originalAppearance = originalScore->getScoreHeaderGroup()->getDefaults()->getAppearance();
    const auto savedAppearance = savedScore->getScoreHeaderGroup()->getDefaults()->getAppearance();

    const auto lineWidthSetSize = savedAppearance->getLineWidthSet().size();
    CHECK( lineWidthSetSize > 0 );
    CHECK_EQUAL( originalAppearance->getLineWidthSet().size(), savedAppearance->getLineWidthSet().size() );

    for( int i = 0; i < lineWidthSetSize; ++i )
    {
        const auto savedElement = savedAppearance->getLineWidthSet().at( i );
        const auto originalElement = originalAppearance->getLineWidthSet().at( i );
        const auto savedLineWidth = savedElement->getValue().getValue();
        const auto originalLineWidth = originalElement->getValue().getValue();
        CHECK_DOUBLES_EQUAL( originalLineWidth, savedLineWidth, 0.00001 );

        CHECK( originalElement->getAttributes()->hasType == savedElement->getAttributes()->hasType );
        CHECK_EQUAL( originalElement->getAttributes()->type.getValueString(), savedElement->getAttributes()->type.getValueString() );
    }
}

TEST( appearanceNoteSize, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const auto originalAppearance = originalScore->getScoreHeaderGroup()->getDefaults()->getAppearance();
    const auto savedAppearance = savedScore->getScoreHeaderGroup()->getDefaults()->getAppearance();

    const auto noteSizeSetSize = savedAppearance->getNoteSizeSet().size();
    CHECK( noteSizeSetSize > 0 );
    CHECK_EQUAL( originalAppearance->getNoteSizeSet().size(), savedAppearance->getNoteSizeSet().size() );

    for( int i = 0; i < noteSizeSetSize; ++i )
    {
        const auto savedElement = savedAppearance->getNoteSizeSet().at( i );
        const auto originalElement = originalAppearance->getNoteSizeSet().at( i );
        const auto savedNoteSize = savedElement->getValue().getValue();
        const auto originalNoteSize = originalElement->getValue().getValue();
        CHECK_DOUBLES_EQUAL( originalNoteSize, savedNoteSize, 0.00001 );

        CHECK( originalElement->getAttributes()->hasType == savedElement->getAttributes()->hasType );
        CHECK( originalElement->getAttributes()->type == savedElement->getAttributes()->type );
    }
}



TEST( appearancDistance, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( fileName );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const auto originalAppearance = originalScore->getScoreHeaderGroup()->getDefaults()->getAppearance();
    const auto savedAppearance = savedScore->getScoreHeaderGroup()->getDefaults()->getAppearance();

    const auto distanceSetSize = savedAppearance->getDistanceSet().size();
    CHECK( distanceSetSize > 0 );
    CHECK_EQUAL( originalAppearance->getDistanceSet().size(), savedAppearance->getDistanceSet().size() );

    for( int i = 0; i < distanceSetSize; ++i )
    {
        const auto savedElement = savedAppearance->getDistanceSet().at( i );
        const auto originalElement = originalAppearance->getDistanceSet().at( i );
        const auto savedDistance = savedElement->getValue().getValue();
        const auto originalDistance = originalElement->getValue().getValue();
        CHECK_DOUBLES_EQUAL( originalDistance, savedDistance, 0.00001 );

        CHECK( originalElement->getAttributes()->hasType == savedElement->getAttributes()->hasType );
        CHECK( originalElement->getAttributes()->type.getValueString() == savedElement->getAttributes()->type.getValueString() );
    }
}

#endif
