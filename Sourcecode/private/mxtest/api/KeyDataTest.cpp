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
    const auto& deserializedKeys = deserializedScore.parts.at( 0 ).measures.at( 0 ).staves.at( 0 ).keys;
    CHECK_EQUAL( 1, deserializedKeys.size() )

    std::cout << xml.str() << std::endl;
}

#endif