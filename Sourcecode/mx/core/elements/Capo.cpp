// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Capo.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Capo::Capo()
        :myValue()
        {}


        Capo::Capo( const NonNegativeInteger& value )
        :myValue( value )
        {}


        bool Capo::hasAttributes() const
        {
            return false;
        }


        bool Capo::hasContents() const
        {
            return true;
        }


        std::ostream& Capo::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Capo::streamName( std::ostream& os ) const
        {
            os << "capo";
            return os;
        }


        std::ostream& Capo::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NonNegativeInteger Capo::getValue() const
        {
            return myValue;
        }


        void Capo::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool Capo::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
