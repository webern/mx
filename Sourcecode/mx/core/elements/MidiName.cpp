// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiName.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiName::MidiName()
        :myValue()
        {}


        MidiName::MidiName( const XsString& value )
        :myValue( value )
        {}


        bool MidiName::hasAttributes() const
        {
            return false;
        }


        bool MidiName::hasContents() const
        {
            return true;
        }


        std::ostream& MidiName::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MidiName::streamName( std::ostream& os ) const
        {
            os << "midi-name";
            return os;
        }


        std::ostream& MidiName::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString MidiName::getValue() const
        {
            return myValue;
        }


        void MidiName::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool MidiName::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
