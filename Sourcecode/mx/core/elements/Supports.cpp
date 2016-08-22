// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Supports.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Supports::Supports()
        :ElementInterface()
        ,myAttributes( std::make_shared<SupportsAttributes>() )
        {}


        bool Supports::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Supports::hasContents() const  { return false; }
        std::ostream& Supports::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Supports::streamName( std::ostream& os ) const  { os << "supports"; return os; }
        std::ostream& Supports::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        SupportsAttributesPtr Supports::getAttributes() const
        {
            return myAttributes;
        }


        void Supports::setAttributes( const SupportsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Supports::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
