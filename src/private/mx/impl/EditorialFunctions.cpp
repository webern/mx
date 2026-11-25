// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/EditorialFunctions.h"
#include "mx/impl/Converter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
namespace impl
{
api::LevelData makeLevelData(const core::Level &inLevel)
{
    Converter converter;
    api::LevelData outLevel;
    outLevel.text = inLevel.value();

    if (inLevel.reference().has_value())
    {
        outLevel.reference = converter.convert(*inLevel.reference());
    }

    if (inLevel.type().has_value())
    {
        outLevel.type = converter.convertLevelType(*inLevel.type());
    }

    if (inLevel.parentheses().has_value())
    {
        outLevel.parentheses = converter.convert(*inLevel.parentheses());
    }

    if (inLevel.bracket().has_value())
    {
        outLevel.bracket = converter.convert(*inLevel.bracket());
    }

    if (inLevel.size().has_value())
    {
        outLevel.size = converter.convert(*inLevel.size());
    }

    return outLevel;
}

core::Level makeCoreLevel(const api::LevelData &inLevelData)
{
    Converter converter;
    core::Level outLevel;
    outLevel.setValue(inLevelData.text);

    if (inLevelData.reference != api::Bool::unspecified)
    {
        outLevel.setReference(converter.convert(inLevelData.reference));
    }

    if (inLevelData.type != api::LevelType::unspecified)
    {
        outLevel.setType(converter.convertLevelType(inLevelData.type));
    }

    if (inLevelData.parentheses != api::Bool::unspecified)
    {
        outLevel.setParentheses(converter.convert(inLevelData.parentheses));
    }

    if (inLevelData.bracket != api::Bool::unspecified)
    {
        outLevel.setBracket(converter.convert(inLevelData.bracket));
    }

    if (inLevelData.size != api::SymbolSize::unspecified)
    {
        outLevel.setSize(converter.convert(inLevelData.size));
    }

    return outLevel;
}

api::FootnoteData makeFootnoteData(const core::FormattedText &inFootnote)
{
    Converter converter;
    api::FootnoteData outFootnote;
    outFootnote.text = inFootnote.value();
    outFootnote.positionData = getPositionData(inFootnote);
    outFootnote.fontData = getFontData(inFootnote);

    if (inFootnote.color().has_value())
    {
        outFootnote.color = getColor(inFootnote);
    }

    if (inFootnote.justify().has_value())
    {
        outFootnote.justify = converter.convert(*inFootnote.justify());
    }

    if (inFootnote.enclosure().has_value())
    {
        outFootnote.enclosure = converter.convert(*inFootnote.enclosure());
    }

    if (inFootnote.xmlSpace().has_value())
    {
        outFootnote.xmlSpace = converter.convert(*inFootnote.xmlSpace());
    }

    return outFootnote;
}

core::FormattedText makeCoreFootnote(const api::FootnoteData &inFootnoteData)
{
    Converter converter;
    core::FormattedText outFootnote;
    outFootnote.setValue(inFootnoteData.text);

    // <footnote> does not have a placement attribute, so setAttributesFromPositionData skips it.
    setAttributesFromPositionData(inFootnoteData.positionData, outFootnote);
    setAttributesFromFontData(inFootnoteData.fontData, outFootnote);

    if (inFootnoteData.color.has_value())
    {
        setAttributesFromColorData(*inFootnoteData.color, outFootnote);
    }

    if (inFootnoteData.justify != api::HorizontalAlignment::unspecified)
    {
        outFootnote.setJustify(converter.convert(inFootnoteData.justify));
    }

    if (inFootnoteData.enclosure != api::Enclosure::unspecified)
    {
        outFootnote.setEnclosure(converter.convert(inFootnoteData.enclosure));
    }

    if (inFootnoteData.xmlSpace != api::XmlSpace::unspecified)
    {
        outFootnote.setXMLSpace(converter.convert(inFootnoteData.xmlSpace));
    }

    return outFootnote;
}
} // namespace impl
} // namespace mx
