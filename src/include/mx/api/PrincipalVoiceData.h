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
// Whether a PrincipalVoiceData marks where the designation begins or where it ends.
enum class PrincipalVoiceType
{
    start,
    stop
};

// How a principal-voice marking is displayed: the Hauptstimme or Nebenstimme bracket symbols
// used in Schoenberg's and Berg's scores, a plain bracket, or no visible symbol at all.
enum class PrincipalVoiceSymbol
{
    hauptstimme,
    nebenstimme,
    plain,
    none
};

// A principal-voice designation, MusicXML's <principal-voice> element: marks a passage as the
// principal (Hauptstimme) or secondary (Nebenstimme) voice of the texture, a practice from the
// Second Viennese School. The marking spans from a start to the matching stop; the symbol
// chooses what is drawn at the start (the stop is drawn as the end of the bracket).
class PrincipalVoiceData
{
  public:
    PrincipalVoiceType type;
    PrincipalVoiceSymbol symbol;

    // Optional text content carried by the element, used for analysis rather than display.
    std::string text;

    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<Id> id;

    PrincipalVoiceData()
        : type{PrincipalVoiceType::start}, symbol{PrincipalVoiceSymbol::hauptstimme}, text{}, positionData{},
          fontData{}, color{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(PrincipalVoiceData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PrincipalVoiceData);
} // namespace api
} // namespace mx
