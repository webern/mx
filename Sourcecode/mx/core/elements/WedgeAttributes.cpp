// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/WedgeAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WedgeAttributes::WedgeAttributes()
        :type( WedgeType::crescendo )
        ,number()
        ,lineType( LineType::solid )
        ,spread()
        ,niente( YesNo::no )
        ,dashLength()
        ,spaceLength()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,hasType( true )
        ,hasNumber( false )
        ,hasLineType( false )
        ,hasSpread( false )
        ,hasNiente( false )
        ,hasDashLength( false )
        ,hasSpaceLength( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        {}


        bool WedgeAttributes::hasValues() const
        {
            return hasType ||
            hasNumber ||
            hasLineType ||
            hasSpread ||
            hasNiente ||
            hasDashLength ||
            hasSpaceLength ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY;
        }


        std::ostream& WedgeAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, lineType, "line-type", hasLineType );
                streamAttribute( os, spread, "spread", hasSpread );
                streamAttribute( os, niente, "niente", hasNiente );
                streamAttribute( os, dashLength, "dash-length", hasDashLength );
                streamAttribute( os, spaceLength, "space-length", hasSpaceLength );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
            }
            return os;
        }


        bool WedgeAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "WedgeAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseWedgeType ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lineType, hasLineType, "line-type", &parseLineType ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, spread, hasSpread, "spread" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, niente, hasNiente, "niente", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dashLength, hasDashLength, "dash-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, spaceLength, hasSpaceLength, "space-length" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
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
