// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Beats.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Beats::Beats()
        :myValue()
        {}


        Beats::Beats( const XsString& value )
        :myValue( value )
        {}


        bool Beats::hasAttributes() const
        {
            return false;
        }


        bool Beats::hasContents() const
        {
            return true;
        }


        std::ostream& Beats::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Beats::streamName( std::ostream& os ) const
        {
            os << "beats";
            return os;
        }


        std::ostream& Beats::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString Beats::getValue() const
        {
            return myValue;
        }


        void Beats::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Beats::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
