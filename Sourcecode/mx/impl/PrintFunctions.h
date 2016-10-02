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
            if( !checkHasPrintObject( &inAttributes ) )
            {
                return api::Bool::unspecified;
            }
            Converter converter;
            return converter.convert( checkPrintObject( &inAttributes ) );
        }

        
        template <typename ATTRIBUTES_TYPE>
        core::Color getColor( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasColor( &inAttributes ) )
            {
                return core::Color{};
            }
            return checkColor( &inAttributes );
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::PrintData getPrintData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::PrintData outPrintData;
            outPrintData.printObject = getPrintObject( &inAttributes );
            outPrintData.fontData = getFontData( &inAttributes );
            
            if( checkHasColor( &inAttributes ) )
            {
                outPrintData.isColorSpecified = true;
                const auto theColor = getColor( &inAttributes );
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
    }
}
