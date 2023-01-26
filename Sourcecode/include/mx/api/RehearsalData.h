// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/PositionData.h"

namespace mx
{
    namespace api
    {
        class RehearsalData
        {
        public:
            std::string text;
            PositionData positionData;
            bool isColorSpecified;
            ColorData colorData;
            FontData fontData;
            
            // Additional data about enclosing shape is available
            // but not supported at this time.
            
            RehearsalData()
            : text{}
            , positionData{}
            , isColorSpecified{ false }
            , colorData{}
            , fontData{}
            {
            }
        };
        
        MXAPI_EQUALS_BEGIN( RehearsalData )
        MXAPI_EQUALS_MEMBER( text )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( isColorSpecified )
        MXAPI_EQUALS_MEMBER( colorData )
        MXAPI_EQUALS_MEMBER( fontData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( RehearsalData );
    }
}
