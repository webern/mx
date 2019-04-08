// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Octave.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Octave::Octave()
        :myValue()
        {}


        Octave::Octave( const OctaveValue& value )
        :myValue( value )
        {}


        bool Octave::hasAttributes() const
        {
            return false;
        }


        bool Octave::hasContents() const
        {
            return true;
        }


        std::ostream& Octave::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Octave::streamName( std::ostream& os ) const
        {
            os << "octave";
            return os;
        }


        std::ostream& Octave::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OctaveValue Octave::getValue() const
        {
            return myValue;
        }


        void Octave::setValue( const OctaveValue& value )
        {
            myValue = value;
        }


        bool Octave::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
