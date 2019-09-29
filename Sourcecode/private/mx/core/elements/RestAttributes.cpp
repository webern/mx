// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/RestAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        RestAttributes::RestAttributes()
        :measure( YesNo::no )
        ,hasMeasure( false )
        {}


        bool RestAttributes::hasValues() const
        {
            return hasMeasure;
        }


        std::ostream& RestAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, measure, "measure", hasMeasure );
            }
            return os;
        }


        bool RestAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "RestAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, measure, hasMeasure, "measure", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
