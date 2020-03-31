// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
        part.measures.push_back( MeasureData() );
        auto& measure = part.measures.back();
        measure.keys.push_back( key );
        return score;
    }
}

TEST( CMajor, KeyData )
{
    KeyData key;
    const auto original = putKeyInScore( key );
    auto& docMgr = DocumentManager::getInstance();
    const int originalId = docMgr.createFromScore( original );
    const mx::core::DocumentPtr corePtr = docMgr.getDocument( originalId );
    const auto coreScorePtr = corePtr->getScorePartwise();
    const auto coreParts = coreScorePtr->getPartwisePartSet();
    CHECK_EQUAL( 1, coreParts.size() );
}

#endif