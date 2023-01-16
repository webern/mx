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
        class CodaData
        {
        public:
            PositionData positionData;
            bool isColorSpecified;
            ColorData colorData;

            CodaData()
            : positionData{}
            , isColorSpecified{ false }
            , colorData{}
            {
            }
        };
        
        MXAPI_EQUALS_BEGIN( CodaData )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( isColorSpecified )
        MXAPI_EQUALS_MEMBER( colorData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( CodaData );
    }
}
