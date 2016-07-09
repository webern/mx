// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/RepeatAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        RepeatAttributes::RepeatAttributes()
        :direction( BackwardForward::backward )
        ,times()
        ,winged( Winged::none )
        ,hasDirection( true )
        ,hasTimes( false )
        ,hasWinged( false )
        {}


        bool RepeatAttributes::hasValues() const
        {
            return hasDirection ||
            hasTimes ||
            hasWinged;
        }


        std::ostream& RepeatAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, direction, "direction", hasDirection );
                streamAttribute( os, times, "times", hasTimes );
                streamAttribute( os, winged, "winged", hasWinged );
            }
            return os;
        }


        bool RepeatAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "RepeatAttributes";
            bool isSuccess = true;
            bool isDirectionFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, direction, isDirectionFound, "direction", &parseBackwardForward ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, times, hasTimes, "times" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, winged, hasWinged, "winged", &parseWinged ) ) { continue; }
            }
        
            if( !isDirectionFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
