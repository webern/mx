// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NormalType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NormalType::NormalType()
        :myValue( NoteTypeValue::eighth )
        {}


        NormalType::NormalType( const NoteTypeValue& value )
        :myValue( value )
        {}


        bool NormalType::hasAttributes() const
        {
            return false;
        }


        bool NormalType::hasContents() const
        {
            return true;
        }


        std::ostream& NormalType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NormalType::streamName( std::ostream& os ) const
        {
            os << "normal-type";
            return os;
        }


        std::ostream& NormalType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteTypeValue NormalType::getValue() const
        {
            return myValue;
        }


        void NormalType::setValue( const NoteTypeValue& value )
        {
            myValue = value;
        }


        bool NormalType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseNoteTypeValue( xelement.getValue() );
            return true;
        }

    }
}
