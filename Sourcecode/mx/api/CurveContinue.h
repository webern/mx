// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/CurvePoints.h"

namespace mx
{
    namespace api
    {
        struct CurveContinue
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints incomingCurvePoints;
            CurvePoints outgoingCurvePoints;
            
            CurveContinue( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , incomingCurvePoints{}
            , outgoingCurvePoints{}
            {
                
            }
        };
    }
}
