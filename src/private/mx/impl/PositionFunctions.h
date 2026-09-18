// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PositionData.h"
#include "mx/core/generated/ValignImage.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
namespace impl
{
MX_OPTIONAL_HAS_FUNC(defaultX, DefaultX);
MX_OPTIONAL_GET_DECIMAL_FUNC(defaultX, DefaultX, 1.0);

MX_OPTIONAL_HAS_FUNC(defaultY, DefaultY);
MX_OPTIONAL_GET_DECIMAL_FUNC(defaultY, DefaultY, 1.0);

MX_OPTIONAL_HAS_FUNC(relativeX, RelativeX);
MX_OPTIONAL_GET_DECIMAL_FUNC(relativeX, RelativeX, 1.0);

MX_OPTIONAL_HAS_FUNC(relativeY, RelativeY);
MX_OPTIONAL_GET_DECIMAL_FUNC(relativeY, RelativeY, 1.0);

MX_OPTIONAL_HAS_FUNC(halign, Halign);
MX_OPTIONAL_GET_VALUE_FUNC(halign, Halign, core::LeftCenterRight, core::LeftCenterRight::left());

MX_OPTIONAL_HAS_FUNC(valign, Valign);
MX_OPTIONAL_GET_VALUE_FUNC(valign, Valign, core::Valign, core::Valign::baseline());

MX_OPTIONAL_HAS_FUNC(placement, Placement);
MX_OPTIONAL_GET_VALUE_FUNC(placement, Placement, core::AboveBelow, core::AboveBelow::above());

template <typename ATTRIBUTES_TYPE> api::PositionData getPositionData(const ATTRIBUTES_TYPE &inAttributes)
{
    api::PositionData outPositionData;

    if (checkHasDefaultX<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.isDefaultXSpecified = true;
        outPositionData.defaultX = checkDefaultX<ATTRIBUTES_TYPE>(&inAttributes);
    }

    if (checkHasDefaultY<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.isDefaultYSpecified = true;
        outPositionData.defaultY = checkDefaultY<ATTRIBUTES_TYPE>(&inAttributes);
    }

    if (checkHasRelativeX<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.isRelativeXSpecified = true;
        outPositionData.relativeX = checkRelativeX<ATTRIBUTES_TYPE>(&inAttributes);
    }

    if (checkHasRelativeY<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.isRelativeYSpecified = true;
        outPositionData.relativeY = checkRelativeY<ATTRIBUTES_TYPE>(&inAttributes);
    }

    impl::Converter converter;

    if (checkHasHalign<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.horizontalAlignment = converter.convert(checkHalign<ATTRIBUTES_TYPE>(&inAttributes));
    }
    else
    {
        outPositionData.horizontalAlignment = api::HorizontalAlignment::unspecified;
    }

    if (checkHasValign<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.verticalAlignment = converter.convert(checkValign<ATTRIBUTES_TYPE>(&inAttributes));
    }
    else
    {
        outPositionData.verticalAlignment = api::VerticalAlignment::unspecified;
    }

    if (checkHasPlacement<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPositionData.placement = converter.convert(checkPlacement<ATTRIBUTES_TYPE>(&inAttributes));
    }
    else
    {
        outPositionData.placement = api::Placement::unspecified;
    }

    return outPositionData;
}

MX_OPTIONAL_SET_HAS_FUNC(defaultX, setDefaultX, DefaultX);
MX_OPTIONAL_SET_DECIMAL_FUNC(defaultX, setDefaultX, DefaultX);

MX_OPTIONAL_SET_HAS_FUNC(defaultY, setDefaultY, DefaultY);
MX_OPTIONAL_SET_DECIMAL_FUNC(defaultY, setDefaultY, DefaultY);

MX_OPTIONAL_SET_HAS_FUNC(relativeX, setRelativeX, RelativeX);
MX_OPTIONAL_SET_DECIMAL_FUNC(relativeX, setRelativeX, RelativeX);

MX_OPTIONAL_SET_HAS_FUNC(relativeY, setRelativeY, RelativeY);
MX_OPTIONAL_SET_DECIMAL_FUNC(relativeY, setRelativeY, RelativeY);

MX_OPTIONAL_SET_HAS_FUNC(halign, setHalign, Halign);
MX_OPTIONAL_SET_VALUE_FUNC(halign, setHalign, Halign);

MX_OPTIONAL_SET_HAS_FUNC(valign, setValign, Valign);
MX_OPTIONAL_SET_VALUE_FUNC(valign, setValign, Valign);

MX_OPTIONAL_SET_HAS_FUNC(placement, setPlacement, Placement);
MX_OPTIONAL_SET_VALUE_FUNC(placement, setPlacement, Placement);

template <typename ATTRIBUTES_TYPE>
void setAttributesFromPositionData(const api::PositionData &positionData, ATTRIBUTES_TYPE &outAttributes)
{
    if (positionData.isDefaultXSpecified)
    {
        lookForAndSetHasDefaultX(true, &outAttributes);
        lookForAndSetDefaultX(positionData.defaultX, &outAttributes);
    }
    else
    {
        lookForAndSetHasDefaultX(false, &outAttributes);
        lookForAndSetDefaultX(0.0, &outAttributes);
    }

    if (positionData.isDefaultYSpecified)
    {
        lookForAndSetHasDefaultY(true, &outAttributes);
        lookForAndSetDefaultY(positionData.defaultY, &outAttributes);
    }
    else
    {
        lookForAndSetHasDefaultY(false, &outAttributes);
        lookForAndSetDefaultY(0.0, &outAttributes);
    }

    if (positionData.isRelativeXSpecified)
    {
        lookForAndSetHasRelativeX(true, &outAttributes);
        lookForAndSetRelativeX(positionData.relativeX, &outAttributes);
    }
    else
    {
        lookForAndSetHasRelativeX(false, &outAttributes);
        lookForAndSetRelativeX(0.0, &outAttributes);
    }

    if (positionData.isRelativeYSpecified)
    {
        lookForAndSetHasRelativeY(true, &outAttributes);
        lookForAndSetRelativeY(positionData.relativeY, &outAttributes);
    }
    else
    {
        lookForAndSetHasRelativeY(false, &outAttributes);
        lookForAndSetRelativeY(0.0, &outAttributes);
    }

    Converter converter;

    if (positionData.horizontalAlignment == api::HorizontalAlignment::unspecified)
    {
        lookForAndSetHasHalign(false, &outAttributes);
    }
    else
    {
        lookForAndSetHasHalign(true, &outAttributes);
        lookForAndSetHalign(converter.convert(positionData.horizontalAlignment), &outAttributes);
    }

    if (positionData.verticalAlignment == api::VerticalAlignment::unspecified)
    {
        lookForAndSetHasValign(false, &outAttributes);
    }
    else
    {
        lookForAndSetHasValign(true, &outAttributes);
        lookForAndSetValign(converter.convert(positionData.verticalAlignment), &outAttributes);
    }

    if (positionData.placement == api::Placement::unspecified)
    {
        lookForAndSetHasPlacement(false, &outAttributes);
    }
    else
    {
        lookForAndSetHasPlacement(true, &outAttributes);
        lookForAndSetPlacement(converter.convert(positionData.placement), &outAttributes);
    }
}

// <image> and <credit-image> write vertical alignment as valign-image (top, middle, bottom --
// no baseline), not valign. setAttributesFromPositionData above cannot write it: its presence
// setter engages the field with its natural zero (top) for any specified alignment, and its
// value setter is compiled out because a core::Valign does not convert to the
// std::optional<core::ValignImage> that valign-image elements expect, so the natural-zero top
// is left in place no matter what was authored. Call this afterward on the same element to
// write the correct value. Vertical alignments unspecified are omitted, and so is baseline:
// valign-image has no baseline value, so a baseline alignment cannot be represented and is
// omitted rather than mapped to something misleading. Callers that want to report the dropped
// baseline case should check for it before calling this.
template <typename ATTRIBUTES_TYPE>
void setImageValignFromVerticalAlignment(api::VerticalAlignment verticalAlignment, ATTRIBUTES_TYPE &outAttributes)
{
    switch (verticalAlignment)
    {
    case api::VerticalAlignment::top:
        outAttributes.setValign(core::ValignImage::top());
        break;
    case api::VerticalAlignment::middle:
        outAttributes.setValign(core::ValignImage::middle());
        break;
    case api::VerticalAlignment::bottom:
        outAttributes.setValign(core::ValignImage::bottom());
        break;
    case api::VerticalAlignment::baseline:
    case api::VerticalAlignment::unspecified:
    default:
        outAttributes.setValign(std::nullopt);
        break;
    }
}
} // namespace impl
} // namespace mx
