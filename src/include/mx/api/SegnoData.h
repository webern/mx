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
// The MusicXML <segno> element carries the empty-print-object-style-align attribute group plus the
// smufl glyph-name and optional-unique-id. positionData captures default/relative x-y plus the
// horizontal and vertical alignment; its placement member is unused here because <segno> has no
// placement attribute (placement lives on the parent <direction>).
class SegnoData
{
  public:
    PositionData positionData;
    FontData fontData;
    bool isColorSpecified;
    ColorData colorData;
    bool isSmuflSpecified;
    std::string smufl;
    bool isIdSpecified;
    std::string id;

    SegnoData()
        : positionData{}, fontData{}, isColorSpecified{false}, colorData{}, isSmuflSpecified{false}, smufl{},
          isIdSpecified{false}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(SegnoData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(isSmuflSpecified)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(isIdSpecified)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(SegnoData);
} // namespace api
} // namespace mx
