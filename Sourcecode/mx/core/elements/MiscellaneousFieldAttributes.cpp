// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MiscellaneousFieldAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MiscellaneousFieldAttributes::MiscellaneousFieldAttributes()
        :name()
        ,hasName( true )
        {}


        bool MiscellaneousFieldAttributes::hasValues() const
        {
            return hasName;
        }


        std::ostream& MiscellaneousFieldAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, name, "name", hasName );
            }
            return os;
        }


        bool MiscellaneousFieldAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "MiscellaneousFieldAttributes";
            bool isSuccess = true;
            bool isNameFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, name, isNameFound, "name" ) ) { continue; }
            }
        
            if( !isNameFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
