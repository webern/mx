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
// A harp damping mark, MusicXML's <damp> element: the player silences one or more ringing
// strings with the hand. positionData captures default/relative x-y plus the horizontal and
// vertical alignment; its placement member is unused here because <damp> has no placement
// attribute (placement lives on the parent <direction>).
class DampData
{
  public:
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    DampData() : positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(DampData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(DampData);

// A harp damping mark, MusicXML's <damp-all> element: the player silences all of the ringing
// strings at once, notated as an X inside a circle. Formatting works the same way as DampData.
class DampAllData
{
  public:
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    DampAllData() : positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(DampAllData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(DampAllData);
} // namespace api
} // namespace mx
