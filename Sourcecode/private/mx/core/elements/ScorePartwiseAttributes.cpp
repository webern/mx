// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScorePartwiseAttributes.h"
#include "mx/core/FromXElement.h"
#include "ezxml/XElement.h"
#include "ezxml/XAttributeIterator.h"

#include <iostream>

namespace mx
{
    namespace core
    {
        ScorePartwiseAttributes::ScorePartwiseAttributes()
        :version( "3.0" )
        ,hasVersion( false )
        {}


        bool ScorePartwiseAttributes::hasValues() const
        {
            return hasVersion;
        }


        std::ostream& ScorePartwiseAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, version, "version", hasVersion );
            }
            return os;
        }


        bool ScorePartwiseAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "ScorePartwiseAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, version, hasVersion, "version" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
