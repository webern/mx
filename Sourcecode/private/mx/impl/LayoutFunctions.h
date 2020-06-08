// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DefaultsData.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/Tenths.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/PageLayout.h"

namespace mx
{
    namespace impl
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // api::Page* -> core:Page*
        core::PageMarginsSet createPageMargins( const api::PageMarginsData& inPageMargins );
        core::PageLayoutPtr createPageLayout( const api::PageLayoutData& inPageLayout );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // api::DefaultsData -> core::ScoreHeaderGroup
        void addDefaultsData( const api::DefaultsData& inDefaults, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addScaling( const api::DefaultsData& inDefaults, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageLayout( const api::PageLayoutData& inPageLayout, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageMargins( const api::PageMarginsData& inPageMargins, core::PageLayout& outPageLayout );
        void addSystemMargins(const api::DefaultsData& inDefaults, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addAppearance(const api::DefaultsData& inDefaults, core::ScoreHeaderGroup& outScoreHeaderGroup );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // core::ScoreHeaderGroup -> api::DefaultsData
        api::DefaultsData createDefaults( const core::ScoreHeaderGroup& inScoreHeaderGroup );
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData );
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData );
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData );
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData );
        void addAppearance( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Util
        inline core::TenthsValue toTenths( long double value )
        {
            return core::TenthsValue{ value > 0.0 ? value : 0.0 };
        }
    }
}
