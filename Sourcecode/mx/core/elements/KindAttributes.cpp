// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/KindAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KindAttributes::KindAttributes()
        :useSymbols()
        ,text()
        ,stackDegrees()
        ,parenthesesDegrees()
        ,bracketDegrees()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( FontSize{ CssFontSize::medium } )
        ,fontWeight( FontWeight::normal )
        ,halign()
        ,hasUseSymbols( false )
        ,hasText( false )
        ,hasStackDegrees( false )
        ,hasParenthesesDegrees( false )
        ,hasBracketDegrees( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasHalign( false )
        {}


        bool KindAttributes::hasValues() const
        {
            return hasUseSymbols ||
            hasText ||
            hasStackDegrees ||
            hasParenthesesDegrees ||
            hasBracketDegrees ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasHalign;
        }


        std::ostream& KindAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, useSymbols, "use-symbols", hasUseSymbols );
                streamAttribute( os, text, "text", hasText );
                streamAttribute( os, stackDegrees, "stack-degrees", hasStackDegrees );
                streamAttribute( os, parenthesesDegrees, "parentheses-degrees", hasParenthesesDegrees );
                streamAttribute( os, bracketDegrees, "bracket-degrees", hasBracketDegrees );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, halign, "halign", hasHalign );
            }
            return os;
        }


        bool KindAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "KindAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, useSymbols, hasUseSymbols, "use-symbols", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, text, hasText, "text" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, stackDegrees, hasStackDegrees, "stack-degrees", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, parenthesesDegrees, hasParenthesesDegrees, "parentheses-degrees", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bracketDegrees, hasBracketDegrees, "bracket-degrees", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, halign, hasHalign, "halign", &parseLeftCenterRight ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
