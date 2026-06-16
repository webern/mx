// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{

// distance values are in 'tenths' governed by the 'scaling' values

/// Corresponds to a `<credit-image>` element. Models a graphical image
/// placed on a page of the score (e.g. a logo or a scanned title page).
class PageImageData
{
  public:
    /// The image file location (the required `source` attribute).
    std::string source;

    /// The MIME type of the image (the required `type` attribute), e.g.
    /// "image/png".
    std::string type;

    /// Image height in tenths. Negative means unspecified.
    double height;

    /// Image width in tenths. Negative means unspecified.
    double width;

    /// The page this credit-image appears on (the credit's `page`
    /// attribute). Values <= 0 mean unspecified.
    int pageNumber;

    /// default-x/default-y/relative-x/relative-y and horizontal alignment.
    /// (Vertical alignment uses a credit-image specific type and is not
    /// modeled here.)
    PositionData positionData;

    PageImageData() : source{}, type{}, height{-1.0}, width{-1.0}, pageNumber{0}, positionData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(PageImageData)
MXAPI_EQUALS_MEMBER(source)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(height)
MXAPI_EQUALS_MEMBER(width)
MXAPI_EQUALS_MEMBER(pageNumber)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PageImageData);
} // namespace api
} // namespace mx
