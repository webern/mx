// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/LineData.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
namespace impl
{
// Note: no element has ever had a member spelled `lineStop` (the wire
// attribute is line-end, a required member where present), so the LineStop
// functions below are universal no-ops -- exactly as they were in the old
// SFINAE machinery. Preserved for call-site fidelity.
MX_OPTIONAL_HAS_FUNC(lineType, LineType);
MX_OPTIONAL_GET_VALUE_FUNC(lineType, LineType, core::LineType, core::LineType::solid());

MX_OPTIONAL_HAS_FUNC(lineStop, LineStop);
MX_OPTIONAL_GET_VALUE_FUNC(lineStop, LineStop, core::LineEnd, core::LineEnd::none());

MX_OPTIONAL_HAS_FUNC(dashLength, DashLength);
MX_OPTIONAL_GET_DECIMAL_FUNC(dashLength, DashLength, 0.0);

MX_OPTIONAL_HAS_FUNC(spaceLength, SpaceLength);
MX_OPTIONAL_GET_DECIMAL_FUNC(spaceLength, SpaceLength, 0.0);

MX_OPTIONAL_HAS_FUNC(endLength, StopLength);
MX_OPTIONAL_GET_DECIMAL_FUNC(endLength, StopLength, 0.0);

template <typename ATTRIBUTES_TYPE> api::LineData getLineData(const ATTRIBUTES_TYPE &inAttributes)
{
    api::LineData lineData;
    Converter converter;

    if (checkHasLineType<ATTRIBUTES_TYPE>(&inAttributes))
    {
        lineData.lineType = converter.convert(checkLineType<ATTRIBUTES_TYPE>(&inAttributes));
    }

    if (checkHasLineStop<ATTRIBUTES_TYPE>(&inAttributes))
    {
        lineData.lineHook = converter.convert(checkLineStop<ATTRIBUTES_TYPE>(&inAttributes));
    }

    if (checkHasDashLength(&inAttributes))
    {
        lineData.isDashLengthSpecified = true;
        lineData.dashLength = checkDashLength(&inAttributes);
    }

    if (checkHasSpaceLength(&inAttributes))
    {
        lineData.isSpaceLengthSpecified = true;
        lineData.spaceLength = checkSpaceLength(&inAttributes);
    }

    if (checkHasStopLength<ATTRIBUTES_TYPE>(&inAttributes))
    {
        lineData.isStopLengthSpecified = true;
        lineData.endLength = checkStopLength<ATTRIBUTES_TYPE>(&inAttributes);
    }

    return lineData;
}

MX_OPTIONAL_SET_HAS_FUNC(lineType, setLineType, LineType);
MX_OPTIONAL_SET_VALUE_FUNC(lineType, setLineType, LineType);

MX_OPTIONAL_SET_HAS_FUNC(lineStop, setLineStop, LineStop);
MX_OPTIONAL_SET_VALUE_FUNC(lineStop, setLineStop, LineStop);

MX_OPTIONAL_SET_HAS_FUNC(spaceLength, setSpaceLength, SpaceLength);
MX_OPTIONAL_SET_DECIMAL_FUNC(spaceLength, setSpaceLength, SpaceLength);

MX_OPTIONAL_SET_HAS_FUNC(dashLength, setDashLength, DashLength);
MX_OPTIONAL_SET_DECIMAL_FUNC(dashLength, setDashLength, DashLength);

MX_OPTIONAL_SET_HAS_FUNC(endLength, setEndLength, StopLength);
MX_OPTIONAL_SET_DECIMAL_FUNC(endLength, setEndLength, StopLength);

template <typename ATTRIBUTES_TYPE>
void setAttributesFromLineData(const api::LineData &inLineData, ATTRIBUTES_TYPE &outAttributes)
{
    Converter converter;
    if (lookForAndSetHasDashLength(inLineData.isDashLengthSpecified, &outAttributes))
    {
        lookForAndSetDashLength(inLineData.dashLength, &outAttributes);
    }

    if (lookForAndSetHasSpaceLength(inLineData.isSpaceLengthSpecified, &outAttributes))
    {
        lookForAndSetSpaceLength(inLineData.spaceLength, &outAttributes);
    }

    if (lookForAndSetHasLineStop(inLineData.lineHook != api::LineHook::unspecified, &outAttributes))
    {
        lookForAndSetLineStop(converter.convert(inLineData.lineHook), &outAttributes);
    }

    if (lookForAndSetHasLineType(inLineData.lineType != api::LineType::unspecified, &outAttributes))
    {
        lookForAndSetLineType(converter.convert(inLineData.lineType), &outAttributes);
    }

    if (lookForAndSetHasStopLength(inLineData.isStopLengthSpecified, &outAttributes))
    {
        lookForAndSetStopLength(inLineData.endLength, &outAttributes);
    }
}
} // namespace impl
} // namespace mx
