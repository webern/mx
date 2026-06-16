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
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PageTextData);
} // namespace api
} // namespace mx
