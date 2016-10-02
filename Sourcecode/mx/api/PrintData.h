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
    }
}
