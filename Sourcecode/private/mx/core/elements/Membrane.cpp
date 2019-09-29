// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Membrane.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Membrane::Membrane()
        :myValue( MembraneEnum::snareDrum )
        {}


        Membrane::Membrane( const MembraneEnum& value )
        :myValue( value )
        {}


        bool Membrane::hasAttributes() const
        {
            return false;
        }


        bool Membrane::hasContents() const
        {
            return true;
        }


        std::ostream& Membrane::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Membrane::streamName( std::ostream& os ) const
        {
            os << "membrane";
            return os;
        }


        std::ostream& Membrane::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MembraneEnum Membrane::getValue() const
        {
            return myValue;
        }


        void Membrane::setValue( const MembraneEnum& value )
        {
            myValue = value;
        }


        bool Membrane::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parseMembraneEnum( xelement.getValue() );
            return true;
        }

    }
}
