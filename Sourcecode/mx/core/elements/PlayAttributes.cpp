// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PlayAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PlayAttributes::PlayAttributes()
        :id()
        ,hasId( false )
        {}


        bool PlayAttributes::hasValues() const
        {
            return hasId;
        }


        std::ostream& PlayAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, id, "id", hasId );
            }
            return os;
        }


        bool PlayAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PlayAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, id, hasId, "id" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
