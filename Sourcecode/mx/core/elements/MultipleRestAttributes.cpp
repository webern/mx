// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MultipleRestAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MultipleRestAttributes::MultipleRestAttributes()
        :useSymbols()
        ,hasUseSymbols( false )
        {}


        bool MultipleRestAttributes::hasValues() const
        {
            return hasUseSymbols;
        }


        std::ostream& MultipleRestAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, useSymbols, "use-symbols", hasUseSymbols );
            }
            return os;
        }


        bool MultipleRestAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "MultipleRestAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, useSymbols, hasUseSymbols, "use-symbols", &parseYesNo ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
