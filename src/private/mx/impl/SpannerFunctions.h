// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SpannerData.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/utility/Unused.h"

#include <optional>

namespace mx
{
namespace impl
{
MX_OPTIONAL_HAS_FUNC(number, Number);
MX_OPTIONAL_GET_INT_FUNC(number, Number, api::NUMBER_LEVEL_UNSPECIFIED);

template <typename ATTRIBUTES_TYPE> api::SpannerStart getSpannerStart(const ATTRIBUTES_TYPE &inAttributes)
{
    api::SpannerStart start;
    if (checkHasNumber(&inAttributes))
    {
        start.number = api::SpannerNumber(checkNumber(&inAttributes));
    }
    start.positionData = getPositionData(inAttributes);
    start.printData = getPrintData(inAttributes);
    start.lineData = getLineData(inAttributes);
    return start;
}

template <typename ATTRIBUTES_TYPE> api::SpannerStop getSpannerStop(const ATTRIBUTES_TYPE &inAttributes)
{
    api::SpannerStop stop;
    if (checkHasNumber(&inAttributes))
    {
        stop.number = api::SpannerNumber(checkNumber(&inAttributes));
    }
    stop.positionData = getPositionData(inAttributes);
    stop.lineData = getLineData(inAttributes);
    return stop;
}

MX_OPTIONAL_SET_HAS_FUNC(number, setNumber, Number);
MX_OPTIONAL_SET_INT_FUNC(number, setNumber, Number);

// The number written is the one the SpannerNumberResolver pass resolved for
// the object; these helpers never look at the SpannerNumber directly.
template <typename ATTRIBUTES_TYPE>
void setAttributesFromSpannerStart(const api::SpannerStart &start, ATTRIBUTES_TYPE &outAttributes,
                                   const std::optional<int> &inResolvedNumber)
{
    MX_UNUSED(start);
    if (inResolvedNumber.has_value())
    {
        lookForAndSetHasNumber(true, &outAttributes);
        lookForAndSetNumber(*inResolvedNumber, &outAttributes);
    }
    else
    {
        lookForAndSetHasNumber(false, &outAttributes);
    }
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromSpannerStop(const api::SpannerStop &stop, ATTRIBUTES_TYPE &outAttributes,
                                  const std::optional<int> &inResolvedNumber)
{
    if (inResolvedNumber.has_value())
    {
        lookForAndSetHasNumber(true, &outAttributes);
        lookForAndSetNumber(*inResolvedNumber, &outAttributes);
    }
    else
    {
        lookForAndSetHasNumber(false, &outAttributes);
    }

    setAttributesFromPositionData(stop.positionData, outAttributes);
    setAttributesFromLineData(stop.lineData, outAttributes);
}
} // namespace impl
} // namespace mx
