// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PrintData.h"
#include "mx/core/generated/Color.h"
#include "mx/core/generated/YesNo.h"
#include "mx/impl/Converter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
namespace impl
{
MX_OPTIONAL_HAS_FUNC(printObject, PrintObject);
MX_OPTIONAL_GET_VALUE_FUNC(printObject, PrintObject, core::YesNo, core::YesNo::yes());

MX_OPTIONAL_HAS_FUNC(color, Color);
MX_OPTIONAL_GET_VALUE_FUNC(color, Color, core::Color, core::Color{});

template <typename ATTRIBUTES_TYPE> api::Bool getPrintObject(const ATTRIBUTES_TYPE &inAttributes)
{
    const ATTRIBUTES_TYPE *const ptr = &inAttributes;
    if (!checkHasPrintObject<ATTRIBUTES_TYPE>(ptr))
    {
        return api::Bool::unspecified;
    }
    Converter converter;
    return converter.convert(checkPrintObject<ATTRIBUTES_TYPE>(&inAttributes));
}

template <typename ATTRIBUTES_TYPE> api::ColorData getColor(const ATTRIBUTES_TYPE &inAttributes)
{
    if (!checkHasColor<ATTRIBUTES_TYPE>(&inAttributes))
    {
        return api::ColorData{};
    }
    const auto coreColor = checkColor<ATTRIBUTES_TYPE>(&inAttributes);
    api::ColorData outApiColor;
    outApiColor.red = static_cast<uint8_t>(coreColor.red());
    outApiColor.green = static_cast<uint8_t>(coreColor.green());
    outApiColor.blue = static_cast<uint8_t>(coreColor.blue());
    outApiColor.isAlphaSpecified = coreColor.alpha().has_value();

    if (outApiColor.isAlphaSpecified)
    {
        outApiColor.alpha = static_cast<uint8_t>(*coreColor.alpha());
    }
    else
    {
        outApiColor.alpha = 255;
    }
    return outApiColor;
}

template <typename ATTRIBUTES_TYPE> api::PrintData getPrintData(const ATTRIBUTES_TYPE &inAttributes)
{
    api::PrintData outPrintData;
    const ATTRIBUTES_TYPE *const ptr = &inAttributes;
    outPrintData.printObject = getPrintObject<ATTRIBUTES_TYPE>(inAttributes);
    outPrintData.fontData = getFontData<ATTRIBUTES_TYPE>(inAttributes);
    if (checkHasColor<ATTRIBUTES_TYPE>(ptr))
    {
        outPrintData.isColorSpecified = true;
        outPrintData.color = getColor(inAttributes);
    }
    else
    {
        outPrintData.isColorSpecified = false;
    }
    return outPrintData;
}

inline core::Color createCoreColor(const api::ColorData &inColor)
{
    if (inColor.isAlphaSpecified)
    {
        return core::Color{inColor.alpha, inColor.red, inColor.green, inColor.blue};
    }
    return core::Color{inColor.red, inColor.green, inColor.blue};
}

MX_OPTIONAL_SET_HAS_FUNC(printObject, setPrintObject, PrintObject);
MX_OPTIONAL_SET_VALUE_FUNC(printObject, setPrintObject, PrintObject);

MX_OPTIONAL_SET_HAS_FUNC(color, setColor, Color);
MX_OPTIONAL_SET_VALUE_FUNC(color, setColor, Color);

template <typename ATTRIBUTES_TYPE> void setPrintObject(const api::Bool &inPrintObject, ATTRIBUTES_TYPE &outAttributes)
{
    if (inPrintObject == api::Bool::unspecified)
    {
        lookForAndSetHasPrintObject(false, &outAttributes);
        return;
    }

    if (lookForAndSetHasPrintObject(true, &outAttributes))
    {
        Converter converter;
        lookForAndSetPrintObject(converter.convert(inPrintObject), &outAttributes);
    }
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromColorData(const api::ColorData &inColorData, ATTRIBUTES_TYPE &outAttributes)
{
    const auto valueToSet = createCoreColor(inColorData);
    // Ensure the optional color field is initialized before setting its value.
    lookForAndSetHasColor(true, &outAttributes);
    lookForAndSetColor(valueToSet, &outAttributes);
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromPrintData(const api::PrintData &inPrintData, ATTRIBUTES_TYPE &outAttributes)
{
    setPrintObject(inPrintData.printObject, outAttributes);

    if (inPrintData.isColorSpecified)
    {
        lookForAndSetHasColor(true, &outAttributes);
        setAttributesFromColorData(inPrintData.color, outAttributes);
    }
    else
    {
        lookForAndSetHasColor(false, &outAttributes);
        lookForAndSetColor(core::Color{}, &outAttributes);
    }

    setAttributesFromFontData(inPrintData.fontData, outAttributes);
}
} // namespace impl
} // namespace mx
