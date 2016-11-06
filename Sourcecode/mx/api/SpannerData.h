// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/LineData.h"

#include <string>

namespace mx
{
    namespace api
    {

        struct SpannerStart
        {
            int numberLevel;
            int tickTimePosition;
            PositionData positionData;
            PrintData printData;
            LineData lineData;
            
            SpannerStart()
            : numberLevel{ -1 }
            , tickTimePosition{ 0 }
            , positionData{}
            , printData{}
            , lineData{}
            {
                
            }
        };
        
        struct SpannerStop
        {
            int numberLevel;
            int tickTimePosition;
            PositionData positionData;
            
            SpannerStop()
            : numberLevel{ -1 }
            , tickTimePosition{ 0 }
            , positionData{}
            {
                
            }
        };
                
        MXAPI_EQUALS_BEGIN( SpannerStart )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( printData )
        MXAPI_EQUALS_MEMBER( lineData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SpannerStart );
        
        MXAPI_EQUALS_BEGIN( SpannerStop )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SpannerStop );
    }
}
