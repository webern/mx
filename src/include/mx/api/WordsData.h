// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"
#include "mx/api/RehearsalData.h"

namespace mx
{
namespace api
{
// A span of direction text, MusicXML's <words> element: performance words such as "dolce" or
// "rit." drawn at the direction's position. A words item lives in a run that may also hold
// SMuFL symbols -- see WordsChoice and DirectionChoice's wordsRun alternative.
class WordsData
{
  public:
    std::string text;
    PositionData positionData;
    FontData fontData;
    bool isColorSpecified;
    ColorData colorData;

    // A shape drawn around the text. RehearsalEnclosure::unspecified draws no enclosure;
    // RehearsalEnclosure::none states explicitly that there is none.
    RehearsalEnclosure enclosure;

    WordsData()
        : text{}, positionData{}, fontData{}, isColorSpecified{false}, colorData{},
          enclosure{RehearsalEnclosure::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(WordsData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WordsData);
} // namespace api
} // namespace mx
