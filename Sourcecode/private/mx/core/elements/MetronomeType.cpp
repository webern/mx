// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MetronomeType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeType::MetronomeType()
        :myValue( NoteTypeValue::eighth )
        {}


        MetronomeType::MetronomeType( const NoteTypeValue& value )
        :myValue( value )
        {}


        bool MetronomeType::hasAttributes() const
        {
            return false;
        }


        bool MetronomeType::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MetronomeType::streamName( std::ostream& os ) const
        {
            os << "metronome-type";
            return os;
        }


        std::ostream& MetronomeType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteTypeValue MetronomeType::getValue() const
        {
            return myValue;
        }


        void MetronomeType::setValue( const NoteTypeValue& value )
        {
            myValue = value;
        }


        bool MetronomeType::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseNoteTypeValue( xelement.getValue() );
            return true;
        }

    }
}
