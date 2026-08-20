// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// Whether a StringMuteData turns muting on or off.
enum class StringMuteType
{
    on,
    off
};

// A string mute change, MusicXML's <string-mute> element: the "mute on" or "mute off" symbol
// telling string players to apply or remove their mutes. positionData captures default/relative
// x-y plus the horizontal and vertical alignment; its placement member is unused here because
// <string-mute> has no placement attribute (placement lives on the parent <direction>).
class StringMuteData
{
  public:
    StringMuteType type;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<Id> id;

    StringMuteData() : type{StringMuteType::on}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(StringMuteData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(StringMuteData);
} // namespace api
} // namespace mx
