// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/LayoutData.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/Tenths.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/PageLayout.h"

namespace mx
{
    namespace impl
    {
        void addLayoutData( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addScaling( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );

        void addPageLayoutOld(const api::PageLayoutData& inPageLayout, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageMargins(const api::PageMarginsData& inPageMargins, core::PageLayout& outPageLayout );



        void addSystemMargins( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addAppearance( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        
        api::LayoutData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addAppearance( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );

        inline core::TenthsValue toTenths( long double value )
        {
            return core::TenthsValue{ value > 0.0 ? value : 0.0 };
        }
    }
}
