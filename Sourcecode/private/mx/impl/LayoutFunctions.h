// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/SetupData.h"
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
        // api::SetupData -> core::ScoreHeaderGroup
        void addLayoutData( const api::SetupData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addScaling( const api::SetupData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageLayout( const api::PageLayoutData& inPageLayout, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageMargins( const api::PageMarginsData& inPageMargins, core::PageLayout& outPageLayout );
        void addSystemMargins( const api::SetupData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addAppearance( const api::SetupData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // core::ScoreHeaderGroup -> api::SetupData
        api::SetupData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup );
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::SetupData& outLayoutData );
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::SetupData& outLayoutData );
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::SetupData& outLayoutData );
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::SetupData& outLayoutData );
        void addAppearance( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::SetupData& outLayoutData );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Util
        inline core::TenthsValue toTenths( long double value )
        {
            return core::TenthsValue{ value > 0.0 ? value : 0.0 };
        }
    }
}
