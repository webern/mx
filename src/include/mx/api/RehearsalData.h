// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/PositionData.h"

namespace mx
{
namespace api
{
enum class RehearsalEnclosure
{
    rectangle,
    square,
    oval,
    circle,
    bracket,
    triangle,
    diamond,
    none
};

class RehearsalData
{
  public:
    std::string text;
    PositionData positionData;
    bool isColorSpecified;
    ColorData colorData;
    FontData fontData;
    RehearsalEnclosure enclosure;

    RehearsalData()
        : text{}, positionData{}, isColorSpecified{false}, colorData{}, fontData{},
          enclosure{RehearsalEnclosure::rectangle}
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
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(RehearsalData);
} // namespace api
} // namespace mx
