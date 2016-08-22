// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TimeAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TimeAttributes::TimeAttributes()
        :number()
        ,symbol()
        ,separator()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::normal )
        ,color()
        ,halign( LeftCenterRight::center )
        ,valign()
        ,printObject( YesNo::no )
        ,hasNumber( false )
        ,hasSymbol( false )
        ,hasSeparator( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasColor( false )
        ,hasHalign( false )
        ,hasValign( false )
        ,hasPrintObject( false )
        {}


        bool TimeAttributes::hasValues() const
        {
            return hasNumber ||
            hasSymbol ||
            hasSeparator ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasColor ||
            hasHalign ||
            hasValign ||
            hasPrintObject;
        }


        std::ostream& TimeAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, symbol, "symbol", hasSymbol );
                streamAttribute( os, separator, "separator", hasSeparator );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, halign, "halign", hasHalign );
                streamAttribute( os, valign, "valign", hasValign );
                streamAttribute( os, printObject, "print-object", hasPrintObject );
            }
            return os;
        }


        bool TimeAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "TimeAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, symbol, hasSymbol, "symbol", &parseTimeSymbol ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, separator, hasSeparator, "separator", &parseTimeSeparator ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, halign, hasHalign, "halign", &parseLeftCenterRight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, valign, hasValign, "valign", &parseValign ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, printObject, hasPrintObject, "print-object", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
