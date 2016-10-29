// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_ROUNDTRIP

#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"

namespace mxtest
{

    
constexpr const char* const roundTripFileName = "testChord.xml";

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
}

#endif
