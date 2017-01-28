// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Fifths.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Fifths::Fifths()
        :myValue{ 0 }
        {}


        Fifths::Fifths( const FifthsValue& value )
        :myValue( value )
        {}


        bool Fifths::hasAttributes() const
        {
            return false;
        }


        bool Fifths::hasContents() const
        {
            return true;
        }


        std::ostream& Fifths::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Fifths::streamName( std::ostream& os ) const
        {
            os << "fifths";
            return os;
        }


        std::ostream& Fifths::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FifthsValue Fifths::getValue() const
        {
            return myValue;
        }


        void Fifths::setValue( const FifthsValue& value )
        {
            myValue = value;
        }


        bool Fifths::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
