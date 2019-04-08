// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_ROUNDTRIP

#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"

namespace mxtest
{
    
    
    constexpr const char* const roundTripFileName = "k007a_Notations_Dynamics.xml";
    
    inline void roundTrip()
    {
        const std::string path{ MxFileRepository::getFullPath( roundTripFileName ) };
        auto& docMgr = mx::api::DocumentManager::getInstance();
        auto docId = docMgr.createFromFile( path );
        auto scoreData = docMgr.getData( docId );
        docMgr.destroyDocument( docId );
        docId = docMgr.createFromScore( scoreData );
        docMgr.writeToFile( docId, "./output.xml" );
        docMgr.destroyDocument( docId );
    }
    
    inline mx::api::ScoreData roundTrip( const mx::api::ScoreData inScoreData )
    {
        auto& docMgr = mx::api::DocumentManager::getInstance();
        auto docId = docMgr.createFromScore( inScoreData );
        std::stringstream ss;
        docMgr.writeToStream(docId, ss);
        docMgr.destroyDocument(docId);
        auto xmlData = ss.str();
        std::istringstream iss{ xmlData };
        docId = docMgr.createFromStream( iss );
        auto outScoreData = docMgr.getData( docId );
        docMgr.destroyDocument( docId );
        //std::cout << xmlData << std::endl;
        return outScoreData;
    }
}

#endif
