// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Alter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Alter::Alter()
        :myValue()
        {}


        Alter::Alter( const Semitones& value )
        :myValue( value )
        {}


        bool Alter::hasAttributes() const
        {
            return false;
        }


        bool Alter::hasContents() const
        {
            return true;
        }


        std::ostream& Alter::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Alter::streamName( std::ostream& os ) const
        {
            os << "alter";
            return os;
        }


        std::ostream& Alter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Semitones Alter::getValue() const
        {
            return myValue;
        }


        void Alter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool Alter::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
