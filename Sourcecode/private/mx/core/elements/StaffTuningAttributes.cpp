// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StaffTuningAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffTuningAttributes::StaffTuningAttributes()
        :line()
        ,hasLine( false )
        {}


        bool StaffTuningAttributes::hasValues() const
        {
            return hasLine;
        }


        std::ostream& StaffTuningAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, line, "line", hasLine );
            }
            return os;
        }


        bool StaffTuningAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "StaffTuningAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, line, hasLine, "line" ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
