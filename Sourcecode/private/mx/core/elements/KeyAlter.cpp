// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/KeyAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KeyAlter::KeyAlter()
        :myValue()
        {}


        KeyAlter::KeyAlter( const Semitones& value )
        :myValue( value )
        {}


        bool KeyAlter::hasAttributes() const
        {
            return false;
        }


        bool KeyAlter::hasContents() const
        {
            return true;
        }


        std::ostream& KeyAlter::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& KeyAlter::streamName( std::ostream& os ) const
        {
            os << "key-alter";
            return os;
        }


        std::ostream& KeyAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Semitones KeyAlter::getValue() const
        {
            return myValue;
        }


        void KeyAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool KeyAlter::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
