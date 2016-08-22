// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/OtherPercussion.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherPercussion::OtherPercussion()
        :myValue()
        {}


        OtherPercussion::OtherPercussion( const XsString& value )
        :myValue( value )
        {}


        bool OtherPercussion::hasAttributes() const
        {
            return false;
        }


        bool OtherPercussion::hasContents() const
        {
            return true;
        }


        std::ostream& OtherPercussion::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& OtherPercussion::streamName( std::ostream& os ) const
        {
            os << "other-percussion";
            return os;
        }


        std::ostream& OtherPercussion::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString OtherPercussion::getValue() const
        {
            return myValue;
        }


        void OtherPercussion::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherPercussion::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
