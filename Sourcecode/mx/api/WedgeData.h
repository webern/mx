// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/ColorData.h"

namespace mx
{
    namespace api
    {
        enum class WedgeType
        {
            unspecified,
            crescendo,
            diminuendo
        };

        struct WedgeStart
        {
            int numberLevel;
            WedgeType wedgeType;
            bool isSpreadSpecified;
            long double spread;
            LineData lineData;
            PositionData positionData;
            ColorData colorData;
            
            WedgeStart()
            : numberLevel{ -1 }
            , wedgeType{ WedgeType::unspecified }
            , isSpreadSpecified{ false }
            , spread{ 0.0 }
            , lineData{}
            , positionData{}
            , colorData{}
            {
                
            }
        };
        
        struct WedgeStop
        {
            int numberLevel;
            PositionData positionData;
            bool isSpreadSpecified;
            long double spread;
            
            WedgeStop()
            : numberLevel{ -1 }
            , isSpreadSpecified{ false }
            , spread{ 0.0 }
            , positionData{}
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( WedgeStart )
        MXAPI_EQUALS_FIRST_MEMBER( numberLevel )
        MXAPI_EQUALS_NEXT_MEMBER( wedgeType )
        MXAPI_EQUALS_NEXT_MEMBER( isSpreadSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( spread )
        MXAPI_EQUALS_NEXT_MEMBER( lineData )
        MXAPI_EQUALS_NEXT_MEMBER( positionData )
        MXAPI_EQUALS_LAST_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( WedgeStart );
        
        MXAPI_EQUALS_BEGIN( WedgeStop )
        MXAPI_EQUALS_FIRST_MEMBER( numberLevel )
        MXAPI_EQUALS_NEXT_MEMBER( positionData )
        MXAPI_EQUALS_NEXT_MEMBER( isSpreadSpecified )
        MXAPI_EQUALS_LAST_MEMBER( spread )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( WedgeStop );
    }
}
