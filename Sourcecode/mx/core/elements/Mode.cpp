// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Mode.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Mode::Mode()
        :myValue()
        {}


        Mode::Mode( const ModeValue& value )
        :myValue( value )
        {}


        bool Mode::hasAttributes() const
        {
            return false;
        }


        bool Mode::hasContents() const
        {
            return true;
        }


        std::ostream& Mode::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Mode::streamName( std::ostream& os ) const
        {
            os << "mode";
            return os;
        }


        std::ostream& Mode::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        ModeValue Mode::getValue() const
        {
            return myValue;
        }


        void Mode::setValue( const ModeValue& value )
        {
            myValue = value;
        }


        bool Mode::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
