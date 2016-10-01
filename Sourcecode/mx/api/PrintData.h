// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/FontData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        struct Color
        {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            bool isAlphaSpecified;
            uint8_t alpha;

            inline bool isBlack() { return ( red == 255 && green == 255 && blue == 255 ) && ( ( !isAlphaSpecified ) || ( alpha == 255 ) ); }
            Color()
            : red{ 255 }
            , green{ 255 }
            , blue{ 255 }
            , isAlphaSpecified{ false }
            , alpha{ 255 }
            {
                
            }

        };

        struct PrintData
        {
            Bool printObject;
            FontData fontData;
            Color color;

            PrintData()
            : printObject{ Bool::unspecified }
            , fontData{}
            , color{}
            {
                
            }
        };
    }
}
