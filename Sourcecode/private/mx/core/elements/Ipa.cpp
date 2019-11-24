// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Ipa.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Ipa::Ipa()
        :myValue()
        {}


        Ipa::Ipa( const XsString& value )
        :myValue( value )
        {}


        bool Ipa::hasAttributes() const
        {
            return false;
        }


        bool Ipa::hasContents() const
        {
            return true;
        }


        std::ostream& Ipa::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Ipa::streamName( std::ostream& os ) const
        {
            os << "ipa";
            return os;
        }


        std::ostream& Ipa::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString Ipa::getValue() const
        {
            return myValue;
        }


        void Ipa::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Ipa::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
