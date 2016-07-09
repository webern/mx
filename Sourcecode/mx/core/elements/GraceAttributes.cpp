// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/GraceAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GraceAttributes::GraceAttributes()
        :stealTimePrevious()
        ,stealTimeFollowing()
        ,makeTime()
        ,slash( YesNo::no )
        ,hasStealTimePrevious( false )
        ,hasStealTimeFollowing( false )
        ,hasMakeTime( false )
        ,hasSlash( false )
        {}


        bool GraceAttributes::hasValues() const
        {
            return hasStealTimePrevious ||
            hasStealTimeFollowing ||
            hasMakeTime ||
            hasSlash;
        }


        std::ostream& GraceAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, stealTimePrevious, "steal-time-previous", hasStealTimePrevious );
                streamAttribute( os, stealTimeFollowing, "steal-time-following", hasStealTimeFollowing );
                streamAttribute( os, makeTime, "make-time", hasMakeTime );
                streamAttribute( os, slash, "slash", hasSlash );
            }
            return os;
        }


        bool GraceAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "GraceAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, stealTimePrevious, hasStealTimePrevious, "steal-time-previous" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, stealTimeFollowing, hasStealTimeFollowing, "steal-time-following" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, makeTime, hasMakeTime, "make-time" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, slash, hasSlash, "slash", &parseYesNo ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
