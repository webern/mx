// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/EditorialData.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/Level.h"
#include "mx/impl/Converter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
namespace impl
{
// The MusicXML editorial group (<footnote> + <level>) is shared by several elements (<part-group>,
// <direction>, ...). These helpers translate it in both directions, templated on the core group
// type (EditorialGroup, EditorialVoiceDirectionGroup, ...) which all expose footnote()/level().

inline api::FootnoteData getFootnoteData(const core::FormattedText &ft)
{
    api::FootnoteData out;
    out.text = ft.value();
    out.positionData = getPositionData(ft);
    out.fontData = getFontData(ft);
    out.isColorSpecified = ft.color().has_value();
    if (out.isColorSpecified)
    {
        out.colorData = getColor(ft);
    }
    return out;
}

inline core::FormattedText makeFootnote(const api::FootnoteData &fn)
{
    core::FormattedText ft;
    ft.setValue(fn.text);
    setAttributesFromPositionData(fn.positionData, ft);
    setAttributesFromFontData(fn.fontData, ft);
    if (fn.isColorSpecified)
    {
        setAttributesFromColorData(fn.colorData, ft);
    }
    return ft;
}

inline api::LevelData getLevelData(const core::Level &lvl)
{
    const Converter c;
    api::LevelData out;
    out.value = lvl.value();
    if (lvl.reference().has_value())
    {
        out.reference = c.convert(*lvl.reference());
    }
    if (lvl.type().has_value())
    {
        out.type = c.convert(*lvl.type());
    }
    if (lvl.parentheses().has_value())
    {
        out.parentheses = c.convert(*lvl.parentheses());
    }
    if (lvl.bracket().has_value())
    {
        out.bracket = c.convert(*lvl.bracket());
    }
    if (lvl.size().has_value())
    {
        out.size = c.convert(*lvl.size());
    }
    return out;
}

inline core::Level makeLevel(const api::LevelData &lvl)
{
    const Converter c;
    core::Level out;
    out.setValue(lvl.value);
    if (lvl.reference != api::Bool::unspecified)
    {
        out.setReference(c.convert(lvl.reference));
    }
    if (lvl.type != api::StartStopSingle::unspecified)
    {
        out.setType(c.convert(lvl.type));
    }
    if (lvl.parentheses != api::Bool::unspecified)
    {
        out.setParentheses(c.convert(lvl.parentheses));
    }
    if (lvl.bracket != api::Bool::unspecified)
    {
        out.setBracket(c.convert(lvl.bracket));
    }
    if (lvl.size != api::SymbolSize::unspecified)
    {
        out.setSize(c.convert(lvl.size));
    }
    return out;
}

// Reads footnote/level off any core editorial group into an api::EditorialData.
template <typename GROUP> api::EditorialData getEditorialData(const GROUP &group)
{
    api::EditorialData out;
    if (group.footnote().has_value())
    {
        out.isFootnoteSpecified = true;
        out.footnote = getFootnoteData(*group.footnote());
    }
    if (group.level().has_value())
    {
        out.isLevelSpecified = true;
        out.level = getLevelData(*group.level());
    }
    return out;
}

// Writes footnote/level from an api::EditorialData onto any core editorial group.
template <typename GROUP> void setEditorial(const api::EditorialData &ed, GROUP &group)
{
    if (ed.isFootnoteSpecified)
    {
        group.setFootnote(makeFootnote(ed.footnote));
    }
    if (ed.isLevelSpecified)
    {
        group.setLevel(makeLevel(ed.level));
    }
}
} // namespace impl
} // namespace mx
