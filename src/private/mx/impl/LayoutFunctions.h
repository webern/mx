// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DefaultsData.h"
#include "mx/core/generated/ScoreHeaderGroup.h"

namespace mx
{
namespace impl
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// api::DefaultsData -> core::ScoreHeaderGroup
void addDefaultsData(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup);
void addScaling(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup);
void addPageLayout(const api::PageLayoutData &inPageLayout, core::ScoreHeaderGroup &outScoreHeaderGroup);
void addSystemMargins(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup);
void addAppearance(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup);
void addDefaultsFonts(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// core::ScoreHeaderGroup -> api::DefaultsData
api::DefaultsData createDefaults(const core::ScoreHeaderGroup &inScoreHeaderGroup);
void addScaling(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
void addPageMargins(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
void addSystemMargins(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
void addStaffLayout(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
void addAppearance(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
void addDefaultsFonts(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults);
} // namespace impl
} // namespace mx
