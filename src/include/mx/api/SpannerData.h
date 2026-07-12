// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/SpannerNumber.h"

#include <string>

namespace mx
{
namespace api
{

struct SpannerStart
{
    SpannerNumber number;
    int tickTimePosition;
    PositionData positionData;
    PrintData printData;
    LineData lineData;

    SpannerStart() : number{}, tickTimePosition{0}, positionData{}, printData{}, lineData{}
    {
    }
};

struct SpannerStop
{
    SpannerNumber number;
    int tickTimePosition;
    PositionData positionData;
    LineData lineData;

    SpannerStop() : number{}, tickTimePosition{0}, positionData{}, lineData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SpannerStart)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStart);

MXAPI_EQUALS_BEGIN(SpannerStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStop);
} // namespace api
} // namespace mx
