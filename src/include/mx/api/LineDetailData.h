// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/LineData.h"

#include <optional>

namespace mx
{
namespace api
{
// How one line of a staff is drawn, MusicXML's <line-detail>. Use it to thicken, color, dash or
// hide a single staff line, for example to hide the middle line of a widely spaced percussion
// staff. A hidden line still counts when placing clefs and notes on the staff.
struct LineDetailData
{
    // Which staff line this describes, counting from the bottom line as 1.
    int line;

    // The thickness of the line, in tenths of a staff space. Leave it empty for the usual width.
    std::optional<double> width;

    // The color of the line. Leave it empty to draw the line in the same color as the rest of the
    // staff.
    std::optional<ColorData> color;

    // Whether the line is solid, dashed, dotted or wavy. Leave it unspecified for a solid line.
    LineType lineType;

    // Whether the line is drawn. `no` hides it. `unspecified` (the default) does not write
    // print-object, which leaves the line printed.
    Bool printObject;

    LineDetailData() : line{1}, width{}, color{}, lineType{LineType::unspecified}, printObject{Bool::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(LineDetailData)
MXAPI_EQUALS_MEMBER(line)
MXAPI_EQUALS_MEMBER(width)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(lineType)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LineDetailData);
} // namespace api
} // namespace mx
