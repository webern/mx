// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BarlineAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BarlineAttributes::BarlineAttributes()
        :location( RightLeftMiddle::right )
        ,segno()
        ,coda()
        ,divisions()
        ,hasLocation( false )
        ,hasSegno( false )
        ,hasCoda( false )
        ,hasDivisions( false )
        {}


        bool BarlineAttributes::hasValues() const
        {
            return hasLocation ||
            hasSegno ||
            hasCoda ||
            hasDivisions;
        }


        std::ostream& BarlineAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, location, "location", hasLocation );
                streamAttribute( os, segno, "segno", hasSegno );
                streamAttribute( os, coda, "coda", hasCoda );
                streamAttribute( os, divisions, "divisions", hasDivisions );
            }
            return os;
        }


        bool BarlineAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "BarlineAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, location, hasLocation, "location", &parseRightLeftMiddle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, segno, hasSegno, "segno" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, coda, hasCoda, "coda" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, divisions, hasDivisions, "divisions" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
