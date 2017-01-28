// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Sign.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Sign::Sign()
        :myValue( ClefSign::g )
        {}


        Sign::Sign( const ClefSign& value )
        :myValue( value )
        {}


        bool Sign::hasAttributes() const
        {
            return false;
        }


        bool Sign::hasContents() const
        {
            return true;
        }


        std::ostream& Sign::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Sign::streamName( std::ostream& os ) const
        {
            os << "sign";
            return os;
        }


        std::ostream& Sign::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        ClefSign Sign::getValue() const
        {
            return myValue;
        }


        void Sign::setValue( const ClefSign& value )
        {
            myValue = value;
        }


        bool Sign::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseClefSign( xelement.getValue() );
            return true;
        }

    }
}
