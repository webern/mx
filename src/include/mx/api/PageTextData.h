// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{

// distance values are in 'tenths' governed by the 'scaling' values

// One more stretch of text in a credit, drawn after the text before it. A credit is usually one
// stretch of text in one style. A title in italics followed by a catalogue number in upright type
// is two stretches in one credit. Each field holds only what this stretch states. Font, justify and
// enclosure left unspecified carry over from the stretch before it; position does not. See
// PageTextData::continuations.
class PageTextSegment
{
  public:
    std::string text;
    PositionData positionData;
    FontData fontData;
    HorizontalAlignment justify = HorizontalAlignment::unspecified;
    Enclosure enclosure = Enclosure::unspecified;
};

MXAPI_EQUALS_BEGIN(PageTextSegment)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PageTextSegment);

class PageTextData
{
  public:
    std::string text;
    int pageNumber;
    PositionData positionData;
    FontData fontData;
    std::string description; // e.g. 'composer', 'page number', 'title', etc., this is metadata which does not appear on
                             // the printed page

    // All `<credit-type>` values for this credit, in document order. A
    // `<credit>` may carry more than one credit-type; `description` mirrors
    // the first for source compatibility. When `text` is empty and
    // `creditTypes` is non-empty, this represents a metadata-only credit
    // (a `<credit>` with no `<credit-words>`).
    std::vector<std::string> creditTypes;

    // The `justify` attribute of `<credit-words>`. `unspecified` means the
    // attribute is absent. Distinct from the `halign` attribute, which is
    // carried in `positionData.horizontalAlignment`; MusicXML defines both
    // attributes on `<credit-words>`.
    HorizontalAlignment justify = HorizontalAlignment::unspecified;

    // A shape drawn around the text. Enclosure::unspecified draws no enclosure; Enclosure::none
    // states explicitly that there is none.
    Enclosure enclosure = Enclosure::unspecified;

    // The stretches of text that follow `text` in the same credit, in order. Empty for a credit
    // that is one stretch of text, which is the usual case. The fields above describe the first
    // stretch; each continuation has its own font, position, justify and enclosure.
    std::vector<PageTextSegment> continuations;
};

MXAPI_EQUALS_BEGIN(PageTextData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(pageNumber)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(description)
if (!areVectorsEqual(lhs.creditTypes, rhs.creditTypes))
{
    MX_SHOW_UNEQUAL("PageTextData", "creditTypes");
    return false;
}
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_MEMBER(enclosure)
MXAPI_EQUALS_MEMBER(continuations)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PageTextData);
} // namespace api
} // namespace mx
