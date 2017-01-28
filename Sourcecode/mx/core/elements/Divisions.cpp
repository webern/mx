// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Divisions.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Divisions::Divisions()
        :myValue()
        {}


        Divisions::Divisions( const PositiveDivisionsValue& value )
        :myValue( value )
        {}


        bool Divisions::hasAttributes() const
        {
            return false;
        }


        bool Divisions::hasContents() const
        {
            return true;
        }


        std::ostream& Divisions::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Divisions::streamName( std::ostream& os ) const
        {
            os << "divisions";
            return os;
        }


        std::ostream& Divisions::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PositiveDivisionsValue Divisions::getValue() const
        {
            return myValue;
        }


        void Divisions::setValue( const PositiveDivisionsValue& value )
        {
            myValue = value;
        }


        bool Divisions::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
