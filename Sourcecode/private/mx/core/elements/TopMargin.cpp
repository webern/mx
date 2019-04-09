// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TopMargin.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TopMargin::TopMargin()
        :myValue()
        {}


        TopMargin::TopMargin( const TenthsValue& value )
        :myValue( value )
        {}


        bool TopMargin::hasAttributes() const
        {
            return false;
        }


        bool TopMargin::hasContents() const
        {
            return true;
        }


        std::ostream& TopMargin::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TopMargin::streamName( std::ostream& os ) const
        {
            os << "top-margin";
            return os;
        }


        std::ostream& TopMargin::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue TopMargin::getValue() const
        {
            return myValue;
        }


        void TopMargin::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool TopMargin::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
