// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/testFramework.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/DocumentHeader.h"
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
    constexpr const char* const freezingFile = "freezing.xml";
    constexpr const char* const chordDirFile = "ChordDirectionPlacement.xml";
    constexpr const char* const hasVerFile = "HasMusicXmlVersionTrue.xml";
    constexpr const char* const noVerFile = "HasMusicXmlVersionFalse.xml";
    constexpr const char* const tupletType = "PreserveTimeModificationNormalType.xml";
}

namespace
{
    inline bool writeRoundTrip( std::string inFilename )
    {
        const auto nameWithoutExtension = mxtest::MxFileRepository::getNameWithoutExtension( inFilename );
        const auto outBeforeFilepath = std::string{ "./" } + nameWithoutExtension + std::string{ "_before.xml" };
        const auto outAfterFilepath = std::string{ "./" } + nameWithoutExtension + std::string{ "_after.xml" };
        const auto scoreData = mxtest::MxFileRepository::loadFile( inFilename );
        const auto filePath = mxtest::MxFileRepository::getFullPath( inFilename );
        auto& docMgr = DocumentManager::getInstance();
        const auto docId = docMgr.createFromFile( filePath );
        docMgr.writeToFile( docId, outBeforeFilepath );
        docMgr.destroyDocument( docId );
        const auto apiDocId = docMgr.createFromScore( scoreData );
        docMgr.writeToFile( apiDocId, outAfterFilepath );
        docMgr.destroyDocument( apiDocId );
        return docId != apiDocId;
    }

    struct TestData
    {
        const mx::core::ScorePartwisePtr originalScore;
        const mx::core::ScorePartwisePtr savedScore;
        const mx::api::ScoreData originalScoreData;
        const mx::api::ScoreData savedScoreData;

        TestData( mx::core::ScorePartwisePtr inOriginalScore,
                  mx::core::ScorePartwisePtr inSavedScore,
                  mx::api::ScoreData inOriginalScoreData,
                  mx::api::ScoreData inSavedScoreData )
        : originalScore{ inOriginalScore }
        , savedScore{ inSavedScore }
        , originalScoreData{ inOriginalScoreData }
        , savedScoreData{ inSavedScoreData }
        {

        }

        //////////////////////////// original score ///////////////////// saved score ///////////////////
        inline const std::pair<const mx::core::MusicDataChoiceSet, const mx::core::MusicDataChoiceSet>
        getMusicData( int inPartIndex, int inMeasureIndex ) const
        {
            const auto& oPset = this->originalScore->getPartwisePartSet();
            const auto& sPset = this->savedScore->getPartwisePartSet();
            const auto& oPart = oPset.at( static_cast<size_t>( inPartIndex ) );
            const auto& sPart = sPset.at( static_cast<size_t>( inPartIndex ) );
            const auto& oMdc = oPart->getPartwiseMeasureSet().at( static_cast<int>( inMeasureIndex ) )->getMusicDataGroup()->getMusicDataChoiceSet();
            const auto& sMdc = sPart->getPartwiseMeasureSet().at( static_cast<int>( inMeasureIndex ) )->getMusicDataGroup()->getMusicDataChoiceSet();
            return std::make_pair<const mx::core::MusicDataChoiceSet&, const mx::core::MusicDataChoiceSet&>( oMdc, sMdc );
        }
    };

    inline TestData getTestData( std::string filename )
    {
        auto& mgr = DocumentManager::getInstance();
        const auto filePath = mxtest::MxFileRepository::getFullPath( filename );
        const auto originalId = mgr.createFromFile( filePath );
        const auto originalScoreData = mgr.getData( originalId );
        const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
        const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
        const auto savedScoreData = mgr.getData( savedId );
        const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
        mgr.destroyDocument( originalId );
        mgr.destroyDocument( savedId );
        return TestData{ originalScore, savedScore, originalScoreData, savedScoreData };
    }
}

TEST( roundTripChordDirFile, Freezing )
{
    const auto success = writeRoundTrip( chordDirFile );
    CHECK( success );
}
T_END


TEST( chordDirectionPlacement, Freezing )
{
    const auto testData = getTestData( chordDirFile );
    const auto musicData = testData.getMusicData( 0, 0 );
    const auto oNumEntries = musicData.first.size();
    const auto sNumEntries = musicData.second.size();
    CHECK( oNumEntries > 0 );
    CHECK_EQUAL( oNumEntries, sNumEntries );

    for( size_t i = 0; i < oNumEntries; ++i )
    {
        const auto& o = musicData.first.at( i );
        const auto& s = musicData.second.at( i );
        const auto oChoice = o->getChoice();
        const auto sChoice = s->getChoice();
        std::stringstream ss;
        ss << "After round trip the MusicDataChoice elements are not the same. Currently index (i) == " << i << ".";
        ss << " ( oChoice == sChoice ) was supposed to return true, but returned false.";
        ss << " with ( " << static_cast<int>( oChoice ) << " == " << static_cast<int>( sChoice ) << " )";
        const std::string message = ss.str();
        CHECK_WITH_MESSAGE( oChoice == sChoice, message );
    }

    CHECK( true );
}
T_END


TEST( roundTripOutput, Freezing )
{
    const auto success = writeRoundTrip( freezingFile );
    CHECK( success );
}
T_END

TEST( roundTripViolaDynamicWrongTime, Freezing )
{
    // in the original file measure number="X7" implicit="yes" width="389"
    // or search for font-family="roundTripViolaDynamicWrongTime"
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
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

    // this is the first note in the measure
    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*originalMdcIter)->getChoice() );
    auto originalCurrentNote = (*originalMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( !originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    // this is the second note in the measure
    ++originalMdcIter;
    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*originalMdcIter)->getChoice() );
    originalCurrentNote = (*originalMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0, originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( originalCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    // this is where we find the first pp dynamic in the measure
    ++originalMdcIter;
    CHECK( originalMdcIter != originalMdcEnd );
    CHECK( MusicDataChoice::Choice::direction == (*originalMdcIter)->getChoice() );
    auto originalDirection = (*originalMdcIter)->getDirection();
    CHECK( DirectionType::Choice::dynamics == originalDirection->getDirectionTypeSet().front()->getChoice() );
    auto originalDynamics = originalDirection->getDirectionTypeSet().front()->getDynamicsSet().front();
    auto originalDynamicsValue = originalDynamics->getValue();
    CHECK( DynamicsEnum::pp == originalDynamicsValue.getValue() );
    const auto& originalDirections = originalScoreData.parts.at( partIndex ).measures.at( measureIndex ).staves.at( 0 ).directions;
    const auto originalDirectionsBegin = std::cbegin( originalDirections );
    const auto originalDirectionsEnd = std::cend( originalDirections );

    const auto findDirectionLambda = [&]( const DirectionData& inDirection )
    {
        if( inDirection.marks.size() == 1 )
        {
            const auto& mark = inDirection.marks.front();
            if( mark.markType == MarkType::pp )
            {
                return true;
            }
        }

        return false;
    };

    const auto directionIter = std::find_if( originalDirectionsBegin, originalDirectionsEnd, findDirectionLambda );
    const double tickTimeScaleFactor = 2.0;
    CHECK_DOUBLES_EQUAL( 30.0 * tickTimeScaleFactor, directionIter->tickTimePosition, 0.001 );

    const auto& savedPart = savedScore->getPartwisePartSet().at( partIndex );
    const auto& savedMeasure = savedPart->getPartwiseMeasureSet().at( measureIndex );
    const auto& savedMdcSet = savedMeasure->getMusicDataGroup()->getMusicDataChoiceSet();
    auto savedMdcIter = savedMdcSet.cbegin();
    const auto savedMdcEnd = savedMdcSet.cend();

    CHECK( savedMdcIter != savedMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*savedMdcIter)->getChoice() );
    auto savedCurrentNote = (*savedMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0 * tickTimeScaleFactor, savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
    CHECK( !savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

    ++savedMdcIter;
    CHECK( savedMdcIter != savedMdcEnd );
    CHECK( MusicDataChoice::Choice::note == (*savedMdcIter)->getChoice() );
    savedCurrentNote = (*savedMdcIter)->getNote();
    CHECK_DOUBLES_EQUAL( 30.0 * tickTimeScaleFactor, savedCurrentNote->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.0001 );
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


TEST( hasVersion, Freezing )
{
    const auto testData = getTestData( hasVerFile );
    const bool hasVersion = testData.savedScore->getAttributes()->hasVersion;
    CHECK( hasVersion );
    const auto version = testData.savedScore->getAttributes()->version.getValue();
    CHECK_EQUAL( "3.0", version );
}
T_END


TEST( hasNoVersion, Freezing )
{
    const auto testData = getTestData( noVerFile );
    const bool hasVersion = testData.savedScore->getAttributes()->hasVersion;
    CHECK( !hasVersion );
}
T_END


TEST( missingMusicXMLVersion, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
    const auto originalId = mgr.createFromFile( filePath );
    const auto originalScoreData = mgr.getData( originalId );
    const auto originalScore = mgr.getDocument( originalId )->getScorePartwise();
    const auto savedId = mgr.createFromScore( mgr.getData( originalId ) );
    const auto savedScoreData = mgr.getData( savedId );
    const auto savedScore = mgr.getDocument( savedId )->getScorePartwise();
    mgr.destroyDocument( originalId );
    mgr.destroyDocument( savedId );

    const bool originalScoreHasVersion = originalScore->getAttributes()->hasVersion;
    const bool savedScoreHasVersion = savedScore->getAttributes()->hasVersion;
    CHECK( originalScoreHasVersion );
    CHECK( savedScoreHasVersion );
}

TEST( HasDefaultsHasAppearance, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
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

    const auto originalAppearance = originalScore->getScoreHeaderGroup()->getDefaults()->getAppearance();
    const auto savedAppearance = savedScore->getScoreHeaderGroup()->getDefaults()->getAppearance();

    for( size_t i = 0; i < originalAppearance->getLineWidthSet().size(); ++i )
    {
        CHECK_DOUBLES_EQUAL( originalAppearance->getLineWidthSet().at( i )->getValue().getValue(), savedAppearance->getLineWidthSet().at( i )->getValue().getValue(), 0.0001 );
        CHECK( originalAppearance->getLineWidthSet().at( i )->getAttributes()->type.getValueString() == savedAppearance->getLineWidthSet().at( i )->getAttributes()->type.getValueString() );
    }

    for( size_t i = 0; i < originalAppearance->getNoteSizeSet().size(); ++i )
    {
        CHECK_DOUBLES_EQUAL( originalAppearance->getNoteSizeSet().at( i )->getValue().getValue(), savedAppearance->getNoteSizeSet().at( i )->getValue().getValue(), 0.0001 );
        CHECK( originalAppearance->getNoteSizeSet().at( i )->getAttributes()->type == savedAppearance->getNoteSizeSet().at( i )->getAttributes()->type );
    }

    for( size_t i = 0; i < originalAppearance->getDistanceSet().size(); ++i )
    {
        CHECK_DOUBLES_EQUAL( originalAppearance->getDistanceSet().at( i )->getValue().getValue(), savedAppearance->getDistanceSet().at( i )->getValue().getValue(), 0.0001 );
        CHECK( originalAppearance->getDistanceSet().at( i )->getAttributes()->type.getValueString() == savedAppearance->getDistanceSet().at( i )->getAttributes()->type.getValueString() );
    }
}

TEST( appearanceLineWidths, Freezing )
{
    auto& mgr = DocumentManager::getInstance();
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
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
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
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
    const auto filePath = mxtest::MxFileRepository::getFullPath( freezingFile );
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

TEST( checkMissingNormalTypeSimple, Freezing )
{
    const auto testData = getTestData( tupletType );
    const auto musicData = testData.getMusicData( 0, 0 );
    CHECK( musicData.second.at( 1 )->getNote()->getTimeModification()->getHasNormalTypeNormalDotGroup() );
    CHECK( mx::core::NoteTypeValue::eighth == musicData.second.at( 1 )->getNote()->getTimeModification()->getNormalTypeNormalDotGroup()->getNormalType()->getValue() );
}

TEST( checkMissingNormalType, Freezing )
{
    const auto testData = getTestData( freezingFile );
    const auto partCount = testData.originalScore->getPartwisePartSet().size();

    for( size_t partIndex = 0; partIndex < partCount; ++partIndex )
    {
        const auto measureCount = testData.originalScore->getPartwisePartSet().at( partIndex )->getPartwiseMeasureSet().size();

        for( size_t measureIndex = 0; measureIndex < measureCount; ++measureIndex )
        {
            const auto music = testData.getMusicData( static_cast<int>( partIndex ), static_cast<int>( measureIndex ) );

            const auto filterLambda = [&]( const mx::core::MusicDataChoicePtr& inMdc )
            {
                if( inMdc->getChoice() != mx::core::MusicDataChoice::Choice::note )
                {
                    return false;
                }

                const auto& note = inMdc->getNote();

                if( !note->getHasTimeModification() )
                {
                    return false;
                }
                
                return true;
            };

            MusicDataChoiceSet originalNotes;
            MusicDataChoiceSet savedNotes;
            std::copy_if( std::cbegin( music.first ), std::cend( music.first ), std::back_inserter( originalNotes ), filterLambda );
            std::copy_if( std::cbegin( music.second ), std::cend( music.second ), std::back_inserter( savedNotes ), filterLambda );
            {
                std::stringstream message1;
                message1 << "( originalNotes.size() == savedNotes.size() ), ";
                message1 << "( " << originalNotes.size() << " == " << savedNotes.size() << " ), ";
                message1 << " partIndex = " << partIndex << ", measureIndex = " << measureIndex;
                CHECK_WITH_MESSAGE( originalNotes.size() == savedNotes.size(), message1.str() );
            }
            const size_t numNotes = originalNotes.size();
            for( int i = 0; i < numNotes; ++i )
            {
                CHECK( savedNotes.at( i )->getNote()->getTimeModification()->getHasNormalTypeNormalDotGroup() );
                
                if( originalNotes.at( i )->getNote()->getTimeModification()->getHasNormalTypeNormalDotGroup() )
                {
                    const auto originalType = originalNotes.at( i )->getNote()->getTimeModification()->getNormalTypeNormalDotGroup()->getNormalType()->getValue();
                    const auto savedType = savedNotes.at( i )->getNote()->getTimeModification()->getNormalTypeNormalDotGroup()->getNormalType()->getValue();
                    
                    if( originalType != savedType )
                    {
                        std::stringstream message2;
                        message2 << "( originalType == savedType ), ";
                        message2 << "( " << mx::core::toString( originalType ) << " == " << mx::core::toString( savedType ) << " ), ";
                        message2 << " partIndex = " << partIndex << ", measureIndex = " << measureIndex;
                        CHECK_WITH_MESSAGE( originalType == savedType, message2.str() );
                    }
                }
            }
        }
    }
}

#endif
