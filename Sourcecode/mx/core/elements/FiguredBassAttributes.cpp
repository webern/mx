// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/FiguredBassAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FiguredBassAttributes::FiguredBassAttributes()
        :defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::normal )
        ,color()
        ,printObject( YesNo::no )
        ,printDot( YesNo::no )
        ,printSpacing( YesNo::no )
        ,printLyric( YesNo::no )
        ,parentheses( YesNo::no )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasColor( false )
        ,hasPrintObject( false )
        ,hasPrintDot( false )
        ,hasPrintSpacing( false )
        ,hasPrintLyric( false )
        ,hasParentheses( false )
        {}


        bool FiguredBassAttributes::hasValues() const
        {
            return hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasColor ||
            hasPrintObject ||
            hasPrintDot ||
            hasPrintSpacing ||
            hasPrintLyric ||
            hasParentheses;
        }


        std::ostream& FiguredBassAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, printObject, "print-object", hasPrintObject );
                streamAttribute( os, printDot, "print-dot", hasPrintDot );
                streamAttribute( os, printSpacing, "print-spacing", hasPrintSpacing );
                streamAttribute( os, printLyric, "print-lyric", hasPrintLyric );
                streamAttribute( os, parentheses, "parentheses", hasParentheses );
            }
            return os;
        }


        bool FiguredBassAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "FiguredBassAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printDot, hasPrintDot, "print-dot", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printSpacing, hasPrintSpacing, "print-spacing", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printLyric, hasPrintLyric, "print-lyric", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, parentheses, hasParentheses, "parentheses", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
