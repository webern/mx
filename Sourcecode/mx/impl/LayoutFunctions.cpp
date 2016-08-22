// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/LayoutFunctions.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/Defaults.h"
#include "mx/core/elements/Scaling.h"
#include "mx/core/elements/Millimeters.h"
#include "mx/core/elements/Tenths.h"
#include "mx/core/elements/PageLayout.h"
#include "mx/core/elements/PageHeight.h"
#include "mx/core/elements/PageWidth.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/BottomMargin.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/StaffLayout.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/elements/StaffDistance.h"

namespace mx
{
    namespace impl
    {
        void addLayoutData( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            addScaling( inScore, outScoreHeaderGroup );
            addPageMargins( inScore, outScoreHeaderGroup );
            addSystemMargins( inScore, outScoreHeaderGroup );
            addStaffLayout( inScore, outScoreHeaderGroup );
        }
        
        
        void addScaling( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            
        }
        
        
        void addPageMargins( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            
        }
        
        
        void addSystemMargins( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            
        }
        
        
        void addStaffLayout( const api::LayoutData& inScore, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            
        }
        
        
        api::LayoutData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup )
        {
            api::LayoutData layout;
            addScaling( inScoreHeaderGroup, layout );
            addPageMargins( inScoreHeaderGroup, layout );
            addSystemMargins( inScoreHeaderGroup, layout );
            addStaffLayout( inScoreHeaderGroup, layout );
            return layout;
        }
        
        
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData )
        {
            if( inScoreHeaderGroup.getHasDefaults() && inScoreHeaderGroup.getDefaults()->getHasScaling() )
            {
                auto scaling = inScoreHeaderGroup.getDefaults()->getScaling();
                outLayoutData.scalingMillimeters = scaling->getMillimeters()->getValue().getValue();
                outLayoutData.scalingTenths = scaling->getTenths()->getValue().getValue();
            }
            else
            {
                outLayoutData.scalingMillimeters = -1.0;
                outLayoutData.scalingTenths = -1.0;
            }
        }
        
        
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData )
        {
            if( !inScoreHeaderGroup.getHasDefaults() || !inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getHasPageLayout() )
            {
                return;
            }
            
            auto pageLayout = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getPageLayout();
            outLayoutData.pageWidth = pageLayout->getPageWidth()->getValue().getValue();
            outLayoutData.pageHeight = pageLayout->getPageHeight()->getValue().getValue();
            
            auto pageMargins = pageLayout->getPageMarginsSet();
            
            for( auto m : pageMargins )
            {
                const auto a = m->getAttributes();
                const auto t = a->type;
                
                if( !a->hasType || t == core::MarginType::both || t == core::MarginType::odd )
                {
                    outLayoutData.oddPageLeftMargin = m->getLeftMargin()->getValue().getValue();
                    outLayoutData.oddPageRightMargin = m->getRightMargin()->getValue().getValue();
                    outLayoutData.oddPageTopMargin = m->getTopMargin()->getValue().getValue();
                    outLayoutData.oddPageBottomMargin = m->getBottomMargin()->getValue().getValue();
                }
                
                if( !a->hasType || t == core::MarginType::both || t == core::MarginType::even )
                {
                    outLayoutData.evenPageLeftMargin = m->getLeftMargin()->getValue().getValue();
                    outLayoutData.evenPageRightMargin = m->getRightMargin()->getValue().getValue();
                    outLayoutData.evenPageTopMargin = m->getTopMargin()->getValue().getValue();
                    outLayoutData.evenPageBottomMargin = m->getBottomMargin()->getValue().getValue();
                }
            }
            
            
        }
        
        
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData )
        {
            if( !inScoreHeaderGroup.getHasDefaults() || !inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getHasSystemLayout() )
            {
                return;
            }
            
            auto systemLayout = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getSystemLayout();
            
            if( systemLayout->getHasTopSystemDistance() )
            {
                outLayoutData.topSystemDistance = systemLayout->getTopSystemDistance()->getValue().getValue();
            }
            
            if( systemLayout->getHasSystemDistance() )
            {
                outLayoutData.systemDistance = systemLayout->getSystemDistance()->getValue().getValue();
            }
            
            if( systemLayout->getHasSystemMargins() )
            {
                auto systemMargins = systemLayout->getSystemMargins();
                outLayoutData.systemLeftMargin = systemMargins->getLeftMargin()->getValue().getValue();
                outLayoutData.systemRightMargin = systemMargins->getRightMargin()->getValue().getValue();
            }
        
        }
        
        
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData )
        {
            const auto& layoutSet = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getStaffLayoutSet();
            if( !inScoreHeaderGroup.getHasDefaults() || layoutSet.size() == 0 )
            {
                return;
            }
            
            const auto& firstStaffLayout = *layoutSet.cbegin();
            
            if( firstStaffLayout->getHasStaffDistance() )
            {
                outLayoutData.staffDistance = firstStaffLayout->getStaffDistance()->getValue().getValue();
            }
            
        }
        
    }
}