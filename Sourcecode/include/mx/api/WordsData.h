// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"

namespace mx
{
    namespace api
    {
        class WordsData
        {
        public:
            std::string text;
            PositionData positionData;
            FontData fontData;
            bool isColorSpecified;
            ColorData colorData;

            WordsData()
            : text{}
            , positionData{}
            , fontData{}
            , isColorSpecified{ false }
            , colorData{}
            {

            }
        };
        
        MXAPI_EQUALS_BEGIN( WordsData )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( fontData )
        MXAPI_EQUALS_MEMBER( isColorSpecified )
        MXAPI_EQUALS_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( WordsData );
    }
}
