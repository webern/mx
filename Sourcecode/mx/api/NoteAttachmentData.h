// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/api/CurveData.h"
#include "mx/api/TupletData.h"

namespace mx
{
    namespace api
    {
        class NoteAttachmentData
        {
        public:
            std::vector<MarkData> marks;
            std::vector<CurveStart> curveStarts;
            std::vector<CurveContinue> curveContinuations;
            std::vector<CurveStop> curveStops;
            std::vector<TupletStart> tupletStarts;
            std::vector<TupletStop> tupletStops;
            //std::vector<SpannerData> spannerStarts;
            //std::vector<SpannerData> spannerStops;
        };
        
        MXAPI_EQUALS_BEGIN( NoteAttachmentData )
        MXAPI_EQUALS_MEMBER( marks )
        MXAPI_EQUALS_MEMBER( curveStarts )
        MXAPI_EQUALS_MEMBER( curveContinuations )
        MXAPI_EQUALS_MEMBER( curveStops )
        MXAPI_EQUALS_MEMBER( tupletStarts )
        MXAPI_EQUALS_MEMBER( tupletStops )
        //MXAPI_EQUALS_MEMBER( spannerStarts )
        //MXAPI_EQUALS_MEMBER( spannerStops )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( NoteAttachmentData );
    }
}
