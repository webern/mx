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
// A direction that has no dedicated MusicXML element, carried by the <other-direction>
// catch-all. The text is the direction's content; when the direction is best shown as a
// standard SMuFL glyph, smufl names that glyph (a canonical SMuFL glyph name) and the text
// serves as a fallback description. Set printObject to no for a direction that affects
// playback or analysis but should not be drawn.
class OtherDirectionData
{
  public:
    std::string text;
    Bool printObject;
    std::optional<std::string> smufl;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    OtherDirectionData() : text{}, printObject{Bool::unspecified}, smufl{}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(OtherDirectionData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherDirectionData);
} // namespace api
} // namespace mx
