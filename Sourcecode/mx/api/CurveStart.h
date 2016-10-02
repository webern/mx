// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/CurveType.h"
#include "mx/api/CurvePoints.h"
#include "mx/api/LineData.h"
#include "mx/api/ColorData.h"
#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        struct CurveStart
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints curvePoints;
            CurveOrientation curveOrientation;
            Placement placement;
            LineData lineData;
            bool isColorSpecified;
            ColorData colorData;
            
            CurveStart( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , curvePoints{}
            , curveOrientation{ CurveOrientation::unspecified }
            , placement{ Placement::unspecified }
            , lineData{}
            , isColorSpecified{ false }
            , colorData{}
            {
                
            }
        };
    }
}
