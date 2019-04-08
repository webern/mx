// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PedalAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PedalAlter::PedalAlter()
        :myValue()
        {}


        PedalAlter::PedalAlter( const Semitones& value )
        :myValue( value )
        {}


        bool PedalAlter::hasAttributes() const
        {
            return false;
        }


        bool PedalAlter::hasContents() const
        {
            return true;
        }


        std::ostream& PedalAlter::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PedalAlter::streamName( std::ostream& os ) const
        {
            os << "pedal-alter";
            return os;
        }


        std::ostream& PedalAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Semitones PedalAlter::getValue() const
        {
            return myValue;
        }


        void PedalAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool PedalAlter::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
