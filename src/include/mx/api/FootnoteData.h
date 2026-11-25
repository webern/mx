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
// The MusicXML <footnote> element: editorial commentary about a symbol, printed as a footnote in
// the score, such as "m. 12: slur absent in the autograph". A footnote explains in prose, while a
// level (see LevelData) classifies the symbol and says how to draw it. One symbol may carry both.
struct FootnoteData
{
    // The words of the footnote.
    std::string text;

    // Where the footnote text is drawn, in tenths of a staff space, and how it aligns against that
    // point. placement is ignored. Leave this alone to let the application place it.
    PositionData positionData;

    // The font of the footnote text.
    FontData fontData;

    // The color of the footnote text. Leave it empty to draw it in the score's usual color.
    std::optional<ColorData> color;

    // How the lines of the footnote line up with each other when it spans more than one line.
    // Leave it unspecified to let the application decide.
    HorizontalAlignment justify = HorizontalAlignment::unspecified;

    // A shape drawn around the footnote text. Enclosure::unspecified does not draw an enclosure;
    // Enclosure::none states explicitly that there is no enclosure.
    Enclosure enclosure = Enclosure::unspecified;

    // Whether the spaces at the start and end of text are part of it (see XmlSpace).
    XmlSpace xmlSpace = XmlSpace::unspecified;
};

MXAPI_EQUALS_BEGIN(FootnoteData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_MEMBER(xmlSpace)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(FootnoteData);
} // namespace api
} // namespace mx
