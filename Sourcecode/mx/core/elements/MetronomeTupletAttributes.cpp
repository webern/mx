// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MetronomeTupletAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeTupletAttributes::MetronomeTupletAttributes()
        :type( StartStop::start )
        ,bracket( YesNo::no )
        ,showNumber( ShowTuplet::actual )
        ,hasType( true )
        ,hasBracket( false )
        ,hasShowNumber( false )
        {}


        bool MetronomeTupletAttributes::hasValues() const
        {
            return hasType ||
            hasBracket ||
            hasShowNumber;
        }


        std::ostream& MetronomeTupletAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, bracket, "bracket", hasBracket );
                streamAttribute( os, showNumber, "show-number", hasShowNumber );
            }
            return os;
        }


        bool MetronomeTupletAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "MetronomeTupletAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStop ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, bracket, hasBracket, "bracket", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, showNumber, hasShowNumber, "show-number", &parseShowTuplet ) ) { continue; }
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
