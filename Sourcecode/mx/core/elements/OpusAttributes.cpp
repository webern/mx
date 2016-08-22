// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/OpusAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OpusAttributes::OpusAttributes()
        :href()
        ,type( XlinkType::simple )
        ,role()
        ,title()
        ,show( XlinkShow::replace )
        ,actuate( XlinkActuate::onRequest )
        ,hasHref( true )
        ,hasType( false )
        ,hasRole( false )
        ,hasTitle( false )
        ,hasShow( false )
        ,hasActuate( false )
        {}


        bool OpusAttributes::hasValues() const
        {
            return hasHref ||
            hasType ||
            hasRole ||
            hasTitle ||
            hasShow ||
            hasActuate;
        }


        std::ostream& OpusAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, href, "xlink:href", hasHref );
                streamAttribute( os, type, "xlink:type", hasType );
                streamAttribute( os, role, "xlink:role", hasRole );
                streamAttribute( os, title, "xlink:title", hasTitle );
                streamAttribute( os, show, "xlink:show", hasShow );
                streamAttribute( os, actuate, "xlink:actuate", hasActuate );
            }
            return os;
        }


        bool OpusAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "OpusAttributes";
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
