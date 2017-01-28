// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Pitched.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Pitched::Pitched()
        :myValue( PitchedEnum::xylophone )
        {}


        Pitched::Pitched( const PitchedEnum& value )
        :myValue( value )
        {}


        bool Pitched::hasAttributes() const
        {
            return false;
        }


        bool Pitched::hasContents() const
        {
            return true;
        }


        std::ostream& Pitched::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Pitched::streamName( std::ostream& os ) const
        {
            os << "pitched";
            return os;
        }


        std::ostream& Pitched::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PitchedEnum Pitched::getValue() const
        {
            return myValue;
        }


        void Pitched::setValue( const PitchedEnum& value )
        {
            myValue = value;
        }


        bool Pitched::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parsePitchedEnum( xelement.getValue() );
            return true;
        }

    }
}
