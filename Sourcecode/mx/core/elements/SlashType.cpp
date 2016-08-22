// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SlashType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SlashType::SlashType()
        :myValue( NoteTypeValue::eighth )
        {}


        SlashType::SlashType( const NoteTypeValue& value )
        :myValue( value )
        {}


        bool SlashType::hasAttributes() const
        {
            return false;
        }


        bool SlashType::hasContents() const
        {
            return true;
        }


        std::ostream& SlashType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SlashType::streamName( std::ostream& os ) const
        {
            os << "slash-type";
            return os;
        }


        std::ostream& SlashType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteTypeValue SlashType::getValue() const
        {
            return myValue;
        }


        void SlashType::setValue( const NoteTypeValue& value )
        {
            myValue = value;
        }


        bool SlashType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseNoteTypeValue( xelement.getValue() );
            return true;
        }

    }
}
