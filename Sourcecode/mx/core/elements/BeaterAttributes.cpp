// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeaterAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeaterAttributes::BeaterAttributes()
        :tip()
        ,hasTip( false )
        {}


        bool BeaterAttributes::hasValues() const
        {
            return hasTip;
        }


        std::ostream& BeaterAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, tip, "tip", hasTip );
            }
            return os;
        }


        bool BeaterAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "BeaterAttributes";
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
