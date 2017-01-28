// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/LineData.h"
#include "mx/api/DurationData.h"

namespace mx
{
    namespace api
    {
        // in the tuplet ratio, for example 2:3 for
        // triplets, the 'actual' part of the ratio
        // is the '2' and the 'normal' part of the
        // ratio is the '3'

        class TupletStart
        {
        public:

            // used to id when nested
            // -1 means unspecified
            int numberLevel;
            
            PositionData positionData;

            int actualNumber;
            DurationName actualDurationName;
            int actualDots;
            
            int normalNumber;
            DurationName normalDurationName;
            int normalDots;
            
            // show the number of notes in the tuplet
            // description ratio, for example 3 for triplets
            Bool showActualNumber;
            
            // show the number of 'normal' notes in the
            // tuplet ratio, for example the 2 in 3:2 for
            // a triplet or the 4 in 5:4 for quintuplet
            Bool showNormalNumber;
            
            Bool bracket;
            
            TupletStart()
            : numberLevel{ -1 }
            , positionData{}
            , actualNumber{ -1 }
            , actualDurationName{ api::DurationName::unspecified }
            , actualDots{ -1 }
            , normalNumber{ -1 }
            , normalDurationName{ api::DurationName::unspecified }
            , normalDots{ -1 }
            , showActualNumber{ Bool::unspecified }
            , showNormalNumber{ Bool::unspecified }
            , bracket{ Bool::unspecified }
            {
                
            }
        };

        class TupletStop
        {
        public:
            // used to id when nested
            // -1 means unspecified
            int numberLevel;
            
            PositionData positionData;
            
            TupletStop()
            : numberLevel{ -1 }
            , positionData{}
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( TupletStart )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( actualNumber )
        MXAPI_EQUALS_MEMBER( actualDurationName )
        MXAPI_EQUALS_MEMBER( actualDots )
        MXAPI_EQUALS_MEMBER( normalNumber )
        MXAPI_EQUALS_MEMBER( normalDurationName )
        MXAPI_EQUALS_MEMBER( normalDots )
        MXAPI_EQUALS_MEMBER( showActualNumber )
        MXAPI_EQUALS_MEMBER( showNormalNumber )
        MXAPI_EQUALS_MEMBER( bracket )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TupletStart );
        
        MXAPI_EQUALS_BEGIN( TupletStop )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TupletStop );
    }
}
