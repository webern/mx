// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/api/CurveStart.h"
#include "mx/api/CurveContinue.h"
#include "mx/api/CurveEnd.h"
#include "mx/api/TupletData.h"
#include "mx/api/SpannerData.h"

#include <vector>


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
            std::vector<SpannerData> spannerStarts;
            std::vector<SpannerData> spannerEnds;
        };
    }
}
