// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Scoop.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Scoop::Scoop()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyLineAttributes>() )
        {}


        bool Scoop::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Scoop::hasContents() const  { return false; }
        std::ostream& Scoop::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Scoop::streamName( std::ostream& os ) const  { os << "scoop"; return os; }
        std::ostream& Scoop::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyLineAttributesPtr Scoop::getAttributes() const
        {
            return myAttributes;
        }


        void Scoop::setAttributes( const EmptyLineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Scoop::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
