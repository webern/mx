// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Staff.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Staff::Staff()
        :myValue()
        {}


        Staff::Staff( const PositiveInteger& value )
        :myValue( value )
        {}


        bool Staff::hasAttributes() const
        {
            return false;
        }


        bool Staff::hasContents() const
        {
            return true;
        }


        std::ostream& Staff::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Staff::streamName( std::ostream& os ) const
        {
            os << "staff";
            return os;
        }


        std::ostream& Staff::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PositiveInteger Staff::getValue() const
        {
            return myValue;
        }


        void Staff::setValue( const PositiveInteger& value )
        {
            myValue = value;
        }


        bool Staff::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
