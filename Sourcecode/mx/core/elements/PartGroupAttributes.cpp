// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartGroupAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartGroupAttributes::PartGroupAttributes()
        :type( StartStop::start )
        ,number( XsToken( "1" ) )
        ,hasType( true )
        ,hasNumber( false )
        {}


        bool PartGroupAttributes::hasValues() const
        {
            return hasType ||
            hasNumber;
        }


        std::ostream& PartGroupAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, number, "number", hasNumber );
            }
            return os;
        }


        bool PartGroupAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "PartGroupAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
