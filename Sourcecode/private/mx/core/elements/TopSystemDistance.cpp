// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TopSystemDistance::TopSystemDistance()
        :myValue()
        {}


        TopSystemDistance::TopSystemDistance( const TenthsValue& value )
        :myValue( value )
        {}


        bool TopSystemDistance::hasAttributes() const
        {
            return false;
        }


        bool TopSystemDistance::hasContents() const
        {
            return true;
        }


        std::ostream& TopSystemDistance::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TopSystemDistance::streamName( std::ostream& os ) const
        {
            os << "top-system-distance";
            return os;
        }


        std::ostream& TopSystemDistance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue TopSystemDistance::getValue() const
        {
            return myValue;
        }


        void TopSystemDistance::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool TopSystemDistance::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
