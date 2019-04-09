// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Diatonic.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Diatonic::Diatonic()
        :myValue()
        {}


        Diatonic::Diatonic( const Integer& value )
        :myValue( value )
        {}


        bool Diatonic::hasAttributes() const
        {
            return false;
        }


        bool Diatonic::hasContents() const
        {
            return true;
        }


        std::ostream& Diatonic::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Diatonic::streamName( std::ostream& os ) const
        {
            os << "diatonic";
            return os;
        }


        std::ostream& Diatonic::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Integer Diatonic::getValue() const
        {
            return myValue;
        }


        void Diatonic::setValue( const Integer& value )
        {
            myValue = value;
        }


        bool Diatonic::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
