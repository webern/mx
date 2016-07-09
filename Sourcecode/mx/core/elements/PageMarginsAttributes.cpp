// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PageMarginsAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PageMarginsAttributes::PageMarginsAttributes()
        :type( MarginType::both )
        ,hasType( false )
        {}


        bool PageMarginsAttributes::hasValues() const
        {
            return hasType;
        }


        std::ostream& PageMarginsAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
            }
            return os;
        }


        bool PageMarginsAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PageMarginsAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, hasType, "type", &parseMarginType ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
