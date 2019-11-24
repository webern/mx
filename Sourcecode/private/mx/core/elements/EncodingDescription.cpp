// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/EncodingDescription.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EncodingDescription::EncodingDescription()
        :myValue()
        {}


        EncodingDescription::EncodingDescription( const XsString& value )
        :myValue( value )
        {}


        bool EncodingDescription::hasAttributes() const
        {
            return false;
        }


        bool EncodingDescription::hasContents() const
        {
            return true;
        }


        std::ostream& EncodingDescription::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& EncodingDescription::streamName( std::ostream& os ) const
        {
            os << "encoding-description";
            return os;
        }


        std::ostream& EncodingDescription::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString EncodingDescription::getValue() const
        {
            return myValue;
        }


        void EncodingDescription::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool EncodingDescription::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
