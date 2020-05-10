// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/LayoutFunctions.h"
#include "mx/api/ScoreData.h"
#include "mx/core/elements/Appearance.h"
#include "mx/core/elements/LineWidth.h"
#include "mx/core/elements/NoteSize.h"
#include "mx/core/elements/Distance.h"
#include "mx/core/elements/OtherAppearance.h"
#include "mx/core/elements/BottomMargin.h"
#include "mx/core/elements/Defaults.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/Millimeters.h"
#include "mx/core/elements/PageHeight.h"
#include "mx/core/elements/PageLayout.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/PageWidth.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/Scaling.h"
#include "mx/core/elements/StaffDistance.h"
#include "mx/core/elements/StaffLayout.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/Tenths.h"
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/TopSystemDistance.h"

namespace mx
{
    namespace impl
    {
        void addLayoutData( const api::LayoutData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            addScaling( inLayout, outScoreHeaderGroup );
            addPageMargins( inLayout, outScoreHeaderGroup );
            addSystemMargins( inLayout, outScoreHeaderGroup );
            addAppearance( inLayout, outScoreHeaderGroup );
        }
        
        
        void addScaling( const api::LayoutData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            if( inLayout.scalingMillimeters > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                outScoreHeaderGroup.getDefaults()->setHasScaling( true );
                outScoreHeaderGroup.getDefaults()->getScaling()->getMillimeters()->setValue( core::MillimetersValue{ inLayout.scalingMillimeters } );
            }
            
            if( inLayout.scalingTenths > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                outScoreHeaderGroup.getDefaults()->setHasScaling( true );
                outScoreHeaderGroup.getDefaults()->getScaling()->getTenths()->setValue( core::PositiveDecimal{ inLayout.scalingTenths } );
            }
        }
        
        
        void addPageMargins( const api::LayoutData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            auto& defaults = *outScoreHeaderGroup.getDefaults();
            auto& layout = *defaults.getLayoutGroup();
            auto& pageLayout = *layout.getPageLayout();
            
            if( inLayout.pageWidth >= 0 || inLayout.pageHeight >= 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layout.setHasPageLayout( true );
                pageLayout.getPageWidth()->setValue( core::TenthsValue{ inLayout.pageWidth } );
                pageLayout.getPageHeight()->setValue( core::TenthsValue{ inLayout.pageHeight } );
            }
            bool areEvenOddSame = true;
            areEvenOddSame &= api::areSame( inLayout.pageMargins.evenPageLeftMargin, inLayout.pageMargins.oddPageLeftMargin );
            areEvenOddSame &= api::areSame( inLayout.pageMargins.evenPageRightMargin, inLayout.pageMargins.oddPageRightMargin );
            areEvenOddSame &= api::areSame( inLayout.pageMargins.evenPageTopMargin, inLayout.pageMargins.oddPageTopMargin );
            areEvenOddSame &= api::areSame( inLayout.pageMargins.evenPageBottomMargin, inLayout.pageMargins.oddPageBottomMargin );

            bool areOddPagesSpecified =    inLayout.pageMargins.oddPageLeftMargin > 0
                                        || inLayout.pageMargins.oddPageRightMargin > 0
                                        || inLayout.pageMargins.oddPageTopMargin > 0
                                        || inLayout.pageMargins.oddPageBottomMargin > 0;
            
            bool areEvenPagesSpecified =   inLayout.pageMargins.evenPageLeftMargin > 0
                                        || inLayout.pageMargins.evenPageRightMargin > 0
                                        || inLayout.pageMargins.evenPageTopMargin > 0
                                        || inLayout.pageMargins.evenPageBottomMargin > 0;
            
            if( !areOddPagesSpecified && !areEvenPagesSpecified )
            {
                return;
            }
            
            outScoreHeaderGroup.setHasDefaults( true );
            layout.setHasPageLayout( true );
            
            if( areOddPagesSpecified )
            {
                auto oddPages = core::makePageMargins();
                oddPages->getAttributes()->hasType = true;
                const auto t = areEvenOddSame ? core::MarginType::both : core::MarginType::odd;
                oddPages->getAttributes()->type = t;
                const auto left = core::TenthsValue{ inLayout.pageMargins.oddPageLeftMargin > 0 ? inLayout.pageMargins.oddPageLeftMargin : 0 };
                const auto right = core::TenthsValue{ inLayout.pageMargins.oddPageRightMargin > 0 ? inLayout.pageMargins.oddPageRightMargin : 0 };
                const auto top = core::TenthsValue{ inLayout.pageMargins.oddPageTopMargin > 0 ? inLayout.pageMargins.oddPageTopMargin : 0 };
                const auto bottom = core::TenthsValue{ inLayout.pageMargins.oddPageBottomMargin > 0 ? inLayout.pageMargins.oddPageBottomMargin : 0 };
                oddPages->getLeftMargin()->setValue( left );
                oddPages->getRightMargin()->setValue( right );
                oddPages->getTopMargin()->setValue( top );
                oddPages->getBottomMargin()->setValue( bottom );
                pageLayout.addPageMargins( oddPages );
            }
            
            if( !areEvenOddSame && areEvenPagesSpecified )
            {
                auto evenPages = core::makePageMargins();
                evenPages->getAttributes()->hasType = true;
                const auto t = core::MarginType::even;
                evenPages->getAttributes()->type = t;
                const auto left = core::TenthsValue{ inLayout.pageMargins.evenPageLeftMargin > 0 ? inLayout.pageMargins.evenPageLeftMargin : 0 };
                const auto right = core::TenthsValue{ inLayout.pageMargins.evenPageRightMargin > 0 ? inLayout.pageMargins.evenPageRightMargin : 0 };
                const auto top = core::TenthsValue{ inLayout.pageMargins.evenPageTopMargin > 0 ? inLayout.pageMargins.evenPageTopMargin : 0 };
                const auto bottom = core::TenthsValue{ inLayout.pageMargins.evenPageBottomMargin > 0 ? inLayout.pageMargins.evenPageBottomMargin : 0 };
                evenPages->getLeftMargin()->setValue( left );
                evenPages->getRightMargin()->setValue( right );
                evenPages->getTopMargin()->setValue( top );
                evenPages->getBottomMargin()->setValue( bottom );
                pageLayout.addPageMargins( evenPages );
            }
        }
        
        
        void addSystemMargins( const api::LayoutData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            auto& defaults = *outScoreHeaderGroup.getDefaults();
            auto& layoutGroup = *defaults.getLayoutGroup();
            auto& systemLayout = *layoutGroup.getSystemLayout();
            auto& systemMargins = *systemLayout.getSystemMargins();

            if( inLayout.systemDistance > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemDistance( true );
                systemLayout.getSystemDistance()->setValue( core::TenthsValue{ inLayout.systemDistance } );
            }
            
            if( inLayout.staffDistance > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                auto staffLayout = core::makeStaffLayout();
                staffLayout->setHasStaffDistance( true );
                staffLayout->getStaffDistance()->setValue( core::TenthsValue{ inLayout.staffDistance } );
                layoutGroup.addStaffLayout( staffLayout );
            }
            
            if( inLayout.systemLeftMargin > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemMargins.getLeftMargin()->setValue( core::TenthsValue{ inLayout.systemLeftMargin } );
            }
            
            if( inLayout.systemRightMargin > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemMargins.getRightMargin()->setValue( core::TenthsValue{ inLayout.systemRightMargin } );
            }
            
            if( inLayout.topSystemDistance > 0 )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemLayout.setHasTopSystemDistance( true );
                systemLayout.getTopSystemDistance()->setValue( core::TenthsValue{ inLayout.topSystemDistance } );
            }
        }


        void addAppearance( const api::LayoutData& inLayoutData, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            for( const auto& appearanceData : inLayoutData.appearance )
            {
                if( appearanceData.appearanceType == api::AppearanceType::LineWidth )
                {
                    const auto lw = core::makeLineWidth();
                    lw->getAttributes()->type = core::LineWidthType{ appearanceData.appearanceSubType };
                    lw->setValue( core::TenthsValue{ appearanceData.value } );
                    outScoreHeaderGroup.setHasDefaults( true );
                    outScoreHeaderGroup.getDefaults()->setHasAppearance( true );
                    outScoreHeaderGroup.getDefaults()->getAppearance()->addLineWidth( lw );
                }

                if( appearanceData.appearanceType == api::AppearanceType::NoteSize )
                {
                    const auto ns = core::makeNoteSize();
                    ns->getAttributes()->type = core::parseNoteSizeType( appearanceData.appearanceSubType );
                    ns->setValue( core::NonNegativeDecimal{ appearanceData.value } );
                    outScoreHeaderGroup.setHasDefaults( true );
                    outScoreHeaderGroup.getDefaults()->setHasAppearance( true );
                    outScoreHeaderGroup.getDefaults()->getAppearance()->addNoteSize( ns );
                }

                if( appearanceData.appearanceType == api::AppearanceType::Distance )
                {
                    const auto di = core::makeDistance();
                    di->getAttributes()->type = core::parseDistanceType( appearanceData.appearanceSubType );
                    di->setValue( core::TenthsValue{ appearanceData.value } );
                    outScoreHeaderGroup.setHasDefaults( true );
                    outScoreHeaderGroup.getDefaults()->setHasAppearance( true );
                    outScoreHeaderGroup.getDefaults()->getAppearance()->addDistance( di );
                }

                if( appearanceData.appearanceType == api::AppearanceType::OtherAppearance )
                {
                    const auto oa = core::makeOtherAppearance();
                    oa->getAttributes()->type = appearanceData.appearanceSubType;
                    oa->setValue( core::XsString{ std::to_string( appearanceData.value ) } );
                    outScoreHeaderGroup.setHasDefaults( true );
                    outScoreHeaderGroup.getDefaults()->setHasAppearance( true );
                    outScoreHeaderGroup.getDefaults()->getAppearance()->addOtherAppearance( oa );
                }
            }
        }
        
        
        api::LayoutData createLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup )
        {
            api::LayoutData layout;
            addScaling( inScoreHeaderGroup, layout );
            addPageMargins( inScoreHeaderGroup, layout );
            addSystemMargins( inScoreHeaderGroup, layout );
            addStaffLayout( inScoreHeaderGroup, layout );
            addAppearance( inScoreHeaderGroup, layout );
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
                    outLayoutData.pageMargins.oddPageLeftMargin = m->getLeftMargin()->getValue().getValue();
                    outLayoutData.pageMargins.oddPageRightMargin = m->getRightMargin()->getValue().getValue();
                    outLayoutData.pageMargins.oddPageTopMargin = m->getTopMargin()->getValue().getValue();
                    outLayoutData.pageMargins.oddPageBottomMargin = m->getBottomMargin()->getValue().getValue();
                }
                
                if( !a->hasType || t == core::MarginType::both || t == core::MarginType::even )
                {
                    outLayoutData.pageMargins.evenPageLeftMargin = m->getLeftMargin()->getValue().getValue();
                    outLayoutData.pageMargins.evenPageRightMargin = m->getRightMargin()->getValue().getValue();
                    outLayoutData.pageMargins.evenPageTopMargin = m->getTopMargin()->getValue().getValue();
                    outLayoutData.pageMargins.evenPageBottomMargin = m->getBottomMargin()->getValue().getValue();
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
            if( !inScoreHeaderGroup.getHasDefaults() || layoutSet.empty() )
            {
                return;
            }
            
            const auto& firstStaffLayout = *layoutSet.cbegin();
            
            if( firstStaffLayout->getHasStaffDistance() )
            {
                outLayoutData.staffDistance = firstStaffLayout->getStaffDistance()->getValue().getValue();
            }
            
        }


        void addAppearance( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::LayoutData& outLayoutData )
        {
            outLayoutData.appearance.clear();

            if( !inScoreHeaderGroup.getHasDefaults() )
            {
                return;
            }

            const auto& defaults = *inScoreHeaderGroup.getDefaults();

            if( !defaults.getHasAppearance() )
            {
                return;
            }

            const auto& appearance = *defaults.getAppearance();

            for( const auto& lineWidth : appearance.getLineWidthSet() )
            {
                api::AppearanceData data{};
                data.appearanceType = api::AppearanceType::LineWidth;
                data.appearanceSubType = core::toString( lineWidth->getAttributes()->type );
                data.value = lineWidth->getValue().getValue();
                outLayoutData.appearance.emplace_back( std::move( data ) );
            }

            for( const auto& noteSize : appearance.getNoteSizeSet() )
            {
                api::AppearanceData data{};
                data.appearanceType = api::AppearanceType::NoteSize;
                data.appearanceSubType = core::toString( noteSize->getAttributes()->type );
                data.value = noteSize->getValue().getValue();
                outLayoutData.appearance.emplace_back( std::move( data ) );
            }

            for( const auto& distance : appearance.getDistanceSet() )
            {
                api::AppearanceData data{};
                data.appearanceType = api::AppearanceType::Distance;
                data.appearanceSubType = core::toString( distance->getAttributes()->type );
                data.value = distance->getValue().getValue();
                outLayoutData.appearance.emplace_back( std::move( data ) );
            }

            for( const auto& other : appearance.getOtherAppearanceSet() )
            {
                api::AppearanceData data{};
                data.appearanceType = api::AppearanceType::Distance;
                data.appearanceSubType = core::toString( other->getAttributes()->type );

                // TODO - fix
                data.value = 0.0;//other->getValue().getValue();
                outLayoutData.appearance.emplace_back( std::move( data ) );
            }
        }
    }
}
