// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatRepeatAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatRepeatAttributes::BeatRepeatAttributes()
        :type( StartStop::start )
        ,slashes( 1 )
        ,useDots( YesNo::no )
        ,hasType( true )
        ,hasSlashes( false )
        ,hasUseDots( false )
        {}


        bool BeatRepeatAttributes::hasValues() const
        {
            return hasType ||
            hasSlashes ||
            hasUseDots;
        }


        std::ostream& BeatRepeatAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, slashes, "slashes", hasSlashes );
                streamAttribute( os, useDots, "use-dots", hasUseDots );
            }
            return os;
        }


        bool BeatRepeatAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "BeatRepeatAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, slashes, hasSlashes, "slashes" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, useDots, hasUseDots, "use-dots", &parseYesNo ) ) { continue; }
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
