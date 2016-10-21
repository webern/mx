// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/FontData.h"
#include "mx/api/ColorData.h"

namespace mx
{
    namespace api
    {
        struct PrintData
        {
            Bool printObject;
            FontData fontData;
            bool isColorSpecified;
            ColorData color;

            PrintData()
            : printObject{ Bool::unspecified }
            , fontData{}
            , isColorSpecified{ false }
            , color{}
            {
                
            }
        };
        
        MXAPI_EQUALS_BEGIN( PrintData )
        MXAPI_EQUALS_FIRST_MEMBER( printObject )
        MXAPI_EQUALS_NEXT_MEMBER( fontData )
        MXAPI_EQUALS_NEXT_MEMBER( isColorSpecified )
        MXAPI_EQUALS_LAST_MEMBER( color )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PrintData );
    }
}
