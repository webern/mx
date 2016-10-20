// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_ROUNDTRIP

#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"

namespace mxtest
{

    
constexpr const char* const roundTripFileName =  "ly43e_Multistaff_ClefDynamics.xml";
    //#define MX_COMPILE_API_ROUNDTRIP "ly11a_TimeSignatures.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "ly01a_Pitches_Pitches.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "ly02c_Rests_MultiMeasureRests.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "freezing.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "FreezingSmall.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "OctoberXML.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "k011a_Tuplets.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "ly33a_Spanners.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "k012a_Tempo_Markings.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "ly13a_KeySignatures.xml"
    //#define MX_COMPILE_API_ROUNDTRIP "k001a_Articulations.xml"


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
