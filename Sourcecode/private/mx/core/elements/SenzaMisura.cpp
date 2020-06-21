// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SenzaMisura.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SenzaMisura::SenzaMisura()
        :myValue()
        {}


        SenzaMisura::SenzaMisura( const XsString& value )
        :myValue( value )
        {}


        bool SenzaMisura::hasAttributes() const
        {
            return false;
        }


        bool SenzaMisura::hasContents() const
        {
            return true;
        }


        std::ostream& SenzaMisura::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SenzaMisura::streamName( std::ostream& os ) const
        {
            os << "senza-misura";
            return os;
        }


        std::ostream& SenzaMisura::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString SenzaMisura::getValue() const
        {
            return myValue;
        }


        void SenzaMisura::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool SenzaMisura::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
