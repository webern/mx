// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/ArpeggiateAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ArpeggiateAttributes::ArpeggiateAttributes()
        :number()
        ,direction()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,placement( AboveBelow::below )
        ,hasNumber( false )
        ,hasDirection( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasPlacement( false )
        {}


        bool ArpeggiateAttributes::hasValues() const
        {
            return hasNumber ||
            hasDirection ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasPlacement;
        }


        std::ostream& ArpeggiateAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, direction, "direction", hasDirection );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, placement, "placement", hasPlacement );
            }
            return os;
        }


        bool ArpeggiateAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "ArpeggiateAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, direction, hasDirection, "direction", &parseUpDown ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
            }
        
        
            return isSuccess;
        }

    }
}
