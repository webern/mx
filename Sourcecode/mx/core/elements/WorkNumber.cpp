// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/WorkNumber.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WorkNumber::WorkNumber()
        :myValue()
        {}


        WorkNumber::WorkNumber( const XsString& value )
        :myValue( value )
        {}


        bool WorkNumber::hasAttributes() const
        {
            return false;
        }


        bool WorkNumber::hasContents() const
        {
            return true;
        }


        std::ostream& WorkNumber::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& WorkNumber::streamName( std::ostream& os ) const
        {
            os << "work-number";
            return os;
        }


        std::ostream& WorkNumber::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString WorkNumber::getValue() const
        {
            return myValue;
        }


        void WorkNumber::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool WorkNumber::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
