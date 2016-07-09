// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TypeAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TypeAttributes::TypeAttributes()
        :size()
        ,hasSize( false )
        {}


        bool TypeAttributes::hasValues() const
        {
            return hasSize;
        }


        std::ostream& TypeAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, size, "size", hasSize );
            }
            return os;
        }


        bool TypeAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "TypeAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, size, hasSize, "size", &parseSymbolSize ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
