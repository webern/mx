// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

namespace mx
{
    namespace api
    {

        enum class BarlineType
        {
            unspecified,
            unsupported,
            none,
            normal,
            lightLight,
            lightHeavy,
            heavyLight
        };

        class BarlineData
        {
        public:
            int tickTimePosition;
            BarlineType barlineType;
            HorizontalAlignment location;
            
            BarlineData()
            : tickTimePosition{ 0 }
            , barlineType{ BarlineType::normal }
            , location{ HorizontalAlignment::right }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( BarlineData )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( barlineType )
        MXAPI_EQUALS_MEMBER( location )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( BarlineData );
    }
}
