// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include <sstream>
#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"

using namespace std;
using namespace mx::api;
//using namespace mxtest;

namespace
{
    ScoreData putKeyInScore( KeyData key )
    {
        ScoreData score;
        score.parts.push_back( PartData{} );
        auto& part = score.parts.back();
        part.measures.emplace_back( );
        auto& measure = part.measures.back();
        measure.keys.push_back( key );
        return score;
    }
}

//TEST( CMajor, KeyData )
//{
//    KeyData key;
//    const auto original = putKeyInScore( key );
//    auto& docMgr = DocumentManager::getInstance();
//    const int originalId = docMgr.createFromScore( original );
//    const mx::core::DocumentPtr corePtr = docMgr.getDocument( originalId );
//    const auto coreScorePtr = corePtr->getScorePartwise();
//    const auto coreParts = coreScorePtr->getPartwisePartSet();
//    CHECK_EQUAL( 1, coreParts.size() );
//
//}

TEST( EMajor, KeyData )
{
    KeyData key;
    key.fifths = 4;
    key.cancel = -1;
    key.mode = KeyMode::major;
    key.staffIndex = 0;

    const auto original = putKeyInScore( key );
    auto& docMgr = DocumentManager::getInstance();
    const int originalId = docMgr.createFromScore( original );
    const mx::core::DocumentPtr corePtr = docMgr.getDocument( originalId );
    const auto coreScorePtr = corePtr->getScorePartwise();
    const auto coreParts = coreScorePtr->getPartwisePartSet();
    REQUIRE( 1 == coreParts.size() );
    const auto corePart = coreParts.at( 0 );
    const auto coreMeasures = corePart->getPartwiseMeasureSet();
    REQUIRE( 1 == coreMeasures.size() );
    const auto coreMeasure = coreMeasures.at( 0 );
    const auto coreKeySet = coreMeasure->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getProperties()->getKeySet();
    REQUIRE( 1 == coreKeySet.size() );
    const auto coreKeyChoice = coreKeySet.at( 0 );
    CHECK_EQUAL( mx::core::KeyChoice::Choice::traditionalKey, coreKeyChoice->getKeyChoice()->getChoice() );
    const auto coreKey = coreKeyChoice->getKeyChoice()->getTraditionalKey();

    // check if the staff number was written
    CHECK( coreKeyChoice->getAttributes()->hasNumber )
    CHECK_EQUAL( key.staffIndex, coreKeyChoice->getAttributes()->number.getValue() - 1 );

    // check the mode
    CHECK( coreKey->getHasMode() )
    CHECK_EQUAL( mx::core::ModeEnum::major, coreKey->getMode()->getValue().getValue() );

    // check the cancel
    CHECK( coreKey->getHasCancel() )
    CHECK_EQUAL( -1, coreKey->getCancel()->getValue().getValue() );

    // check the fifths
    CHECK_EQUAL( 4, coreKey->getFifths()->getValue().getValue() );

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream( originalId, xml );
    docMgr.destroyDocument( originalId );
    std::istringstream iss{ xml.str() };
    const auto deserializedId = docMgr.createFromStream( iss );
    const auto deserializedScore = docMgr.getData( deserializedId );
    docMgr.destroyDocument( deserializedId );
    const auto& deserializedKeys = deserializedScore.parts.at( 0 ).measures.at( 0 ).keys;
    CHECK_EQUAL( 1, deserializedKeys.size() )
    const auto deserializedKey = deserializedKeys.at( 0 );

    CHECK_EQUAL( key.staffIndex, deserializedKey.staffIndex );
    CHECK_EQUAL( key.cancel, deserializedKey.cancel );
    CHECK_EQUAL( key.fifths, deserializedKey.fifths );
    CHECK_EQUAL( key.mode , deserializedKey.mode );
    CHECK_EQUAL( 0, deserializedKey.tickTimePosition );
    CHECK( deserializedKey.custom.empty() );
}

TEST( AbMinor, KeyData )
{
    KeyData key;
    key.fifths = -7;
    key.cancel = 0;
    key.mode = KeyMode::minor;
    key.staffIndex = -10;

    const auto original = putKeyInScore( key );
    auto& docMgr = DocumentManager::getInstance();
    const int originalId = docMgr.createFromScore( original );
    const mx::core::DocumentPtr corePtr = docMgr.getDocument( originalId );
    const auto coreScorePtr = corePtr->getScorePartwise();
    const auto coreParts = coreScorePtr->getPartwisePartSet();
    REQUIRE( 1 == coreParts.size() );
    const auto corePart = coreParts.at( 0 );
    const auto coreMeasures = corePart->getPartwiseMeasureSet();
    REQUIRE( 1 == coreMeasures.size() );
    const auto coreMeasure = coreMeasures.at( 0 );
    const auto coreKeySet = coreMeasure->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getProperties()->getKeySet();
    REQUIRE( 1 == coreKeySet.size() );
    const auto coreKeyChoice = coreKeySet.at( 0 );
    CHECK_EQUAL( mx::core::KeyChoice::Choice::traditionalKey, coreKeyChoice->getKeyChoice()->getChoice() );
    const auto coreKey = coreKeyChoice->getKeyChoice()->getTraditionalKey();

    // check if the staff number was written
    CHECK( !coreKeyChoice->getAttributes()->hasNumber )

    // check the mode
    CHECK( coreKey->getHasMode() )
    CHECK_EQUAL( mx::core::ModeEnum::minor, coreKey->getMode()->getValue().getValue() );

    // check the cancel
    CHECK( !coreKey->getHasCancel() )
    CHECK_EQUAL( 0, coreKey->getCancel()->getValue().getValue() );

    // check the fifths
    CHECK_EQUAL( key.fifths, coreKey->getFifths()->getValue().getValue() );

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream( originalId, xml );
    docMgr.destroyDocument( originalId );
    std::istringstream iss{ xml.str() };
    const auto deserializedId = docMgr.createFromStream( iss );
    const auto deserializedScore = docMgr.getData( deserializedId );
    docMgr.destroyDocument( deserializedId );
    const auto& deserializedKeys = deserializedScore.parts.at( 0 ).measures.at( 0 ).keys;
    CHECK_EQUAL( 1, deserializedKeys.size() )
    const auto deserializedKey = deserializedKeys.at( 0 );

    CHECK_EQUAL( -1, deserializedKey.staffIndex );
    CHECK_EQUAL( key.cancel, deserializedKey.cancel );
    CHECK_EQUAL( key.fifths, deserializedKey.fifths );
    CHECK_EQUAL( key.mode , deserializedKey.mode );
    CHECK_EQUAL( 0, deserializedKey.tickTimePosition );
    CHECK( deserializedKey.custom.empty() );
}

TEST( NonTraditional1, KeyData )
{
    KeyData key;
    key.fifths = 1; // should be ignored
    key.cancel = 2;
    key.mode = KeyMode::minor; // should be ignored
    key.staffIndex = -1;
    KeyComponent cSharp{ Step::c, 1, 0.0, Accidental::sharp };
    KeyComponent dQuarterFlat{ Step::d, 0, -50.0, Accidental::quarterFlat };

    const auto original = putKeyInScore( key );
    auto& docMgr = DocumentManager::getInstance();
    const int originalId = docMgr.createFromScore( original );
    const mx::core::DocumentPtr corePtr = docMgr.getDocument( originalId );
    const auto coreScorePtr = corePtr->getScorePartwise();
    const auto coreParts = coreScorePtr->getPartwisePartSet();
    REQUIRE( 1 == coreParts.size() );
    const auto corePart = coreParts.at( 0 );
    const auto coreMeasures = corePart->getPartwiseMeasureSet();
    REQUIRE( 1 == coreMeasures.size() );
    const auto coreMeasure = coreMeasures.at( 0 );
    const auto coreKeySet = coreMeasure->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getProperties()->getKeySet();
    REQUIRE( 1 == coreKeySet.size() );
    const auto coreKeyChoice = coreKeySet.at( 0 );
    CHECK_EQUAL( mx::core::KeyChoice::Choice::nonTraditionalKey, coreKeyChoice->getKeyChoice()->getChoice() );
    const auto coreKeyComponents = coreKeyChoice->getKeyChoice()->getNonTraditionalKeySet();
    REQUIRE( 2 == coreKeyComponents.size() );

    // check if the staff number was written
    CHECK( !coreKeyChoice->getAttributes()->hasNumber )

    // check a key component
    size_t keyComponentIndex = 0;
    auto keyComponent = coreKeyComponents.at( keyComponentIndex );
    CHECK_EQUAL( mx::core::StepEnum::c, keyComponent->getKeyStep()->getValue() );
    CHECK_DOUBLES_EQUAL( 0.0, keyComponent->getKeyAlter()->getValue().getValue(), 0.0001 )
    CHECK( keyComponent->getHasKeyAccidental() );
    CHECK_EQUAL( mx::core::AccidentalValue::sharp, keyComponent->getKeyAccidental()->getValue() );

    // check a key component
    keyComponentIndex = 1;
    keyComponent = coreKeyComponents.at( keyComponentIndex );
    CHECK_EQUAL( mx::core::StepEnum::d, keyComponent->getKeyStep()->getValue() );
    CHECK_DOUBLES_EQUAL( -0.25, keyComponent->getKeyAlter()->getValue().getValue(), 0.0001 )
    CHECK( keyComponent->getHasKeyAccidental() );
    CHECK_EQUAL( mx::core::AccidentalValue::quarterFlat, keyComponent->getKeyAccidental()->getValue() );

    // serialize and deserialize
    std::stringstream xml;
    docMgr.writeToStream( originalId, xml );
    docMgr.destroyDocument( originalId );
    std::istringstream iss{ xml.str() };
    const auto deserializedId = docMgr.createFromStream( iss );
    const auto deserializedScore = docMgr.getData( deserializedId );
    docMgr.destroyDocument( deserializedId );
    const auto& deserializedKeys = deserializedScore.parts.at( 0 ).measures.at( 0 ).keys;
    CHECK_EQUAL( 1, deserializedKeys.size() )
    const auto deserializedKey = deserializedKeys.at( 0 );

    CHECK_EQUAL( -1, deserializedKey.staffIndex );
    CHECK_EQUAL( key.cancel, deserializedKey.cancel );
    CHECK_EQUAL( 0, deserializedKey.fifths );
    CHECK_EQUAL( KeyMode::unspecified, deserializedKey.mode );
    CHECK_EQUAL( 0, deserializedKey.tickTimePosition );
    CHECK_EQUAL( 2, deserializedKey.custom.size() );

    CHECK_EQUAL( cSharp, deserializedKey.custom.at( 0 ) );
    CHECK_EQUAL( dQuarterFlat, deserializedKey.custom.at( 1 ) );

    std::cout << xml.str() << std::endl;
}

// TODO - test KeyComponent equality operator
// TODO - test KeyData equality operator

#endif