// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MovementTitle.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MovementTitle::MovementTitle()
        :myValue()
        {}


        MovementTitle::MovementTitle( const XsString& value )
        :myValue( value )
        {}


        bool MovementTitle::hasAttributes() const
        {
            return false;
        }


        bool MovementTitle::hasContents() const
        {
            return true;
        }


        std::ostream& MovementTitle::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MovementTitle::streamName( std::ostream& os ) const
        {
            os << "movement-title";
            return os;
        }


        std::ostream& MovementTitle::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString MovementTitle::getValue() const
        {
            return myValue;
        }


        void MovementTitle::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool MovementTitle::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
