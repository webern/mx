// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InstrumentSound.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InstrumentSound::InstrumentSound()
        :myValue( PlaybackSound::keyboardPiano )
        {}


        InstrumentSound::InstrumentSound( const PlaybackSound& value )
        :myValue( value )
        {}


        bool InstrumentSound::hasAttributes() const
        {
            return false;
        }


        bool InstrumentSound::hasContents() const
        {
            return true;
        }


        std::ostream& InstrumentSound::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& InstrumentSound::streamName( std::ostream& os ) const
        {
            os << "instrument-sound";
            return os;
        }


        std::ostream& InstrumentSound::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << PlaybackSoundToString( myValue );
            return os;
        }


        PlaybackSound InstrumentSound::getValue() const
        {
            return myValue;
        }


        void InstrumentSound::setValue( const PlaybackSound& value )
        {
            myValue = value;
        }


        bool InstrumentSound::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = ( PlaybackSoundFromString( xelement.getValue() ) );
            return true;
        }

    }
}
