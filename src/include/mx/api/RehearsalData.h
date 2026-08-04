// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"

namespace mx
{
namespace api
{
class RehearsalData
{
  public:
    std::string text;
    PositionData positionData;
    bool isColorSpecified;
    ColorData colorData;
    FontData fontData;

    // A shape drawn around the text. Enclosure::unspecified draws no enclosure; Enclosure::none
    // states explicitly that there is none.
    Enclosure enclosure;

    // The `justify` attribute of `<rehearsal>`: how the mark's lines sit relative to each other
    // when its text runs to more than one line. `unspecified` means the attribute is absent.
    // Distinct from the `halign` attribute, which is carried in `positionData.horizontalAlignment`
    // and says which edge of the text the position refers to; MusicXML defines both.
    HorizontalAlignment justify;

    RehearsalData()
        : text{}, positionData{}, isColorSpecified{false}, colorData{}, fontData{}, enclosure{Enclosure::unspecified},
          justify{HorizontalAlignment::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(RehearsalData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(RehearsalData);
} // namespace api
} // namespace mx
