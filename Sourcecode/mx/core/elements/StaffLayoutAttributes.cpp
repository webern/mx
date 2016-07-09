// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StaffLayoutAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffLayoutAttributes::StaffLayoutAttributes()
        :number()
        ,hasNumber( false )
        {}


        bool StaffLayoutAttributes::hasValues() const
        {
            return hasNumber;
        }


        std::ostream& StaffLayoutAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
            }
            return os;
        }


        bool StaffLayoutAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "StaffLayoutAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
