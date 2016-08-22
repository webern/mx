// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/LinkAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LinkAttributes::LinkAttributes()
        :href()
        ,type( XlinkType::simple )
        ,role()
        ,title()
        ,show( XlinkShow::replace )
        ,actuate( XlinkActuate::onRequest )
        ,name()
        ,element()
        ,position()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,hasHref( true )
        ,hasType( false )
        ,hasRole( false )
        ,hasTitle( false )
        ,hasShow( false )
        ,hasActuate( false )
        ,hasName( false )
        ,hasElement( false )
        ,hasPosition( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        {}


        bool LinkAttributes::hasValues() const
        {
            return hasHref ||
            hasType ||
            hasRole ||
            hasTitle ||
            hasShow ||
            hasActuate ||
            hasName ||
            hasElement ||
            hasPosition ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY;
        }


        std::ostream& LinkAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, href, "xlink:href", hasHref );
                streamAttribute( os, type, "xlink:type", hasType );
                streamAttribute( os, role, "xlink:role", hasRole );
                streamAttribute( os, title, "xlink:title", hasTitle );
                streamAttribute( os, show, "xlink:show", hasShow );
                streamAttribute( os, actuate, "xlink:actuate", hasActuate );
                streamAttribute( os, name, "name", hasName );
                streamAttribute( os, element, "element", hasElement );
                streamAttribute( os, position, "position", hasPosition );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
            }
            return os;
        }


        bool LinkAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "LinkAttributes";
            bool isSuccess = true;
            bool isHrefFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
            
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, href, isHrefFound, "href" ) ) { continue; }
                
                if( it->getName() == "xlink:type" )
                {
                    hasType = true;
                    continue;
                }
                
                if( parseAttribute( message, it, className, isSuccess, role, hasRole, "role" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, title, hasTitle, "title" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, show, hasShow, "show", &parseXlinkShow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, actuate, hasActuate, "actuate", &parseXlinkActuate ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, name, hasName, "name" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, element, hasElement, "element" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, position, hasPosition, "position" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
            }
        
            if( !isHrefFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
