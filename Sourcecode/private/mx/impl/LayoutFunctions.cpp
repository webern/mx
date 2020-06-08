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
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/TopSystemDistance.h"

namespace mx
{
    namespace impl
    {
        void addLayoutData( const api::DefaultsData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            addScaling( inLayout, outScoreHeaderGroup );
            addPageLayout( inLayout.pageLayout, outScoreHeaderGroup);
            addSystemMargins( inLayout, outScoreHeaderGroup );
            addAppearance( inLayout, outScoreHeaderGroup );
        }
        
        
        void addScaling( const api::DefaultsData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
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

        core::PageMarginsSet createPageMargins( const api::PageMarginsData& inPageMargins )
        {
            core::PageMarginsSet outPageMargins;

            if( !inPageMargins.isUsed() )
            {
                return outPageMargins;
            }

            const bool areEvenOddSame = inPageMargins.same();

            // these margins will serve as either the 'odd' margins, or 'both' (if 'even' and 'odd' are the same).
            if( inPageMargins.odd )
            {
                auto outMargins = core::makePageMargins();
                outMargins->getAttributes()->hasType = true;
                const auto t = areEvenOddSame ? core::MarginType::both : core::MarginType::odd;
                outMargins->getAttributes()->type = t;
                const auto left = toTenths( inPageMargins.odd.value().left );
                const auto right = toTenths( inPageMargins.odd.value().right );
                const auto top = toTenths( inPageMargins.odd.value().top );
                const auto bottom = toTenths( inPageMargins.odd.value().bottom );
                outMargins->getLeftMargin()->setValue( left );
                outMargins->getRightMargin()->setValue( right );
                outMargins->getTopMargin()->setValue( top );
                outMargins->getBottomMargin()->setValue( bottom );
                outPageMargins.push_back( outMargins );
            }

            // these margins ('even') are only needed if 'both' was not specified above because.
            if( inPageMargins.even && !areEvenOddSame )
            {
                auto outEvenMargins = core::makePageMargins();
                outEvenMargins->getAttributes()->hasType = true;
                const auto t = core::MarginType::even;
                outEvenMargins->getAttributes()->type = t;
                const auto left = toTenths( inPageMargins.even.value().left );
                const auto right = toTenths( inPageMargins.even.value().right );
                const auto top = toTenths( inPageMargins.even.value().top );
                const auto bottom = toTenths( inPageMargins.even.value().bottom );
                outEvenMargins->getLeftMargin()->setValue( left );
                outEvenMargins->getRightMargin()->setValue( right );
                outEvenMargins->getTopMargin()->setValue( top );
                outEvenMargins->getBottomMargin()->setValue( bottom );
                outPageMargins.push_back( outEvenMargins );
            }

            return outPageMargins;
        }

        core::PageLayoutPtr createPageLayout( const api::PageLayoutData& inPageLayout )
        {
            auto outPageLayout = core::makePageLayout();

            if( inPageLayout.size )
            {
                const auto& size = inPageLayout.size.value();
                outPageLayout->getPageHeight()->setValue( core::TenthsValue{ size.height } );
                outPageLayout->getPageWidth()->setValue( core::TenthsValue{ size.width } );
            }

            if( !inPageLayout.margins.isUsed() )
            {
                return outPageLayout;
            }

            auto outPageMarginsSet = createPageMargins( inPageLayout.margins );
            for( auto& outPageMargins : outPageMarginsSet )
            {
                outPageLayout->addPageMargins( outPageMargins );
            }

            return outPageLayout;
        }
        
        
        void addPageLayout( const api::PageLayoutData& inPageLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            if( !inPageLayout.isUsed() )
            {
                return;
            }

            auto& outDefaults = *outScoreHeaderGroup.getDefaults();
            auto& outLayout = *outDefaults.getLayoutGroup();
            outScoreHeaderGroup.setHasDefaults( true );
            outLayout.setHasPageLayout(true );
            auto outPageLayout = createPageLayout( inPageLayout );
            outLayout.setPageLayout( outPageLayout );
        }

        void addSystemMargins( const api::DefaultsData& inLayout, core::ScoreHeaderGroup& outScoreHeaderGroup )
        {
            auto& defaults = *outScoreHeaderGroup.getDefaults();
            auto& layoutGroup = *defaults.getLayoutGroup();
            auto& systemLayout = *layoutGroup.getSystemLayout();
            auto& systemMargins = *systemLayout.getSystemMargins();

            if( inLayout.systemLayout.systemDistance )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemDistance( true );
                systemLayout.getSystemDistance()->setValue(
                        core::TenthsValue{ inLayout.systemLayout.systemDistance.value() }
                );
            }
            
            if( inLayout.systemLayout.staffDistance )
            {
                outScoreHeaderGroup.setHasDefaults( true );
                auto staffLayout = core::makeStaffLayout();
                staffLayout->setHasStaffDistance( true );
                staffLayout->getStaffDistance()->setValue(
                        core::TenthsValue{ inLayout.systemLayout.staffDistance.value() }
                );
                layoutGroup.addStaffLayout( staffLayout );
            }
            
            if( inLayout.systemLayout.margins )
            {
                const auto& margins = inLayout.systemLayout.margins.value();
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemMargins.getLeftMargin()->setValue( core::TenthsValue{ margins.left } );
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemMargins.getRightMargin()->setValue( core::TenthsValue{ margins.right } );
            }
            
            if( inLayout.systemLayout.topSystemDistance )
            {
                const auto& top = inLayout.systemLayout.topSystemDistance.value();
                outScoreHeaderGroup.setHasDefaults( true );
                layoutGroup.setHasSystemLayout( true );
                systemLayout.setHasSystemMargins( true );
                systemLayout.setHasTopSystemDistance( true );
                systemLayout.getTopSystemDistance()->setValue( core::TenthsValue{ top } );
            }
        }


        void addAppearance( const api::DefaultsData& inLayoutData, core::ScoreHeaderGroup& outScoreHeaderGroup )
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
        
        
        api::DefaultsData createDefaults( const core::ScoreHeaderGroup& inScoreHeaderGroup )
        {
            api::DefaultsData defaults;
            addScaling( inScoreHeaderGroup, defaults );
            addPageMargins( inScoreHeaderGroup, defaults );
            addSystemMargins( inScoreHeaderGroup, defaults );
            addStaffLayout( inScoreHeaderGroup, defaults );
            addAppearance( inScoreHeaderGroup, defaults );
            return defaults;
        }
        
        
        void addScaling( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData )
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
        
        
        void addPageMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData )
        {
            if( !inScoreHeaderGroup.getHasDefaults() || !inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getHasPageLayout() )
            {
                return;
            }
            
            auto pageLayout = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getPageLayout();
            outLayoutData.pageLayout.size = api::SizeData{
                    pageLayout->getPageHeight()->getValue().getValue(),
                    pageLayout->getPageWidth()->getValue().getValue()
            };
            auto pageMargins = pageLayout->getPageMarginsSet();
            for( const auto& m : pageMargins )
            {
                const auto a = m->getAttributes();
                const auto t = a->type;
                
                if( !a->hasType || t == core::MarginType::both || t == core::MarginType::odd )
                {
                    outLayoutData.pageLayout.margins.odd =
                            api::MarginsData{
                                    m->getLeftMargin()->getValue().getValue(),
                                    m->getRightMargin()->getValue().getValue(),
                                    m->getTopMargin()->getValue().getValue(),
                                    m->getBottomMargin()->getValue().getValue()
                    };
                }
                
                if( !a->hasType || t == core::MarginType::both || t == core::MarginType::even )
                {
                    outLayoutData.pageLayout.margins.even =
                            api::MarginsData{
                                    m->getLeftMargin()->getValue().getValue(),
                                    m->getRightMargin()->getValue().getValue(),
                                    m->getTopMargin()->getValue().getValue(),
                                    m->getBottomMargin()->getValue().getValue()
                            };
                }
            }
            
            
        }
        
        
        void addSystemMargins( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData )
        {
            if( !inScoreHeaderGroup.getHasDefaults() || !inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getHasSystemLayout() )
            {
                return;
            }
            
            auto systemLayout = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getSystemLayout();
            
            if( systemLayout->getHasTopSystemDistance() )
            {
                outLayoutData.systemLayout.topSystemDistance =
                        systemLayout->getTopSystemDistance()->getValue().getValue();
            }
            
            if( systemLayout->getHasSystemDistance() )
            {
                outLayoutData.systemLayout.systemDistance =
                        systemLayout->getSystemDistance()->getValue().getValue();
            }
            
            if( systemLayout->getHasSystemMargins() )
            {
                auto systemMargins = systemLayout->getSystemMargins();
                outLayoutData.systemLayout.margins =
                        api::LeftRight{
                            systemMargins->getLeftMargin()->getValue().getValue(),
                            systemMargins->getRightMargin()->getValue().getValue()
                };
            }
        }
        
        
        void addStaffLayout( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData )
        {
            const auto& layoutSet = inScoreHeaderGroup.getDefaults()->getLayoutGroup()->getStaffLayoutSet();
            if( !inScoreHeaderGroup.getHasDefaults() || layoutSet.empty() )
            {
                return;
            }
            
            const auto& firstStaffLayout = *layoutSet.cbegin();
            
            if( firstStaffLayout->getHasStaffDistance() )
            {
                outLayoutData.systemLayout.staffDistance =
                        firstStaffLayout->getStaffDistance()->getValue().getValue();
            }
        }


        void addAppearance( const core::ScoreHeaderGroup& inScoreHeaderGroup, api::DefaultsData& outLayoutData )
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
