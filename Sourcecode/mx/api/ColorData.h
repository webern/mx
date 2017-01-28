// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        struct ColorData
        {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            bool isAlphaSpecified;
            uint8_t alpha;

            inline bool isBlack() { return ( red == 255 && green == 255 && blue == 255 ) && ( ( !isAlphaSpecified ) || ( alpha == 255 ) ); }
            ColorData()
            : red{ 255 }
            , green{ 255 }
            , blue{ 255 }
            , isAlphaSpecified{ false }
            , alpha{ 255 }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( ColorData )
        MXAPI_EQUALS_MEMBER( red )
        MXAPI_EQUALS_MEMBER( green )
        MXAPI_EQUALS_MEMBER( blue )
        MXAPI_EQUALS_MEMBER( isAlphaSpecified )
        MXAPI_EQUALS_MEMBER( alpha )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ColorData );
    }
}
