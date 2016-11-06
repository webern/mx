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
            std::vector<CurveEnd> curveEnds;
            std::vector<TupletStart> tupletStarts;
            std::vector<TupletEnd> tupletEnds;
            //std::vector<SpannerData> spannerStarts;
            //std::vector<SpannerData> spannerEnds;
        };
        
        MXAPI_EQUALS_BEGIN( NoteAttachmentData )
        MXAPI_EQUALS_MEMBER( marks )
        MXAPI_EQUALS_MEMBER( curveStarts )
        MXAPI_EQUALS_MEMBER( curveContinuations )
        MXAPI_EQUALS_MEMBER( curveEnds )
        MXAPI_EQUALS_MEMBER( tupletStarts )
        MXAPI_EQUALS_MEMBER( tupletEnds )
        //MXAPI_EQUALS_MEMBER( spannerStarts )
        //MXAPI_EQUALS_MEMBER( spannerEnds )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( NoteAttachmentData );
    }
}
