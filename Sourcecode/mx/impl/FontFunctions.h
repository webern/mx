// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/FontData.h"
#include "mx/core/Enums.h"
#include "mx/core/FontSize.h"
#include "mx/core/Strings.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
    namespace impl
    {        
        MX_ATTR_FUNC_OPTIONAL( hasFontFamily, HasFontFamily, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( fontFamily, FontFamily, core::CommaSeparatedText, core::CommaSeparatedText{} );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontStyle, HasFontStyle, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontStyle, FontStyle, core::FontStyle, core::FontStyle{} );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontSize, HasFontSize, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( fontSize, FontSize, core::FontSize, core::FontSize{ core::CssFontSize::medium } );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontWeight, HasFontWeight, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontWeight, FontWeight, core::FontWeight, core::FontWeight::normal );

        template <typename ATTRIBUTES_TYPE>
        std::vector<std::string> getFontFamily( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasFontFamily( &inAttributes ) )
            {
                return std::vector<std::string>{};
            }
            
            const auto& fontFamily = checkFontFamily( &inAttributes );
            auto iter = fontFamily.getValuesBeginConst();
            auto end = fontFamily.getValuesEndConst();
            std::vector<std::string> outStrings;
            
            for( ; iter != end; ++iter )
            {
                outStrings.emplace_back( iter->getValue() );
            }
            return outStrings;
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::FontStyle getFontStyle( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasFontStyle( &inAttributes ) )
            {
                return api::FontStyle::unspecified;
            }
            Converter converter;
            return converter.convert( checkFontStyle( &inAttributes ) );
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::FontWeight getFontWeight( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasFontWeight( &inAttributes ) )
            {
                return api::FontWeight::unspecified;
            }
            
            Converter converter;
            return converter.convert( checkFontWeight( &inAttributes ) );
        }
        
        
        
        
        template <typename ATTRIBUTES_TYPE>
        api::FontSizeType getFontSize( const ATTRIBUTES_TYPE& inAttributes,
                                        core::DecimalType& outPointSize,
                                        api::CssSize& outCssSize )
        {
            if( !checkHasFontSize( &inAttributes ) )
            {
                outPointSize = -1.0L;
                outCssSize = api::CssSize::unspecified;
                return api::FontSizeType::unspecified;
            }
            
            const auto& coreFontSize = checkFontSize( &inAttributes );
            Converter converter;
            
            if( coreFontSize.getIsCssFontSize() )
            {
                outPointSize = -1.0L;
                outCssSize = converter.convert( coreFontSize.getValueCssFontSize() );
                return api::FontSizeType::css;
            }
            else
            {
                outPointSize = coreFontSize.getValueNumber().getValue();
                outCssSize = api::CssSize::unspecified;
                return api::FontSizeType::unspecified;
            }
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::FontData getFontData( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::FontData outFontData;
            outFontData.fontFamily = getFontFamily( inAttributes );
            outFontData.style = getFontStyle( inAttributes );
            outFontData.weight = getFontWeight( inAttributes );
            outFontData.sizeType = getFontSize( inAttributes, outFontData.sizePoint, outFontData.sizeCss );
            MX_UNUSED( inAttributes );
            return outFontData;
        }
    }
}
