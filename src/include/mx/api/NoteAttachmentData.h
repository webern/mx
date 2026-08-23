// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/CurveData.h"
#include "mx/api/GlissandoData.h"
#include "mx/api/MarkData.h"
#include "mx/api/TupletData.h"
#include "mx/api/WavyLineData.h"

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
    std::vector<GlissandoStart> glissandoStarts;
    std::vector<GlissandoStop> glissandoStops;
    std::vector<WavyLineStart> wavyLineStarts;
    std::vector<WavyLineContinue> wavyLineContinuations;
    std::vector<WavyLineStop> wavyLineStops;
};

MXAPI_EQUALS_BEGIN(NoteAttachmentData)
MXAPI_EQUALS_MEMBER(marks)
MXAPI_EQUALS_MEMBER(curveStarts)
MXAPI_EQUALS_MEMBER(curveContinuations)
MXAPI_EQUALS_MEMBER(curveStops)
MXAPI_EQUALS_MEMBER(tupletStarts)
MXAPI_EQUALS_MEMBER(tupletStops)
MXAPI_EQUALS_MEMBER(glissandoStarts)
MXAPI_EQUALS_MEMBER(glissandoStops)
MXAPI_EQUALS_MEMBER(wavyLineStarts)
MXAPI_EQUALS_MEMBER(wavyLineContinuations)
MXAPI_EQUALS_MEMBER(wavyLineStops)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NoteAttachmentData);
} // namespace api
} // namespace mx
