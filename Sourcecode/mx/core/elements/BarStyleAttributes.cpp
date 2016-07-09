// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BarStyleAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BarStyleAttributes::BarStyleAttributes()
        :color()
        ,hasColor( false )
        {}


        bool BarStyleAttributes::hasValues() const
        {
            return hasColor;
        }


        std::ostream& BarStyleAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, color, "color", hasColor );
            }
            return os;
        }


        bool BarStyleAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "BarStyleAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
