// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/LineData.h"
#include "mx/api/PositionData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
#include <string>

// MusicXML Documentation for Bezier Attributes Group
// The bezier attribute group is used to indicate the curvature of slurs
// and ties, representing the control points for a cubic bezier curve.
// For ties, the bezier attribute group is used with the tied element.
//
// Normal slurs, S-shaped slurs, and ties need only two bezier points:
// one associated with the start of the slur or tie, the other with the
// stop. Complex slurs and slurs divided over system breaks can specify
// additional bezier data at slur elements with a continue type.
//
// The bezier-offset, bezier-x, and bezier-y attributes describe the
// outgoing bezier point for slurs and ties with a start type, and the
// incoming bezier point for slurs and ties with types of stop or continue.
// The attributes bezier-offset2, bezier-x2, and bezier-y2 are only valid
// with slurs of type continue, and describe the outgoing bezier point.
//
// The bezier-offset and bezier-offset2 attributes are measured in terms of
// musical divisions, like the offset element. These are the recommended
// attributes for specifying horizontal position. The other attributes are
// specified in tenths, relative to any position settings associated with
// the slur or tied element.

// MusicXML Documentation for Slur Element
// Slur types are empty. Most slurs are represented with two elements: one
// with a start type, and one with a stop type. Slurs can add more elements
// using a continue type. This is typically used to specify the formatting
// of cross-system slurs, or to specify the shape of very complex slurs.

namespace mx
{
namespace api
{
enum class CurveType
{
    unspecified,
    slur,
    tie
};

enum class CurveOrientation
{
    unspecified,
    overhand,
    underhand
};

struct CurvePoints
{
    // positionData represents a point along the curve.
    // either the endpoint in the case of starts and stops,
    // or the continuation points in the case of a 'continue'.
    // only the following PositionData fields are supported:
    // defaultX, defaultY, relativeX, relativeY, placement
    PositionData positionData;

    bool isBezierXSpecified;
    double bezierX;

    bool isBezierYSpecified;
    double bezierY;

    bool isBezierOffsetSpecified;
    int bezierOffset;

    // convenience, check if any of the available fields
    // are specified.
    inline bool isSpecified() const
    {
        return positionData.isDefaultXSpecified || positionData.isDefaultYSpecified || isBezierXSpecified ||
               isBezierYSpecified || isBezierOffsetSpecified;
    }

    CurvePoints()
        : positionData{}, isBezierXSpecified{false}, bezierX{0.0}, isBezierYSpecified{false}, bezierY{0.0},
          isBezierOffsetSpecified{false}, bezierOffset{0}
    {
    }
};

struct CurveStart
{
    CurveType curveType;
    SpannerNumber number;
    CurvePoints curvePoints;
    CurveOrientation curveOrientation;
    Placement placement;
    LineData lineData;
    bool isColorSpecified;
    ColorData colorData;

    // The id attribute of the element this curve end is written as -- <slur> for a slur,
    // <tied> for a tie (see ApiCommon.h).
    std::optional<std::string> id;

    CurveStart(CurveType inCurveType)
        : curveType{inCurveType}, number{}, curvePoints{}, curveOrientation{CurveOrientation::unspecified},
          placement{Placement::unspecified}, lineData{}, isColorSpecified{false}, colorData{}, id{}
    {
    }
};

struct CurveContinue
{
    CurveType curveType;
    SpannerNumber number;
    CurvePoints curvePoints;
    bool isBezierX2Specified;
    double bezierX2;
    bool isBezierY2Specified;
    double bezierY2;
    bool isBezierOffset2Specified;
    double bezierOffset2;

    // The id attribute of the element this curve end is written as -- <slur> for a slur,
    // <tied> for a tie (see ApiCommon.h).
    std::optional<std::string> id;

    CurveContinue(CurveType inCurveType)
        : curveType{inCurveType}, number{}, curvePoints{}, isBezierX2Specified{false}, bezierX2{0.0},
          isBezierY2Specified{false}, bezierY2{0.0}, isBezierOffset2Specified{false}, bezierOffset2{0.0}, id{}
    {
    }
};

struct CurveStop
{
    CurveType curveType;
    SpannerNumber number;
    CurvePoints curvePoints;

    // The id attribute of the element this curve end is written as -- <slur> for a slur,
    // <tied> for a tie (see ApiCommon.h).
    std::optional<std::string> id;

    CurveStop(CurveType inCurveType) : curveType{inCurveType}, number{}, curvePoints{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(CurvePoints)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isBezierXSpecified)
MXAPI_EQUALS_MEMBER(bezierX)
MXAPI_EQUALS_MEMBER(isBezierYSpecified)
MXAPI_EQUALS_MEMBER(bezierY)
MXAPI_EQUALS_MEMBER(isBezierOffsetSpecified)
MXAPI_EQUALS_MEMBER(bezierOffset)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(CurvePoints);

MXAPI_EQUALS_BEGIN(CurveStart)
MXAPI_EQUALS_MEMBER(curveType)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(curvePoints)
MXAPI_EQUALS_MEMBER(curveOrientation)
MXAPI_EQUALS_MEMBER(placement)
MXAPI_EQUALS_MEMBER(lineData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(CurveStart);

MXAPI_EQUALS_BEGIN(CurveContinue)
MXAPI_EQUALS_MEMBER(curveType)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(curvePoints)
MXAPI_EQUALS_MEMBER(isBezierX2Specified)
MXAPI_EQUALS_MEMBER(bezierX2)
MXAPI_EQUALS_MEMBER(isBezierY2Specified)
MXAPI_EQUALS_MEMBER(bezierY2)
MXAPI_EQUALS_MEMBER(isBezierOffset2Specified)
MXAPI_EQUALS_MEMBER(bezierOffset2)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(CurveContinue);

MXAPI_EQUALS_BEGIN(CurveStop)
MXAPI_EQUALS_MEMBER(curveType)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(curvePoints)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(CurveStop);
} // namespace api
} // namespace mx
