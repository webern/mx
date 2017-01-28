// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FrameAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FrameAttributes::FrameAttributes()
        :defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,color()
        ,halign( LeftCenterRight::center )
        ,valign()
        ,height()
        ,width()
        ,unplayed()
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasColor( false )
        ,hasHalign( false )
        ,hasValign( false )
        ,hasHeight( false )
        ,hasWidth( false )
        ,hasUnplayed( false )
        {}


        bool FrameAttributes::hasValues() const
        {
            return hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasColor ||
            hasHalign ||
            hasValign ||
            hasHeight ||
            hasWidth ||
            hasUnplayed;
        }


        std::ostream& FrameAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, halign, "halign", hasHalign );
                streamAttribute( os, valign, "valign", hasValign );
                streamAttribute( os, height, "height", hasHeight );
                streamAttribute( os, width, "width", hasWidth );
                streamAttribute( os, unplayed, "unplayed", hasUnplayed );
            }
            return os;
        }


        bool FrameAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "FrameAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, halign, hasHalign, "halign", &parseLeftCenterRight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, valign, hasValign, "valign", &parseValignImage ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, height, hasHeight, "height" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, width, hasWidth, "width" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, unplayed, hasUnplayed, "unplayed" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
