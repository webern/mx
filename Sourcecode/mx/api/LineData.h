// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

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
            LineHook lineEnd;
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
            , lineEnd{ LineHook::unspecified }
            , isDashLengthSpecified{ false }
            , dashLength{ 0.0 }
            , isSpaceLengthSpecified{ false }
            , spaceLength{ 0.0 }
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( LineData )
        MXAPI_EQUALS_FIRST_MEMBER( lineType )
        MXAPI_EQUALS_NEXT_MEMBER( lineEnd )
        MXAPI_EQUALS_NEXT_MEMBER( isDashLengthSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( dashLength )
        MXAPI_EQUALS_NEXT_MEMBER( isSpaceLengthSpecified )
        MXAPI_EQUALS_LAST_MEMBER( spaceLength )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LineData );
    }
}
