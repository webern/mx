// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Software.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Software::Software()
        :myValue()
        {}


        Software::Software( const XsString& value )
        :myValue( value )
        {}


        bool Software::hasAttributes() const
        {
            return false;
        }


        bool Software::hasContents() const
        {
            return true;
        }


        std::ostream& Software::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Software::streamName( std::ostream& os ) const
        {
            os << "software";
            return os;
        }


        std::ostream& Software::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString Software::getValue() const
        {
            return myValue;
        }


        void Software::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Software::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
