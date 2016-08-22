// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/HoleClosedAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HoleClosedAttributes::HoleClosedAttributes()
        :location( HoleClosedLocation::top )
        ,hasLocation( false )
        {}


        bool HoleClosedAttributes::hasValues() const
        {
            return hasLocation;
        }


        std::ostream& HoleClosedAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, location, "location", hasLocation );
            }
            return os;
        }


        bool HoleClosedAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "HoleClosedAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, location, hasLocation, "location", &parseHoleClosedLocation ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
