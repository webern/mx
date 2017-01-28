// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Mute.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Mute::Mute()
        :myValue( MuteEnum::off )
        {}


        Mute::Mute( const MuteEnum& value )
        :myValue( value )
        {}


        bool Mute::hasAttributes() const
        {
            return false;
        }


        bool Mute::hasContents() const
        {
            return true;
        }


        std::ostream& Mute::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Mute::streamName( std::ostream& os ) const
        {
            os << "mute";
            return os;
        }


        std::ostream& Mute::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MuteEnum Mute::getValue() const
        {
            return myValue;
        }


        void Mute::setValue( const MuteEnum& value )
        {
            myValue = value;
        }


        bool Mute::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseMuteEnum( xelement.getValue() );
            return true;
        }

    }
}
