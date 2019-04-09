// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TieAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TieAttributes::TieAttributes()
        :type( StartStop::start )
        ,timeOnly()
        ,hasType( true )
        ,hasTimeOnly( false )
        {}


        bool TieAttributes::hasValues() const
        {
            return hasType ||
            hasTimeOnly;
        }


        std::ostream& TieAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, timeOnly, "time-only", hasTimeOnly );
            }
            return os;
        }


        bool TieAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "TieAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, timeOnly, hasTimeOnly, "time-only" ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
