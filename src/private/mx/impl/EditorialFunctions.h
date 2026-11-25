// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/FootnoteData.h"
#include "mx/api/LevelData.h"
#include "mx/core/generated/FormattedText.h"
#include "mx/core/generated/Level.h"

#include <optional>

namespace mx
{
namespace impl
{
// Conversions between the api level and footnote and their core elements.
api::LevelData makeLevelData(const core::Level &inLevel);
core::Level makeCoreLevel(const api::LevelData &inLevelData);
api::FootnoteData makeFootnoteData(const core::FormattedText &inFootnote);
core::FormattedText makeCoreFootnote(const api::FootnoteData &inFootnoteData);

// Read the <level> of any of the three core editorial groups (EditorialGroup, EditorialVoiceGroup,
// EditorialVoiceDirectionGroup). Absent stays absent.
template <typename GROUP> std::optional<api::LevelData> getLevel(const GROUP &inGroup)
{
    if (!inGroup.level().has_value())
    {
        return std::nullopt;
    }

    return makeLevelData(*inGroup.level());
}

// Read the <footnote> of any of the three core editorial groups. Absent stays absent.
template <typename GROUP> std::optional<api::FootnoteData> getFootnote(const GROUP &inGroup)
{
    if (!inGroup.footnote().has_value())
    {
        return std::nullopt;
    }

    return makeFootnoteData(*inGroup.footnote());
}

// Replace the <level> and <footnote> of any of the three core editorial groups; an absent one is
// removed. The group's <voice> is left as it is. A host returns its group by const reference, so
// copy the group out of the host, call this on the copy, and set the copy back.
template <typename GROUP>
void setEditorial(const std::optional<api::LevelData> &inLevel, const std::optional<api::FootnoteData> &inFootnote,
                  GROUP &ioGroup)
{
    if (inLevel.has_value())
    {
        ioGroup.setLevel(makeCoreLevel(*inLevel));
    }
    else
    {
        ioGroup.setLevel(std::nullopt);
    }

    if (inFootnote.has_value())
    {
        ioGroup.setFootnote(makeCoreFootnote(*inFootnote));
    }
    else
    {
        ioGroup.setFootnote(std::nullopt);
    }
}
} // namespace impl
} // namespace mx
