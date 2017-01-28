// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PrintData.h"
#include "mx/core/Color.h"
#include "mx/core/Enums.h"
#include "mx/impl/Converter.h"
#include "mx/impl/FontFunctions.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
    namespace impl
    {
        MX_ATTR_FUNC_OPTIONAL( hasPrintObject, HasPrintObject, bool, false );
        MX_ATTR_FUNC_OPTIONAL( printObject, PrintObject, core::YesNo, core::YesNo::yes );

        MX_ATTR_FUNC_OPTIONAL( hasColor, HasColor, bool, false );
        MX_ATTR_FUNC_OPTIONAL( color, Color, core::Color, core::Color{} );
        
        template <typename ATTRIBUTES_TYPE>
        api::Bool getPrintObject( const ATTRIBUTES_TYPE& inAttributes )
        {
            const ATTRIBUTES_TYPE* const ptr = &inAttributes;
            if( !checkHasPrintObject<ATTRIBUTES_TYPE>( ptr ) )
            {
                return api::Bool::unspecified;
            }
            Converter converter;
            return converter.convert( checkPrintObject<ATTRIBUTES_TYPE>( &inAttributes ) );
        }

        
        template <typename ATTRIBUTES_TYPE>
        api::ColorData getColor( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasColor<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                return api::ColorData{};
            }
            const auto coreColor = checkColor<ATTRIBUTES_TYPE>( &inAttributes );
            api::ColorData outApiColor;
            outApiColor.red = static_cast<uint8_t>( coreColor.getRed() );
            outApiColor.green = static_cast<uint8_t>( coreColor.getGreen() );
            outApiColor.blue = static_cast<uint8_t>( coreColor.getBlue() );
            outApiColor.isAlphaSpecified = coreColor.getColorType() == core::Color::ColorType::ARGB;
            
            if( outApiColor.isAlphaSpecified )
            {
                outApiColor.alpha = static_cast<uint8_t>( coreColor.getAlpha() );
            }
            else
            {
                outApiColor.alpha = 255;
            }
            return outApiColor;
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::PrintData getPrintData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::PrintData outPrintData;
            const ATTRIBUTES_TYPE* const ptr = &inAttributes;
            outPrintData.printObject = getPrintObject<ATTRIBUTES_TYPE>( inAttributes );
            outPrintData.fontData = getFontData<ATTRIBUTES_TYPE>( inAttributes );
            if( checkHasColor<ATTRIBUTES_TYPE>( ptr ) )
            {
                outPrintData.isColorSpecified = true;
                outPrintData.color = getColor( inAttributes );
            }
            else
            {
                outPrintData.isColorSpecified = false;
            }
            return outPrintData;
        }
        
        
        inline core::Color createCoreColor( const api::ColorData& inColor )
        {
            if( inColor.isAlphaSpecified )
            {
                return core::Color{ inColor.alpha, inColor.red, inColor.green, inColor.blue };
            }
            return core::Color{ inColor.red, inColor.green, inColor.blue };
        }
        
        
        MX_ATTR_SETFUNC_OPTIONAL( hasPrintObject, HasPrintObject, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( printObject, PrintObject, core::YesNo, core::YesNo::yes );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasColor, HasColor, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( color, Color, core::Color, core::Color{} );

        
        template <typename ATTRIBUTES_TYPE>
        void setPrintObject( const api::Bool& inPrintObject, ATTRIBUTES_TYPE& outAttributes )
        {
            if( !lookForAndSetHasPrintObject( inPrintObject != api::Bool::unspecified, &outAttributes ) )
            {
                lookForAndSetPrintObject( inPrintObject, &outAttributes );
            }
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromColorData( const api::ColorData& inColorData, ATTRIBUTES_TYPE& outAttributes )
        {
            const auto valueToSet = createCoreColor( inColorData );
            lookForAndSetColor( valueToSet, &outAttributes );
        }

        
        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromPrintData( const api::PrintData& inPrintData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( inPrintData.isColorSpecified )
            {
                lookForAndSetHasColor( true, &outAttributes );
                setAttributesFromColorData( inPrintData.color, outAttributes );
            }
            else
            {
                lookForAndSetHasColor( false, &outAttributes );
                lookForAndSetColor( core::Color{}, &outAttributes );
            }
            
            setAttributesFromFontData( inPrintData.fontData, outAttributes );
        }
    }
}
