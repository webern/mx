// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/EncoderAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EncoderAttributes::EncoderAttributes()
        :type()
        ,hasType( false )
        {}


        bool EncoderAttributes::hasValues() const
        {
            return hasType;
        }


        std::ostream& EncoderAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
            }
            return os;
        }


        bool EncoderAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "EncoderAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, hasType, "type" ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
