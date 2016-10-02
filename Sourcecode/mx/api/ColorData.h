// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

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
    }
}
