// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DocumentManager.h"
#include <sstream>
#include "mx/utility/Throw.h"

namespace mxtest
{
    inline std::string toXml( const mx::api::ScoreData& inScoreData )
    {
        using namespace mx::api;
        auto& docMgr = DocumentManager::getInstance();
        const auto docId = docMgr.createFromScore( inScoreData );
        std::stringstream ss;
        docMgr.writeToStream( docId, ss );
        docMgr.destroyDocument( docId );
        const auto xml = ss.str();
        return xml;
    }


    inline mx::api::ScoreData fromXml( const std::string& inXml )
    {
        MX_LOG( "trace" );
        using namespace mx::api;
        auto& docMgr = DocumentManager::getInstance();
        std::istringstream iss{ inXml };
        const auto docId = docMgr.createFromStream( iss );
        MX_LOG( "trace" );
        const auto score = docMgr.getData( docId );
        MX_LOG( "trace" );
        docMgr.destroyDocument( docId );
        MX_LOG( "trace" );
        return score;
    }
}
