// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LeftMargin.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LeftMargin::LeftMargin()
        :myValue()
        {}


        LeftMargin::LeftMargin( const TenthsValue& value )
        :myValue( value )
        {}


        bool LeftMargin::hasAttributes() const
        {
            return false;
        }


        bool LeftMargin::hasContents() const
        {
            return true;
        }


        std::ostream& LeftMargin::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& LeftMargin::streamName( std::ostream& os ) const
        {
            os << "left-margin";
            return os;
        }


        std::ostream& LeftMargin::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue LeftMargin::getValue() const
        {
            return myValue;
        }


        void LeftMargin::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool LeftMargin::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
