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
// An accordion registration symbol, MusicXML's <accordion-registration> element: the circular
// three-section diagram telling the player which reed ranks to engage. high engages the high
// (4') section, drawn as a dot in the top section; middle engages the middle (8') section with
// 1 to 3 dots; low engages the low (16') section, drawn as a dot in the bottom section. An
// absent middle means no middle-section dots. A registration with nothing engaged is legal and
// draws the empty diagram.
class AccordionRegistrationData
{
  public:
    bool high;
    std::optional<int> middle;
    bool low;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;

    AccordionRegistrationData() : high{false}, middle{}, low{false}, positionData{}, fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(AccordionRegistrationData)
MXAPI_EQUALS_MEMBER(high)
MXAPI_EQUALS_MEMBER(middle)
MXAPI_EQUALS_MEMBER(low)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(AccordionRegistrationData);
} // namespace api
} // namespace mx
