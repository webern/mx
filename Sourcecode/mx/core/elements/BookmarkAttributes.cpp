// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BookmarkAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BookmarkAttributes::BookmarkAttributes()
        :id()
        ,name()
        ,element()
        ,position()
        ,hasId( true )
        ,hasName( false )
        ,hasElement( false )
        ,hasPosition( false )
        {}


        bool BookmarkAttributes::hasValues() const
        {
            return hasId ||
            hasName ||
            hasElement ||
            hasPosition;
        }


        std::ostream& BookmarkAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, id, "id", hasId );
                streamAttribute( os, name, "name", hasName );
                streamAttribute( os, element, "element", hasElement );
                streamAttribute( os, position, "position", hasPosition );
            }
            return os;
        }


        bool BookmarkAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "BookmarkAttributes";
            bool isSuccess = true;
            bool isIdFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, id, isIdFound, "id" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, name, hasName, "name" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, element, hasElement, "element" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, position, hasPosition, "position" ) ) { continue; }
            }
        
            if( !isIdFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
