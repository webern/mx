// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PrintData.h"
#include "mx/api/PositionData.h"
#include "mx/api/MarkData.h"
#include "mx/api/CurveData.h"

namespace mx
{
    namespace api
    {
    	enum class DurationName
    	{
    		unspecified,
            maxima,
    		longa,
    		breve,
    		whole,
    		quarter,
            half,
    		eighth,
    		dur16th,
    		dur32nd,
    		dur64th,
    		dur128th,
    		dur256th,
    		dur512th,
    		dur1024th
    	};
        
        
        constexpr long double DUR_QUARTERS_VALUE_MAXIMA = 32.0L;
        constexpr long double DUR_QUARTERS_VALUE_LONGA = 16.0L;
        constexpr long double DUR_QUARTERS_VALUE_BREVE = 8.0L;
        constexpr long double DUR_QUARTERS_VALUE_WHOLE = 4.0L;
        constexpr long double DUR_QUARTERS_VALUE_HALF = 2.0L;
        constexpr long double DUR_QUARTERS_VALUE_QUARTER = 1.0L;
        constexpr long double DUR_QUARTERS_VALUE_EIGHTH = 1.0L / 2.0L;
        constexpr long double DUR_QUARTERS_VALUE_16TH = 1.0L / 4.0L;
        constexpr long double DUR_QUARTERS_VALUE_32ND = 1.0L / 8.0L;
        constexpr long double DUR_QUARTERS_VALUE_64TH = 1.0L / 16.0L;
        constexpr long double DUR_QUARTERS_VALUE_128TH = 1.0L / 32.0L;
        constexpr long double DUR_QUARTERS_VALUE_256TH = 1.0L / 64.0L;
        constexpr long double DUR_QUARTERS_VALUE_512TH = 1.0L / 128.0L;
        constexpr long double DUR_QUARTERS_VALUE_1024TH = 1.0L / 256.0L;
        
        struct DurationData
        {
        	DurationData();

        	DurationName durationName;                    // i.e. quarter, eighth etc
            int durationDots;                             // dots
            int durationTimeTicks;                        // length of the note denominated in ticksPerQuarter
            bool isTied;								  // affects sound only. is the note combined sound-wise with the following note of the same pitch
            int timeModificationActualNotes;              // i.e. for a triplet this would be 3
            int timeModificationNormalNotes;              // i.e. for a triplet this would be 2
            api::DurationName timeModificationNormalType; // use this if the time modification ratio involves a different durationName than the main 'durationName'
            int timeModificationNormalTypeDots;           // in case the time modification ratio involves a dotted type
        };
        
        MXAPI_EQUALS_BEGIN( DurationData )
        MXAPI_EQUALS_FIRST_MEMBER( durationName )
        MXAPI_EQUALS_NEXT_MEMBER( durationDots )
        MXAPI_EQUALS_NEXT_MEMBER( durationTimeTicks )
        MXAPI_EQUALS_NEXT_MEMBER( isTied )
        MXAPI_EQUALS_NEXT_MEMBER( timeModificationActualNotes )
        MXAPI_EQUALS_NEXT_MEMBER( timeModificationNormalNotes )
        MXAPI_EQUALS_NEXT_MEMBER( timeModificationNormalType )
        MXAPI_EQUALS_LAST_MEMBER( timeModificationNormalTypeDots )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( DurationData );
    }
}
