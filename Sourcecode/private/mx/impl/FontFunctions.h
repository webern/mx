// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/FontData.h"
#include "mx/core/Enums.h"
#include "mx/core/CommaSeparatedText.h"
#include "mx/core/FontSize.h"
#include "mx/impl/Converter.h"
#include "mx/utility/OptionalMembers.h"

namespace mx
{
    namespace impl
    {        
        MX_ATTR_FUNC_OPTIONAL( hasFontFamily, HasFontFamily, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontFamily, FontFamily, core::CommaSeparatedText, core::CommaSeparatedText{} );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontStyle, HasFontStyle, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontStyle, FontStyle, core::FontStyle, core::FontStyle{} );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontSize, HasFontSize, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontSize, FontSize, core::FontSize, core::FontSize{ core::CssFontSize::medium } );
        
        MX_ATTR_FUNC_OPTIONAL( hasFontWeight, HasFontWeight, bool, false );
        MX_ATTR_FUNC_OPTIONAL( fontWeight, FontWeight, core::FontWeight, core::FontWeight::normal );
        
        MX_ATTR_FUNC_OPTIONAL( hasLineThrough, HasLineThrough, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( lineThrough, LineThrough, int, 0 );
        
        MX_ATTR_FUNC_OPTIONAL( hasOverline, HasOverline, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( overline, Overline, int, 0 );
        
        MX_ATTR_FUNC_OPTIONAL( hasUnderline, HasUnderline, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( underline, Underline, int, 0 );

        template <typename ATTRIBUTES_TYPE>
        std::vector<std::string> getFontFamily( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasFontFamily<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                return std::vector<std::string>{};
            }
            
            const auto& fontFamily = checkFontFamily<ATTRIBUTES_TYPE>( &inAttributes );
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
            if( !checkHasFontStyle<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                return api::FontStyle::unspecified;
            }
            Converter converter;
            return converter.convert( checkFontStyle<ATTRIBUTES_TYPE>( &inAttributes ) );
        }
        
        
        template <typename ATTRIBUTES_TYPE>
        api::FontWeight getFontWeight( const ATTRIBUTES_TYPE& inAttributes )
        {
            if( !checkHasFontWeight<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                return api::FontWeight::unspecified;
            }
            
            Converter converter;
            return converter.convert( checkFontWeight<ATTRIBUTES_TYPE>( &inAttributes ) );
        }
        

        template <typename ATTRIBUTES_TYPE>
        api::FontSizeType getFontSize(
            const ATTRIBUTES_TYPE& inAttributes,
            core::DecimalType& outPointSize,
            api::CssSize& outCssSize )
        {
            if( !checkHasFontSize<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outPointSize = -1.0L;
                outCssSize = api::CssSize::unspecified;
                return api::FontSizeType::unspecified;
            }
            
            const auto& coreFontSize = checkFontSize<ATTRIBUTES_TYPE>( &inAttributes );
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
                return api::FontSizeType::point;
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
            
            if( checkHasLineThrough<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outFontData.lineThrough = checkLineThrough<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            if( checkHasOverline<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outFontData.overline = checkOverline<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            if( checkHasUnderline<ATTRIBUTES_TYPE>( &inAttributes ) )
            {
                outFontData.underline = checkUnderline<ATTRIBUTES_TYPE>( &inAttributes );
            }
            
            return outFontData;
        }

        MX_ATTR_SETFUNC_OPTIONAL( hasFontFamily, HasFontFamily, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( fontFamily, FontFamily, core::CommaSeparatedText, core::CommaSeparatedText{} );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasFontStyle, HasFontStyle, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( fontStyle, FontStyle, core::FontStyle, core::FontStyle{} );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasFontSize, HasFontSize, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( fontSize, FontSize, core::FontSize, core::FontSize{ core::CssFontSize::medium } );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasFontWeight, HasFontWeight, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL( fontWeight, FontWeight, core::FontWeight, core::FontWeight::normal );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasLineThrough, HasLineThrough, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( lineThrough, LineThrough, int, 0 );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasOverline, HasOverline, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( overline, Overline, int, 0 );
        
        MX_ATTR_SETFUNC_OPTIONAL( hasUnderline, HasUnderline, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( underline, Underline, int, 0 );

        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromFontFamily( const std::vector<std::string>& fontFamilyData, ATTRIBUTES_TYPE& outAttributes )
        {
            if( fontFamilyData.empty() )
            {
                lookForAndSetHasFontFamily( false, &outAttributes );
                return;
            }
            
            core::XsTokenSet tokens;
            for( const auto& s : fontFamilyData )
            {
                tokens.emplace_back( s );
            }
            core::CommaSeparatedText csv;
            csv.setValues( tokens );
            lookForAndSetHasFontFamily( true, &outAttributes );
            lookForAndSetFontFamily( csv, &outAttributes );
        }
        
        
        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromFontStyle( api::FontStyle value, ATTRIBUTES_TYPE& outAttributes )
        {
            Converter converter;
            if( value == api::FontStyle::unspecified )
            {
                lookForAndSetHasFontStyle( false, &outAttributes );
                return;
            }
            lookForAndSetHasFontStyle( true, &outAttributes );
            lookForAndSetFontStyle( converter.convert( value ), &outAttributes );
        }
        
        
        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromFontWeight( api::FontWeight value, ATTRIBUTES_TYPE& outAttributes )
        {
            Converter converter;
            if( value == api::FontWeight::unspecified )
            {
                lookForAndSetHasFontWeight( false, &outAttributes );
                return;
            }
            lookForAndSetHasFontWeight( true, &outAttributes );
            lookForAndSetFontWeight( converter.convert( value ), &outAttributes );
        }
        
        
        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromFontSize( const api::FontData& value, ATTRIBUTES_TYPE& outAttributes )
        {
            Converter converter;
            
            if( value.sizeType == api::FontSizeType::unspecified )
            {
                lookForAndSetHasFontSize( false, &outAttributes );
                return;
            }
            
            lookForAndSetHasFontSize( true, &outAttributes );
            core::FontSize fontSize;
            
            if( value.sizeType == api::FontSizeType::css )
            {
                fontSize.setValue( converter.convert( value.sizeCss ) );
            }
            else if( value.sizeType == api::FontSizeType::point )
            {
                fontSize.setValue( core::PositiveDecimal{ value.sizePoint } );
            }
            
            lookForAndSetFontSize( fontSize, &outAttributes );
        }
        
        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromFontData( const api::FontData& value, ATTRIBUTES_TYPE& outAttributes )
        {
            setAttributesFromFontSize( value, outAttributes );
            setAttributesFromFontWeight( value.weight, outAttributes );
            setAttributesFromFontStyle( value.style, outAttributes );
            setAttributesFromFontFamily( value.fontFamily, outAttributes );
            
            if( value.lineThrough > 0 )
            {
                lookForAndSetHasLineThrough( true, &outAttributes );
                lookForAndSetLineThrough( value.lineThrough, &outAttributes );
            }
            else
            {
                lookForAndSetHasLineThrough( false, &outAttributes );
                lookForAndSetLineThrough( value.lineThrough, &outAttributes );
            }
            
            if( value.overline > 0 )
            {
                lookForAndSetHasOverline( true, &outAttributes );
                lookForAndSetOverline( value.overline, &outAttributes );
            }
            else
            {
                lookForAndSetHasOverline( false, &outAttributes );
                lookForAndSetOverline( value.overline, &outAttributes );
            }
            
            if( value.underline > 0 )
            {
                lookForAndSetHasUnderline( true, &outAttributes );
                lookForAndSetUnderline( value.underline, &outAttributes );
            }
            else
            {
                lookForAndSetHasUnderline( false, &outAttributes );
                lookForAndSetUnderline( value.underline, &outAttributes );
            }
        }
    }
}
