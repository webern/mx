// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/LayoutData.h"
#include "mx/core/elements/ScoreHeaderGroup.h"

namespace mx
{
    namespace impl
    {
        void addLayoutData( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addScaling( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addPageMargins( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        void addSystemMargins( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        
        api::LayoutData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup );
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        
    }
}
