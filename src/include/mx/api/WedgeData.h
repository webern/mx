// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/Id.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
enum class WedgeType
{
    unspecified,
    crescendo,
    diminuendo
};

struct WedgeStart
{
    SpannerNumber number;
    WedgeType wedgeType;
    bool isSpreadSpecified;
    double spread;
    LineData lineData;
    PositionData positionData;
    bool isColorSpecified;
    ColorData colorData;

    // Whether the point of the hairpin is drawn as a small circle (niente): a crescendo that grows
    // out of nothing, or a diminuendo that fades away to nothing. Set it here on the start for
    // either direction. MusicXML puts it on the start of a crescendo but on the stop of a
    // diminuendo, and mx writes it in the right place for you. Leave it unspecified for a plain
    // point.
    Bool niente;

    // The <wedge> element's id attribute (see Id.h).
    std::optional<Id> id;

    WedgeStart()
        : number{}, wedgeType{WedgeType::unspecified}, isSpreadSpecified{false}, spread{0.0}, lineData{},
          positionData{}, isColorSpecified{false}, colorData{}, niente{Bool::unspecified}, id{}
    {
    }
};

struct WedgeStop
{
    SpannerNumber number;
    PositionData positionData;
    bool isSpreadSpecified;
    double spread;

    // The <wedge> element's id attribute (see Id.h).
    std::optional<Id> id;

    WedgeStop() : number{}, positionData{}, isSpreadSpecified{false}, spread{0.0}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(WedgeStart)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(wedgeType)
MXAPI_EQUALS_MEMBER(isSpreadSpecified)
MXAPI_EQUALS_MEMBER(spread)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(niente)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WedgeStart);

MXAPI_EQUALS_BEGIN(WedgeStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isSpreadSpecified)
MXAPI_EQUALS_MEMBER(spread)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WedgeStop);
} // namespace api
} // namespace mx
