// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/EncodingDate.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EncodingDate::EncodingDate()
        :myValue()
        {}


        EncodingDate::EncodingDate( const Date& value )
        :myValue( value )
        {}


        bool EncodingDate::hasAttributes() const
        {
            return false;
        }


        bool EncodingDate::hasContents() const
        {
            return true;
        }


        std::ostream& EncodingDate::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& EncodingDate::streamName( std::ostream& os ) const
        {
            os << "encoding-date";
            return os;
        }


        std::ostream& EncodingDate::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Date EncodingDate::getValue() const
        {
            return myValue;
        }


        void EncodingDate::setValue( const Date& value )
        {
            myValue = value;
        }


        bool EncodingDate::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
