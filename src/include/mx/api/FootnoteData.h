// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"

#include <string>

namespace mx
{
namespace api
{
// The MusicXML <footnote> element is a formatted-text: text content plus the position, font, and
// color attribute groups. Modeled to match WordsData (the api's other formatted-text surface).
class FootnoteData
{
  public:
    std::string text;
    PositionData positionData;
    FontData fontData;
    bool isColorSpecified;
    ColorData colorData;

    FootnoteData() : text{}, positionData{}, fontData{}, isColorSpecified{false}, colorData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(FootnoteData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FootnoteData);
} // namespace api
} // namespace mx
