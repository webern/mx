// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
        core::Color getColor( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasColor<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                return core::Color{};
            }
            return checkColor<ATTRIBUTES_TYPE>( &inAttributes );
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
                const auto theColor = getColor( ptr );
                outPrintData.color.red = static_cast<uint8_t>( theColor.getRed() );
                outPrintData.color.green = static_cast<uint8_t>( theColor.getGreen() );
                outPrintData.color.blue = static_cast<uint8_t>( theColor.getBlue() );
                outPrintData.color.isAlphaSpecified = theColor.getColorType() == core::Color::ColorType::ARGB;
                
                if( outPrintData.color.isAlphaSpecified )
                {
                    outPrintData.color.alpha = static_cast<uint8_t>( theColor.getAlpha() );
                }
                else
                {
                    outPrintData.color.alpha = 255;
                }
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
                return core::Color{ inColor.red, inColor.green, inColor.blue, inColor.alpha };
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
        void setAttributesColor( const api::ColorData& inColorData, ATTRIBUTES_TYPE& outAttributes )
        {
            lookForAndSetHasColor( true, &outAttributes );
            lookForAndSetColor( createCoreColor( inColorData ), &outAttributes );
        }
        
        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromPrintData( const api::PrintData& inPrintData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( inPrintData.isColorSpecified )
            {
                lookForAndSetHasColor( true, &outAttributes );
                lookForAndSetColor( inPrintData.color, &outAttributes );
            }
            else
            {
                lookForAndSetHasColor( false, &outAttributes );
                lookForAndSetColor( core::Color{}, &outAttributes );
            }
            
            setAttributesFromFontData( inPrintData, outAttributes );
        }
    }
}
