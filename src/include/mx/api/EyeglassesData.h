// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// The eyeglasses symbol, MusicXML's <eyeglasses> element: a cue common in commercial music
// telling the player to watch the conductor (or another player) closely at this spot.
// positionData captures default/relative x-y plus the horizontal and vertical alignment; its
// placement member is unused here because <eyeglasses> has no placement attribute (placement
// lives on the parent <direction>).
class EyeglassesData
{
  public:
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    EyeglassesData() : positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(EyeglassesData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EyeglassesData);
} // namespace api
} // namespace mx
