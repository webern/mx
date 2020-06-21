// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiChannel.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiChannel::MidiChannel()
        :myValue()
        {}


        MidiChannel::MidiChannel( const Midi16& value )
        :myValue( value )
        {}


        bool MidiChannel::hasAttributes() const
        {
            return false;
        }


        bool MidiChannel::hasContents() const
        {
            return true;
        }


        std::ostream& MidiChannel::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MidiChannel::streamName( std::ostream& os ) const
        {
            os << "midi-channel";
            return os;
        }


        std::ostream& MidiChannel::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Midi16 MidiChannel::getValue() const
        {
            return myValue;
        }


        void MidiChannel::setValue( const Midi16& value )
        {
            myValue = value;
        }


        bool MidiChannel::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
