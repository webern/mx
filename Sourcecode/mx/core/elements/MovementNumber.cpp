// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MovementNumber.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MovementNumber::MovementNumber()
        :myValue()
        {}


        MovementNumber::MovementNumber( const XsString& value )
        :myValue( value )
        {}


        bool MovementNumber::hasAttributes() const
        {
            return false;
        }


        bool MovementNumber::hasContents() const
        {
            return true;
        }


        std::ostream& MovementNumber::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MovementNumber::streamName( std::ostream& os ) const
        {
            os << "movement-number";
            return os;
        }


        std::ostream& MovementNumber::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString MovementNumber::getValue() const
        {
            return myValue;
        }


        void MovementNumber::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool MovementNumber::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
