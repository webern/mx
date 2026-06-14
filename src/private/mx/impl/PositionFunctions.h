// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PositionData.h"
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
        outPositionData.horizontalAlignmnet = converter.convert(checkHalign<ATTRIBUTES_TYPE>(&inAttributes));
    }
    else
    {
        outPositionData.horizontalAlignmnet = api::HorizontalAlignment::unspecified;
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

    if (positionData.horizontalAlignmnet == api::HorizontalAlignment::unspecified)
    {
        lookForAndSetHasHalign(false, &outAttributes);
    }
    else
    {
        lookForAndSetHasHalign(true, &outAttributes);
        lookForAndSetHalign(converter.convert(positionData.horizontalAlignmnet), &outAttributes);
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
} // namespace impl
} // namespace mx
