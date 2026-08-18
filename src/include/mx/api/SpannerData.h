// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
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

    // The id attribute of the element this spanner end is written as -- <bracket>,
    // <dashes>, or <octave-shift> (see ApiCommon.h).
    std::optional<std::string> id;

    SpannerStart() : number{}, tickTimePosition{0}, positionData{}, printData{}, lineData{}, id{}
    {
    }
};

struct SpannerStop
{
    SpannerNumber number;
    int tickTimePosition;
    PositionData positionData;
    LineData lineData;

    // The id attribute of the element this spanner end is written as -- <bracket>,
    // <dashes>, or <octave-shift> (see ApiCommon.h).
    std::optional<std::string> id;

    SpannerStop() : number{}, tickTimePosition{0}, positionData{}, lineData{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SpannerStart)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStart);

MXAPI_EQUALS_BEGIN(SpannerStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SpannerStop);
} // namespace api
} // namespace mx
