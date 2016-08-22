// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
        void addStaffLayout( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup );
        
        api::LayoutData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup );
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData );
        
    }
}
