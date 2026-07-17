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
// Which staff-divide arrow a StaffDivideData shows: an arrow pointing down, up, or both.
enum class StaffDivideType
{
    down,
    up,
    upDown
};

// A staff-divide arrow, MusicXML's <staff-divide> element (MusicXML 3.1): the arrow symbol used
// in condensed conductor scores where one staff carries two players, showing whether the music
// continues on the staff below (down), above (up), or both (up-down). positionData captures
// default/relative x-y plus the horizontal and vertical alignment; its placement member is
// unused here because <staff-divide> has no placement attribute (placement lives on the parent
// <direction>).
class StaffDivideData
{
  public:
    StaffDivideType type;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    StaffDivideData() : type{StaffDivideType::down}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(StaffDivideData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(StaffDivideData);
} // namespace api
} // namespace mx
