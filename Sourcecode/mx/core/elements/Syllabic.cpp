// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Syllabic.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Syllabic::Syllabic()
        :myValue( SyllabicEnum::begin )
        {}


        Syllabic::Syllabic( const SyllabicEnum& value )
        :myValue( value )
        {}


        bool Syllabic::hasAttributes() const
        {
            return false;
        }


        bool Syllabic::hasContents() const
        {
            return true;
        }


        std::ostream& Syllabic::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Syllabic::streamName( std::ostream& os ) const
        {
            os << "syllabic";
            return os;
        }


        std::ostream& Syllabic::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        SyllabicEnum Syllabic::getValue() const
        {
            return myValue;
        }


        void Syllabic::setValue( const SyllabicEnum& value )
        {
            myValue = value;
        }


        bool Syllabic::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parseSyllabicEnum( xelement.getValue() );
            return true;
        }

    }
}
