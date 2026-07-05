// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SpannerData.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
namespace impl
{
MX_OPTIONAL_HAS_FUNC(number, Number);
MX_OPTIONAL_GET_INT_FUNC(number, Number, api::NUMBER_LEVEL_UNSPECIFIED);

template <typename ATTRIBUTES_TYPE> api::SpannerStart getSpannerStart(const ATTRIBUTES_TYPE &inAttributes)
{
    api::SpannerStart start;
    start.numberLevel = api::NUMBER_LEVEL_UNSPECIFIED;
    if (checkHasNumber(&inAttributes))
    {
        start.numberLevel = checkNumber(&inAttributes);
    }
    start.positionData = getPositionData(inAttributes);
    start.printData = getPrintData(inAttributes);
    start.lineData = getLineData(inAttributes);
    return start;
}

template <typename ATTRIBUTES_TYPE> api::SpannerStop getSpannerStop(const ATTRIBUTES_TYPE &inAttributes)
{
    api::SpannerStop stop;
    stop.numberLevel = api::NUMBER_LEVEL_UNSPECIFIED;
    if (checkHasNumber(&inAttributes))
    {
        stop.numberLevel = checkNumber(&inAttributes);
    }
    stop.positionData = getPositionData(inAttributes);
    stop.lineData = getLineData(inAttributes);
    return stop;
}

MX_OPTIONAL_SET_HAS_FUNC(number, setNumber, Number);
MX_OPTIONAL_SET_INT_FUNC(number, setNumber, Number);

template <typename ATTRIBUTES_TYPE>
void setAttributesFromSpannerStart(const api::SpannerStart &start, ATTRIBUTES_TYPE &outAttributes)
{
    if (start.numberLevel > 0)
    {
        lookForAndSetHasNumber(true, &outAttributes);
        lookForAndSetNumber(start.numberLevel, &outAttributes);
    }
    else
    {
        lookForAndSetHasNumber(false, &outAttributes);
        lookForAndSetNumber(1, &outAttributes);
    }
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromSpannerStop(const api::SpannerStop &stop, ATTRIBUTES_TYPE &outAttributes)
{
    if (stop.numberLevel > 0)
    {
        lookForAndSetHasNumber(true, &outAttributes);
        lookForAndSetNumber(stop.numberLevel, &outAttributes);
    }
    else
    {
        lookForAndSetHasNumber(false, &outAttributes);
        lookForAndSetNumber(1, &outAttributes);
    }

    setAttributesFromPositionData(stop.positionData, outAttributes);
    setAttributesFromLineData(stop.lineData, outAttributes);
}
} // namespace impl
} // namespace mx
