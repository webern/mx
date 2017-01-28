// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Metal.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Metal::Metal()
        :myValue( MetalEnum::bell )
        {}


        Metal::Metal( const MetalEnum& value )
        :myValue( value )
        {}


        bool Metal::hasAttributes() const
        {
            return false;
        }


        bool Metal::hasContents() const
        {
            return true;
        }


        std::ostream& Metal::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Metal::streamName( std::ostream& os ) const
        {
            os << "metal";
            return os;
        }


        std::ostream& Metal::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MetalEnum Metal::getValue() const
        {
            return myValue;
        }


        void Metal::setValue( const MetalEnum& value )
        {
            myValue = value;
        }


        bool Metal::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseMetalEnum( xelement.getValue() );
            return true;
        }

    }
}
