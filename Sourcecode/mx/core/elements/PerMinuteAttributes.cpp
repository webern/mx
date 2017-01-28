// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PerMinuteAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PerMinuteAttributes::PerMinuteAttributes()
        :fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( FontSize{ CssFontSize::medium } )
        ,fontWeight( FontWeight::normal )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        {}


        bool PerMinuteAttributes::hasValues() const
        {
            return hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight;
        }


        std::ostream& PerMinuteAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
            }
            return os;
        }


        bool PerMinuteAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PerMinuteAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
