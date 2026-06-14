// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/FontData.h"
#include "mx/core/Decimal.h"
#include "mx/core/generated/FontFamily.h"
#include "mx/core/generated/FontSize.h"
#include "mx/core/generated/FontStyle.h"
#include "mx/core/generated/FontWeight.h"
#include "mx/core/generated/NumberOfLines.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
namespace impl
{
MX_OPTIONAL_HAS_FUNC(fontFamily, FontFamily);
MX_OPTIONAL_GET_VALUE_FUNC(fontFamily, FontFamily, core::FontFamily, core::FontFamily{});

MX_OPTIONAL_HAS_FUNC(fontStyle, FontStyle);
MX_OPTIONAL_GET_VALUE_FUNC(fontStyle, FontStyle, core::FontStyle, core::FontStyle{});

MX_OPTIONAL_HAS_FUNC(fontSize, FontSize);
MX_OPTIONAL_GET_VALUE_FUNC(fontSize, FontSize, core::FontSize,
                           core::FontSize::cssFontSize(core::CSSFontSize::medium()));

MX_OPTIONAL_HAS_FUNC(fontWeight, FontWeight);
MX_OPTIONAL_GET_VALUE_FUNC(fontWeight, FontWeight, core::FontWeight, core::FontWeight::normal());

MX_OPTIONAL_HAS_FUNC(lineThrough, LineThrough);
MX_OPTIONAL_GET_INT_FUNC(lineThrough, LineThrough, 0);

MX_OPTIONAL_HAS_FUNC(overline, Overline);
MX_OPTIONAL_GET_INT_FUNC(overline, Overline, 0);

MX_OPTIONAL_HAS_FUNC(underline, Underline);
MX_OPTIONAL_GET_INT_FUNC(underline, Underline, 0);

template <typename ATTRIBUTES_TYPE> std::vector<std::string> getFontFamily(const ATTRIBUTES_TYPE &inAttributes)
{
    if (!checkHasFontFamily<ATTRIBUTES_TYPE>(&inAttributes))
    {
        return std::vector<std::string>{};
    }

    const auto fontFamily = checkFontFamily<ATTRIBUTES_TYPE>(&inAttributes);
    std::vector<std::string> outStrings;

    for (const auto &item : fontFamily.items())
    {
        outStrings.emplace_back(item);
    }
    return outStrings;
}

template <typename ATTRIBUTES_TYPE> api::FontStyle getFontStyle(const ATTRIBUTES_TYPE &inAttributes)
{
    if (!checkHasFontStyle<ATTRIBUTES_TYPE>(&inAttributes))
    {
        return api::FontStyle::unspecified;
    }
    Converter converter;
    return converter.convert(checkFontStyle<ATTRIBUTES_TYPE>(&inAttributes));
}

template <typename ATTRIBUTES_TYPE> api::FontWeight getFontWeight(const ATTRIBUTES_TYPE &inAttributes)
{
    if (!checkHasFontWeight<ATTRIBUTES_TYPE>(&inAttributes))
    {
        return api::FontWeight::unspecified;
    }

    Converter converter;
    return converter.convert(checkFontWeight<ATTRIBUTES_TYPE>(&inAttributes));
}

template <typename ATTRIBUTES_TYPE>
api::FontSizeType getFontSize(const ATTRIBUTES_TYPE &inAttributes, double &outPointSize, api::CssSize &outCssSize)
{
    if (!checkHasFontSize<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outPointSize = -1.0;
        outCssSize = api::CssSize::unspecified;
        return api::FontSizeType::unspecified;
    }

    const auto coreFontSize = checkFontSize<ATTRIBUTES_TYPE>(&inAttributes);
    Converter converter;

    if (coreFontSize.isCSSFontSize())
    {
        outPointSize = -1.0;
        outCssSize = converter.convert(coreFontSize.asCSSFontSize());
        return api::FontSizeType::css;
    }
    else
    {
        outPointSize = coreFontSize.asDecimal().value();
        outCssSize = api::CssSize::unspecified;
        return api::FontSizeType::point;
    }
}

template <typename ATTRIBUTES_TYPE> api::FontData getFontData(const ATTRIBUTES_TYPE &inAttributes)
{
    api::FontData outFontData;
    outFontData.fontFamily = getFontFamily(inAttributes);
    outFontData.style = getFontStyle(inAttributes);
    outFontData.weight = getFontWeight(inAttributes);
    outFontData.sizeType = getFontSize(inAttributes, outFontData.sizePoint, outFontData.sizeCss);

    if (checkHasLineThrough<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outFontData.lineThrough = checkLineThrough<ATTRIBUTES_TYPE>(&inAttributes);
    }

    if (checkHasOverline<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outFontData.overline = checkOverline<ATTRIBUTES_TYPE>(&inAttributes);
    }

    if (checkHasUnderline<ATTRIBUTES_TYPE>(&inAttributes))
    {
        outFontData.underline = checkUnderline<ATTRIBUTES_TYPE>(&inAttributes);
    }

    return outFontData;
}

MX_OPTIONAL_SET_HAS_FUNC(fontFamily, setFontFamily, FontFamily);
MX_OPTIONAL_SET_VALUE_FUNC(fontFamily, setFontFamily, FontFamily);

MX_OPTIONAL_SET_HAS_FUNC(fontStyle, setFontStyle, FontStyle);
MX_OPTIONAL_SET_VALUE_FUNC(fontStyle, setFontStyle, FontStyle);

MX_OPTIONAL_SET_HAS_FUNC(fontSize, setFontSize, FontSize);
MX_OPTIONAL_SET_VALUE_FUNC(fontSize, setFontSize, FontSize);

MX_OPTIONAL_SET_HAS_FUNC(fontWeight, setFontWeight, FontWeight);
MX_OPTIONAL_SET_VALUE_FUNC(fontWeight, setFontWeight, FontWeight);

MX_OPTIONAL_SET_HAS_FUNC(lineThrough, setLineThrough, LineThrough);
MX_OPTIONAL_SET_INT_FUNC(lineThrough, setLineThrough, LineThrough);

MX_OPTIONAL_SET_HAS_FUNC(overline, setOverline, Overline);
MX_OPTIONAL_SET_INT_FUNC(overline, setOverline, Overline);

MX_OPTIONAL_SET_HAS_FUNC(underline, setUnderline, Underline);
MX_OPTIONAL_SET_INT_FUNC(underline, setUnderline, Underline);

template <typename ATTRIBUTES_TYPE>
void setAttributesFromFontFamily(const std::vector<std::string> &fontFamilyData, ATTRIBUTES_TYPE &outAttributes)
{
    if (fontFamilyData.empty())
    {
        lookForAndSetHasFontFamily(false, &outAttributes);
        return;
    }

    core::FontFamily csv{fontFamilyData};
    lookForAndSetHasFontFamily(true, &outAttributes);
    lookForAndSetFontFamily(csv, &outAttributes);
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromFontStyle(api::FontStyle value, ATTRIBUTES_TYPE &outAttributes)
{
    Converter converter;
    if (value == api::FontStyle::unspecified)
    {
        lookForAndSetHasFontStyle(false, &outAttributes);
        return;
    }
    lookForAndSetHasFontStyle(true, &outAttributes);
    lookForAndSetFontStyle(converter.convert(value), &outAttributes);
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromFontWeight(api::FontWeight value, ATTRIBUTES_TYPE &outAttributes)
{
    Converter converter;
    if (value == api::FontWeight::unspecified)
    {
        lookForAndSetHasFontWeight(false, &outAttributes);
        return;
    }
    lookForAndSetHasFontWeight(true, &outAttributes);
    lookForAndSetFontWeight(converter.convert(value), &outAttributes);
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromFontSize(const api::FontData &value, ATTRIBUTES_TYPE &outAttributes)
{
    Converter converter;

    if (value.sizeType == api::FontSizeType::unspecified)
    {
        lookForAndSetHasFontSize(false, &outAttributes);
        return;
    }

    lookForAndSetHasFontSize(true, &outAttributes);
    core::FontSize fontSize;

    if (value.sizeType == api::FontSizeType::css)
    {
        fontSize = core::FontSize::cssFontSize(converter.convert(value.sizeCss));
    }
    else if (value.sizeType == api::FontSizeType::point)
    {
        fontSize = core::FontSize::decimal(core::Decimal{static_cast<double>(value.sizePoint)});
    }

    lookForAndSetFontSize(fontSize, &outAttributes);
}

template <typename ATTRIBUTES_TYPE>
void setAttributesFromFontData(const api::FontData &value, ATTRIBUTES_TYPE &outAttributes)
{
    setAttributesFromFontSize(value, outAttributes);
    setAttributesFromFontWeight(value.weight, outAttributes);
    setAttributesFromFontStyle(value.style, outAttributes);
    setAttributesFromFontFamily(value.fontFamily, outAttributes);

    if (value.lineThrough > 0)
    {
        lookForAndSetHasLineThrough(true, &outAttributes);
        lookForAndSetLineThrough(value.lineThrough, &outAttributes);
    }
    else
    {
        lookForAndSetHasLineThrough(false, &outAttributes);
        lookForAndSetLineThrough(value.lineThrough, &outAttributes);
    }

    if (value.overline > 0)
    {
        lookForAndSetHasOverline(true, &outAttributes);
        lookForAndSetOverline(value.overline, &outAttributes);
    }
    else
    {
        lookForAndSetHasOverline(false, &outAttributes);
        lookForAndSetOverline(value.overline, &outAttributes);
    }

    if (value.underline > 0)
    {
        lookForAndSetHasUnderline(true, &outAttributes);
        lookForAndSetUnderline(value.underline, &outAttributes);
    }
    else
    {
        lookForAndSetHasUnderline(false, &outAttributes);
        lookForAndSetUnderline(value.underline, &outAttributes);
    }
}
} // namespace impl
} // namespace mx
