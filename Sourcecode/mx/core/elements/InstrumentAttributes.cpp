// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/InstrumentAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InstrumentAttributes::InstrumentAttributes()
        :id()
        ,hasId( true )
        {}


        bool InstrumentAttributes::hasValues() const
        {
            return hasId;
        }


        std::ostream& InstrumentAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, id, "id", hasId );
            }
            return os;
        }


        bool InstrumentAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "InstrumentAttributes";
            bool isSuccess = true;
            bool isIdFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, id, isIdFound, "id" ) ) { continue; }
            }
        
            if( !isIdFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
