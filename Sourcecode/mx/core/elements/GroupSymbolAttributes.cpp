// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupSymbolAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupSymbolAttributes::GroupSymbolAttributes()
        :defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        {}


        bool GroupSymbolAttributes::hasValues() const
        {
            return hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY;
        }


        std::ostream& GroupSymbolAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
            }
            return os;
        }


        bool GroupSymbolAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "GroupSymbolAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
