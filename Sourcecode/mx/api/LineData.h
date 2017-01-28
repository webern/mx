// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        enum class LineType
        {
            unspecified,
            solid,
            dashed,
            dotted,
            wavy
        };

        enum class LineHook
        {
            unspecified,
            down,
            up,
            both,
            arrow,
            none
        };
        
        struct LineData
        {
            LineType lineType;
            LineHook lineHook;
            bool isStopLengthSpecified;
            long double endLength;
            bool isDashLengthSpecified;
            long double dashLength;
            bool isSpaceLengthSpecified;
            long double spaceLength;
            
            inline bool isSpecified() const
            {
                return lineType != LineType::unspecified ||
                isDashLengthSpecified ||
                isSpaceLengthSpecified;
            }
            
            LineData()
            : lineType{ LineType::unspecified }
            , lineHook{ LineHook::unspecified }
            , isStopLengthSpecified{ false }
            , endLength{ 0.0 }
            , isDashLengthSpecified{ false }
            , dashLength{ 0.0 }
            , isSpaceLengthSpecified{ false }
            , spaceLength{ 0.0 }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( LineData )
        MXAPI_EQUALS_MEMBER( lineType )
        MXAPI_EQUALS_MEMBER( lineHook )
        MXAPI_EQUALS_MEMBER( isStopLengthSpecified )
        MXAPI_EQUALS_MEMBER( endLength )
        MXAPI_EQUALS_MEMBER( isDashLengthSpecified )
        MXAPI_EQUALS_MEMBER( dashLength )
        MXAPI_EQUALS_MEMBER( isSpaceLengthSpecified )
        MXAPI_EQUALS_MEMBER( spaceLength )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LineData );
    }
}
