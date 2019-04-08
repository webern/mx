// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PositionData.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
    namespace impl
    {
        using LongDouble = long double;
        
        MX_ATTR_FUNC_OPTIONAL( hasDefaultX, HasDefaultX, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( defaultX, DefaultX, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasDefaultY, HasDefaultY, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( defaultY, DefaultY, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasRelativeX, HasRelativeX, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( relativeX, RelativeX, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasRelativeY, HasRelativeY, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( relativeY, RelativeY, LongDouble, 1.0L );
        
        MX_ATTR_FUNC_OPTIONAL( hasHalign, HasHalign, bool, false );
        MX_ATTR_FUNC_OPTIONAL( halign, Halign, core::LeftCenterRight, core::LeftCenterRight::left );

        MX_ATTR_FUNC_OPTIONAL( hasValign, HasValign, bool, false );
        MX_ATTR_FUNC_OPTIONAL( valign, Valign, core::Valign, core::Valign::baseline );

        MX_ATTR_FUNC_OPTIONAL( hasPlacement, HasPlacement, bool, false );
        MX_ATTR_FUNC_OPTIONAL( placement, Placement, core::AboveBelow, core::AboveBelow::above );


        template <typename ATTRIBUTES_TYPE>
        api::PositionData getPositionData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::PositionData outPositionData;
            
            if( checkHasDefaultX<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.isDefaultXSpecified = true;
                outPositionData.defaultX = checkDefaultX<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            if( checkHasDefaultY<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.isDefaultYSpecified = true;
                outPositionData.defaultY = checkDefaultY<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            if( checkHasRelativeX<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.isRelativeXSpecified = true;
                outPositionData.relativeX = checkRelativeX<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            if( checkHasRelativeY<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.isRelativeYSpecified = true;
                outPositionData.relativeY = checkRelativeY<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            impl::Converter converter;
            
            if( checkHasHalign<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.horizontalAlignmnet = converter.convert( checkHalign<ATTRIBUTES_TYPE>( &inAttributes ) );
            }
            else
            {
                outPositionData.horizontalAlignmnet = api::HorizontalAlignment::unspecified;
                
            }
            
            if( checkHasValign<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.verticalAlignment = converter.convert( checkValign<ATTRIBUTES_TYPE>( &inAttributes ) );
            }
            else
            {
                outPositionData.verticalAlignment = api::VerticalAlignment::unspecified;
            }

            if( checkHasPlacement<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPositionData.placement = converter.convert( checkPlacement<ATTRIBUTES_TYPE>( &inAttributes ) );
            }
            else
            {
                outPositionData.placement = api::Placement::unspecified;
            }
            
            return outPositionData;
        }

        MX_ATTR_SETFUNC_OPTIONAL( hasDefaultX, HasDefaultX, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( defaultX, DefaultX, LongDouble, 1.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasDefaultY, HasDefaultY, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( defaultY, DefaultY, LongDouble, 1.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasRelativeX, HasRelativeX, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( relativeX, RelativeX, LongDouble, 1.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasRelativeY, HasRelativeY, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( relativeY, RelativeY, LongDouble, 1.0L );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasHalign, HasHalign, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( halign, Halign, core::LeftCenterRight, core::LeftCenterRight::left );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasValign, HasValign, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( valign, Valign, core::Valign, core::Valign::baseline );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasPlacement, HasPlacement, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( placement, Placement, core::AboveBelow, core::AboveBelow::above );
        
        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromPositionData( const api::PositionData& positionData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( positionData.isDefaultXSpecified )
            {
                lookForAndSetHasDefaultX( true, &outAttributes );
                lookForAndSetDefaultX( positionData.defaultX, &outAttributes );
            }
            else
            {
                lookForAndSetHasDefaultX( false, &outAttributes );
                lookForAndSetDefaultX( 0.0, &outAttributes );
            }
            
            if( positionData.isDefaultYSpecified )
            {
                lookForAndSetHasDefaultY( true, &outAttributes );
                lookForAndSetDefaultY( positionData.defaultY, &outAttributes );
            }
            else
            {
                lookForAndSetHasDefaultY( false, &outAttributes );
                lookForAndSetDefaultY( 0.0, &outAttributes );
            }
            
            if( positionData.isRelativeXSpecified )
            {
                lookForAndSetHasRelativeX( true, &outAttributes );
                lookForAndSetRelativeX( positionData.relativeX, &outAttributes );
            }
            else
            {
                lookForAndSetHasRelativeX( false, &outAttributes );
                lookForAndSetRelativeX( 0.0, &outAttributes );
            }
            
            if( positionData.isRelativeYSpecified )
            {
                lookForAndSetHasRelativeY( true, &outAttributes );
                lookForAndSetRelativeY( positionData.relativeY, &outAttributes );
            }
            else
            {
                lookForAndSetHasRelativeY( false, &outAttributes );
                lookForAndSetRelativeY( 0.0, &outAttributes );
            }
            
            Converter converter;
            
            if( positionData.horizontalAlignmnet == api::HorizontalAlignment::unspecified )
            {
                lookForAndSetHasHalign( false, &outAttributes );
            }
            else
            {
                lookForAndSetHasHalign( true, &outAttributes );
                lookForAndSetHalign( converter.convert( positionData.horizontalAlignmnet ), &outAttributes );
            }
            
            if( positionData.verticalAlignment == api::VerticalAlignment::unspecified )
            {
                lookForAndSetHasValign( false, &outAttributes );
            }
            else
            {
                lookForAndSetHasValign( true, &outAttributes );
                lookForAndSetValign( converter.convert( positionData.verticalAlignment ), &outAttributes );
            }
            
            if( positionData.placement == api::Placement::unspecified )
            {
                lookForAndSetHasPlacement( false, &outAttributes );
            }
            else
            {
                lookForAndSetHasPlacement( true, &outAttributes );
                lookForAndSetPlacement( converter.convert( positionData.placement ), &outAttributes );
            }
        }
    }
}
