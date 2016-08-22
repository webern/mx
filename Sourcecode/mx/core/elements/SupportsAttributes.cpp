// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SupportsAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SupportsAttributes::SupportsAttributes()
        :type( YesNo::no )
        ,element()
        ,attribute()
        ,value()
        ,hasType( true )
        ,hasElement( true )
        ,hasAttribute( false )
        ,hasValue( false )
        {}


        bool SupportsAttributes::hasValues() const
        {
            return hasType ||
            hasElement ||
            hasAttribute ||
            hasValue;
        }


        std::ostream& SupportsAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, element, "element", hasElement );
                streamAttribute( os, attribute, "attribute", hasAttribute );
                streamAttribute( os, value, "value", hasValue );
            }
            return os;
        }


        bool SupportsAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "SupportsAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
            bool isElementFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, element, isElementFound, "element" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, attribute, hasAttribute, "attribute" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, value, hasValue, "value" ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
            if( !isElementFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
