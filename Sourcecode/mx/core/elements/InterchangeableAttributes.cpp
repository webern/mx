// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InterchangeableAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InterchangeableAttributes::InterchangeableAttributes()
        :symbol()
        ,separator()
        ,hasSymbol( false )
        ,hasSeparator( false )
        {}


        bool InterchangeableAttributes::hasValues() const
        {
            return hasSymbol ||
            hasSeparator;
        }


        std::ostream& InterchangeableAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, symbol, "symbol", hasSymbol );
                streamAttribute( os, separator, "separator", hasSeparator );
            }
            return os;
        }


        bool InterchangeableAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "InterchangeableAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, symbol, hasSymbol, "symbol", &parseTimeSymbol ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, separator, hasSeparator, "separator", &parseTimeSeparator ) ) { continue; }
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
