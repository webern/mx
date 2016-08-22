// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Instruments.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Instruments::Instruments()
        :myValue()
        {}


        Instruments::Instruments( const NonNegativeInteger& value )
        :myValue( value )
        {}


        bool Instruments::hasAttributes() const
        {
            return false;
        }


        bool Instruments::hasContents() const
        {
            return true;
        }


        std::ostream& Instruments::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Instruments::streamName( std::ostream& os ) const
        {
            os << "instruments";
            return os;
        }


        std::ostream& Instruments::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NonNegativeInteger Instruments::getValue() const
        {
            return myValue;
        }


        void Instruments::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool Instruments::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
