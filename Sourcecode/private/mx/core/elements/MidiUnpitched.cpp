// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiUnpitched.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiUnpitched::MidiUnpitched()
        :myValue()
        {}


        MidiUnpitched::MidiUnpitched( const Midi128& value )
        :myValue( value )
        {}


        bool MidiUnpitched::hasAttributes() const
        {
            return false;
        }


        bool MidiUnpitched::hasContents() const
        {
            return true;
        }


        std::ostream& MidiUnpitched::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MidiUnpitched::streamName( std::ostream& os ) const
        {
            os << "midi-unpitched";
            return os;
        }


        std::ostream& MidiUnpitched::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Midi128 MidiUnpitched::getValue() const
        {
            return myValue;
        }


        void MidiUnpitched::setValue( const Midi128& value )
        {
            myValue = value;
        }


        bool MidiUnpitched::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
