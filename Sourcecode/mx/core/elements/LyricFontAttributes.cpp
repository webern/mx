// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/LyricFontAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LyricFontAttributes::LyricFontAttributes()
        :number()
        ,name()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::normal )
        ,hasNumber( false )
        ,hasName( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        {}


        bool LyricFontAttributes::hasValues() const
        {
            return hasNumber ||
            hasName ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight;
        }


        std::ostream& LyricFontAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, name, "name", hasName );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
            }
            return os;
        }


        bool LyricFontAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "LyricFontAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, name, hasName, "name" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
