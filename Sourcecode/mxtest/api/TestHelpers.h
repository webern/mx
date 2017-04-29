// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DocumentManager.h"
#include <sstream>

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
}
