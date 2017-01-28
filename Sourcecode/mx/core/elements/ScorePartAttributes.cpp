// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScorePartAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScorePartAttributes::ScorePartAttributes()
        :id()
        ,hasId( true )
        {}


        bool ScorePartAttributes::hasValues() const
        {
            return hasId;
        }


        std::ostream& ScorePartAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, id, "id", hasId );
            }
            return os;
        }


        bool ScorePartAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "ScorePartAttributes";
            bool isSuccess = true;
            bool isIdFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, id, isIdFound, "id" ) ) { continue; }
            }
        
            if( !isIdFound )
            {
                isSuccess = false;
                message << className << ": 'id' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
