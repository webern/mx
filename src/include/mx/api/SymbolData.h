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
// A musical symbol appearing inside a run of direction text, MusicXML's <symbol> element: a
// single glyph named by its canonical SMuFL glyph name, e.g. "arrowBlackUp" or "noteheadBlack".
// Use it for an occasional glyph interspersed with words ("gliss." followed by an arrow) -- see
// WordsChoice. Notation that has dedicated semantic markup belongs in its own api type instead:
// a dynamic is a MarkData, a metronome mark is a TempoData, a segno is a SegnoData.
class SymbolData
{
  public:
    // The canonical SMuFL glyph name of the symbol to draw.
    std::string smufl;

    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;

    // A shape drawn around the symbol. Enclosure::unspecified draws no enclosure; Enclosure::none
    // states explicitly that there is none.
    Enclosure enclosure;

    // The `justify` attribute of `<symbol>`. `unspecified` means the attribute is absent. Distinct
    // from the `halign` attribute, which is carried in `positionData.horizontalAlignment` and says
    // which edge of the glyph the position refers to; MusicXML defines both on `<symbol>`, whose
    // symbol-formatting attributes mirror the text-formatting ones on `<words>`.
    HorizontalAlignment justify;

    // The <symbol> element's id attribute (see ApiCommon.h).
    std::optional<std::string> id;

    SymbolData()
        : smufl{}, positionData{}, fontData{}, color{}, enclosure{Enclosure::unspecified},
          justify{HorizontalAlignment::unspecified}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SymbolData)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SymbolData);
} // namespace api
} // namespace mx
