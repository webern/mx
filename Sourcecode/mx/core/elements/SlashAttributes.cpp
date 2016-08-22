// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SlashAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SlashAttributes::SlashAttributes()
        :type( StartStop::start )
        ,useDots( YesNo::no )
        ,useStems( YesNo::no )
        ,hasType( true )
        ,hasUseDots( false )
        ,hasUseStems( false )
        {}


        bool SlashAttributes::hasValues() const
        {
            return hasType ||
            hasUseDots ||
            hasUseStems;
        }


        std::ostream& SlashAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, useDots, "use-dots", hasUseDots );
                streamAttribute( os, useStems, "use-stems", hasUseStems );
            }
            return os;
        }


        bool SlashAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "SlashAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, useDots, hasUseDots, "use-dots", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, useStems, hasUseStems, "use-stems", &parseYesNo ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
