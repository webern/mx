// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Line.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Line::Line()
        :myValue()
        {}


        Line::Line( const StaffLine& value )
        :myValue( value )
        {}


        bool Line::hasAttributes() const
        {
            return false;
        }


        bool Line::hasContents() const
        {
            return true;
        }


        std::ostream& Line::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Line::streamName( std::ostream& os ) const
        {
            os << "line";
            return os;
        }


        std::ostream& Line::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StaffLine Line::getValue() const
        {
            return myValue;
        }


        void Line::setValue( const StaffLine& value )
        {
            myValue = value;
        }


        bool Line::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
