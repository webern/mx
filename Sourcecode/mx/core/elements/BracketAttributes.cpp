// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BracketAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BracketAttributes::BracketAttributes()
        :type()
        ,number()
        ,lineEnd( LineEnd::down )
        ,endLength()
        ,lineType()
        ,dashLength()
        ,spaceLength()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,color()
        ,hasType( true )
        ,hasNumber( false )
        ,hasLineEnd( true )
        ,hasEndLength( false )
        ,hasLineType( false )
        ,hasDashLength( false )
        ,hasSpaceLength( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasColor( false )
        {}


        bool BracketAttributes::hasValues() const
        {
            return hasType || hasLineEnd;
        }


        std::ostream& BracketAttributes::toStream( std::ostream& os ) const
        {
            if( !hasValues() )
            {
                return os;
            }
            streamAttribute( os, type, "type", hasType );
            streamAttribute( os, number, "number", hasNumber );
            streamAttribute( os, lineEnd, "line-end", hasLineEnd );
            streamAttribute( os, endLength, "end-length", hasEndLength );
            streamAttribute( os, lineType, "line-type", hasLineType );
            streamAttribute( os, dashLength, "dash-length", hasDashLength );
            streamAttribute( os, spaceLength, "space-length", hasSpaceLength );
            streamAttribute( os, defaultX, "default-x", hasDefaultX );
            streamAttribute( os, defaultY, "default-y", hasDefaultY );
            streamAttribute( os, relativeX, "relative-x", hasRelativeX );
            streamAttribute( os, relativeY, "relative-y", hasRelativeY );
            streamAttribute( os, color, "color", hasColor );
            return os;
        }


        bool BracketAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "BracketAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
            bool isLineEndFound = false;
            
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStopContinue ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineEnd, isLineEndFound, "line-end", &parseLineEnd ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, endLength, hasEndLength, "end-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineType, hasLineType, "line-type", &parseLineType ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dashLength, hasDashLength, "dash-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, spaceLength, hasSpaceLength, "space-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
