// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/CurvePoints.h"

namespace mx
{
    namespace api
    {
        struct CurveEnd
        {
            CurveType curveType;
            int numberLevel;
            CurvePoints curvePoints;
            
            CurveEnd( CurveType inCurveType )
            : curveType{ inCurveType }
            , numberLevel{ -1 }
            , curvePoints{}
            {
                
            }
        };
    }
}
