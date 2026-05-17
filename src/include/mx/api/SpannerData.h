// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <string>

namespace mx
{
namespace api
{

struct SpannerStart
{
    int numberLevel;
    int tickTimePosition;
    PositionData positionData;
    PrintData printData;
    LineData lineData;

    SpannerStart() : numberLevel{-1}, tickTimePosition{0}, positionData{}, printData{}, lineData{}
    {
    }
};

struct SpannerStop
{
    int numberLevel;
    int tickTimePosition;
    PositionData positionData;
    LineData lineData;

    SpannerStop() : numberLevel{-1}, tickTimePosition{0}, positionData{}, lineData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SpannerStart)
MXAPI_EQUALS_MEMBER(numberLevel)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStart);

MXAPI_EQUALS_BEGIN(SpannerStop)
MXAPI_EQUALS_MEMBER(numberLevel)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStop);
} // namespace api
} // namespace mx
