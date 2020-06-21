// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Pan.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Pan::Pan()
        :myValue()
        {}


        Pan::Pan( const RotationDegrees& value )
        :myValue( value )
        {}


        bool Pan::hasAttributes() const
        {
            return false;
        }


        bool Pan::hasContents() const
        {
            return true;
        }


        std::ostream& Pan::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Pan::streamName( std::ostream& os ) const
        {
            os << "pan";
            return os;
        }


        std::ostream& Pan::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RotationDegrees Pan::getValue() const
        {
            return myValue;
        }


        void Pan::setValue( const RotationDegrees& value )
        {
            myValue = value;
        }


        bool Pan::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
