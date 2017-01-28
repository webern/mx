// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            , positionData{}
            , isSpreadSpecified{ false }
            , spread{ 0.0 }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( WedgeStart )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( wedgeType )
        MXAPI_EQUALS_MEMBER( isSpreadSpecified )
        MXAPI_EQUALS_MEMBER( spread )
        MXAPI_EQUALS_MEMBER( lineData )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( WedgeStart );
        
        MXAPI_EQUALS_BEGIN( WedgeStop )
        MXAPI_EQUALS_MEMBER( numberLevel )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( isSpreadSpecified )
        MXAPI_EQUALS_MEMBER( spread )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( WedgeStop );
    }
}
