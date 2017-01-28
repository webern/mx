// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Voice.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Voice::Voice()
        :myValue()
        {}


        Voice::Voice( const XsString& value )
        :myValue( value )
        {}


        bool Voice::hasAttributes() const
        {
            return false;
        }


        bool Voice::hasContents() const
        {
            return true;
        }


        std::ostream& Voice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Voice::streamName( std::ostream& os ) const
        {
            os << "voice";
            return os;
        }


        std::ostream& Voice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString Voice::getValue() const
        {
            return myValue;
        }


        void Voice::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Voice::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
