// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// A graphic placed in the music, MusicXML's <image> direction: source locates the graphic file
// (a URL or file reference) and type gives its MIME type, e.g. "image/png". height and width,
// in tenths, scale the image; give one alone to scale proportionally. positionData captures
// default/relative x-y plus the horizontal and vertical alignment of the image relative to its
// position point; an image's vertical alignment has no baseline variant, so a baseline value
// here is not written. positionData's placement member is unused because <image> has no
// placement attribute (placement lives on the parent <direction>).
class ImageData
{
  public:
    std::string source;
    std::string type;
    std::optional<double> height;
    std::optional<double> width;
    PositionData positionData;
    std::optional<Id> id;

    ImageData() : source{}, type{}, height{}, width{}, positionData{}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(ImageData)
MXAPI_EQUALS_MEMBER(source)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(height)
MXAPI_EQUALS_MEMBER(width)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ImageData);
} // namespace api
} // namespace mx
