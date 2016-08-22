// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Link.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Link::Link()
        :ElementInterface()
        ,myAttributes( std::make_shared<LinkAttributes>() )
        {}


        bool Link::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Link::hasContents() const  { return false; }
        std::ostream& Link::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Link::streamName( std::ostream& os ) const  { os << "link"; return os; }
        std::ostream& Link::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        LinkAttributesPtr Link::getAttributes() const
        {
            return myAttributes;
        }


        void Link::setAttributes( const LinkAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Link::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
