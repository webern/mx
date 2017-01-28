// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FirstFretAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FirstFretAttributes::FirstFretAttributes()
        :text()
        ,location()
        ,hasText( false )
        ,hasLocation( false )
        {}


        bool FirstFretAttributes::hasValues() const
        {
            return hasText ||
            hasLocation;
        }


        std::ostream& FirstFretAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, text, "text", hasText );
                streamAttribute( os, location, "location", hasLocation );
            }
            return os;
        }


        bool FirstFretAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "FirstFretAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, text, hasText, "text" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, location, hasLocation, "location", &parseLeftRight ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
