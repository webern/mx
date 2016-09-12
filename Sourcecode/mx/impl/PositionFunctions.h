// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/PositionData.h"
#include "mx/core/elements/CreditWordsAttributes.h"
#include "mx/core/Enums.h"

namespace mx
{
    namespace core
    {
        class CreditWordsAttributes;
    }

    namespace impl
    {
        template <typename ATTRIBUTES_TYPE>
        void createPositionData( const api::PositionData& inPositionData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( inPositionData.hasDefaultX )
            {
                outAttributes.hasDefaultX = true;
                outAttributes.defaultX.setValue( inPositionData.defaultX );
            }
            
            if( inPositionData.hasDefaultY )
            {
                outAttributes.hasDefaultY = true;
                outAttributes.defaultY.setValue( inPositionData.defaultY );
            }
        
            if( inPositionData.hasRelativeX )
            {
                outAttributes.hasRelativeX = true;
                outAttributes.relativeX.setValue( inPositionData.relativeX );
            }
            
            if( inPositionData.hasRelativeY )
            {
                outAttributes.hasRelativeY = true;
                outAttributes.relativeY.setValue( inPositionData.relativeY );
            }
            
            switch ( inPositionData.horizontalAlignmnet )
            {
                case api::HorizontalAlignment::unspecified:
                {
                    outAttributes.hasHalign = false;
                    break;
                }
                case api::HorizontalAlignment::left:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::left;
                    break;
                }
                case api::HorizontalAlignment::center:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::center;
                    break;
                }
                case api::HorizontalAlignment::right:
                {
                    outAttributes.hasHalign = true;
                    outAttributes.halign = core::LeftCenterRight::right;
                    break;
                }
                default:
                {
                    outAttributes.hasHalign = false;
                    break;
                }
            }
        }

        template <typename ATTRIBUTES_TYPE>
        void createPositionData( const ATTRIBUTES_TYPE& inAttributes, api::PositionData& outPositionData )
        {
            if( inAttributes.hasDefaultX )
            {
                outPositionData.hasDefaultX = true;
                outPositionData.defaultX = inAttributes.defaultX.getValue();
            }
            
            if( inAttributes.hasDefaultY )
            {
                outPositionData.hasDefaultY = true;
                outPositionData.defaultY = inAttributes.defaultY.getValue();
            }
            
            if( inAttributes.hasRelativeX )
            {
                outPositionData.hasRelativeX = true;
                outPositionData.relativeX = inAttributes.relativeX.getValue();
            }
            
            if( inAttributes.hasRelativeY )
            {
                outPositionData.hasRelativeY = true;
                outPositionData.relativeY = inAttributes.relativeY.getValue();
            }
            
            if( inAttributes.hasHalign )
            {
                switch ( inAttributes.halign )
                {
                    case core::LeftCenterRight::left:
                    {
                        outPositionData.horizontalAlignmnet = api::HorizontalAlignment::left;
                        break;
                    }
                    case core::LeftCenterRight::center:
                    {
                        outPositionData.horizontalAlignmnet = api::HorizontalAlignment::center;
                        break;
                    }
                    case core::LeftCenterRight::right:
                    {
                        outPositionData.horizontalAlignmnet = api::HorizontalAlignment::right;
                        break;
                    }
                    default:
                    {
                        outPositionData.horizontalAlignmnet = api::HorizontalAlignment::unspecified;
                        break;
                    }
                }
            }
            else
            {
                outPositionData.horizontalAlignmnet = api::HorizontalAlignment::unspecified;
                
            }
        }
    }
}
