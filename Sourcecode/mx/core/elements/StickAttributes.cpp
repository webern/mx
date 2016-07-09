// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StickAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StickAttributes::StickAttributes()
        :tip()
        ,hasTip( false )
        {}


        bool StickAttributes::hasValues() const
        {
            return hasTip;
        }


        std::ostream& StickAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, tip, "tip", hasTip );
            }
            return os;
        }


        bool StickAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "StickAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, tip, hasTip, "tip", &parseTipDirection ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
