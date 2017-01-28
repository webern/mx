// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Millimeters.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Millimeters::Millimeters()
        :myValue()
        {}


        Millimeters::Millimeters( const MillimetersValue& value )
        :myValue( value )
        {}


        bool Millimeters::hasAttributes() const
        {
            return false;
        }


        bool Millimeters::hasContents() const
        {
            return true;
        }


        std::ostream& Millimeters::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Millimeters::streamName( std::ostream& os ) const
        {
            os << "millimeters";
            return os;
        }


        std::ostream& Millimeters::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MillimetersValue Millimeters::getValue() const
        {
            return myValue;
        }


        void Millimeters::setValue( const MillimetersValue& value )
        {
            myValue = value;
        }


        bool Millimeters::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
