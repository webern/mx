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

    // Whether a small circle (niente) is drawn at the start of the hairpin. The circle belongs at
    // the point, so set it here for a crescendo that grows out of nothing. A diminuendo's point is
    // at its end; set nienteCircle on its WedgeStop instead.
    Bool nienteCircle;

    // The <wedge> element's id attribute (see Id.h).
    std::optional<Id> id;

    WedgeStart()
        : number{}, wedgeType{WedgeType::unspecified}, isSpreadSpecified{false}, spread{0.0}, lineData{},
          positionData{}, isColorSpecified{false}, colorData{}, nienteCircle{Bool::unspecified}, id{}
    {
    }
};

struct WedgeStop
{
    SpannerNumber number;
    PositionData positionData;
    bool isSpreadSpecified;
    double spread;

    // Whether a small circle (niente) is drawn at the end of the hairpin. The circle belongs at the
    // point, so set it here for a diminuendo that fades away to nothing. A crescendo's point is at
    // its start; set nienteCircle on its WedgeStart instead.
    Bool nienteCircle;

    // The <wedge> element's id attribute (see Id.h).
    std::optional<Id> id;

    WedgeStop() : number{}, positionData{}, isSpreadSpecified{false}, spread{0.0}, nienteCircle{Bool::unspecified}, id{}
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
MXAPI_EQUALS_MEMBER(nienteCircle)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WedgeStart);

MXAPI_EQUALS_BEGIN(WedgeStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isSpreadSpecified)
MXAPI_EQUALS_MEMBER(spread)
MXAPI_EQUALS_MEMBER(nienteCircle)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WedgeStop);
} // namespace api
} // namespace mx
