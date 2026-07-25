// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"
#include "mx/api/RehearsalData.h"

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

    // A shape drawn around the symbol. RehearsalEnclosure::unspecified draws no enclosure;
    // RehearsalEnclosure::none states explicitly that there is none.
    RehearsalEnclosure enclosure;

    SymbolData() : smufl{}, positionData{}, fontData{}, color{}, enclosure{RehearsalEnclosure::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(SymbolData)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SymbolData);
} // namespace api
} // namespace mx
