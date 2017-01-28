// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PageWidth.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PageWidth::PageWidth()
        :myValue()
        {}


        PageWidth::PageWidth( const TenthsValue& value )
        :myValue( value )
        {}


        bool PageWidth::hasAttributes() const
        {
            return false;
        }


        bool PageWidth::hasContents() const
        {
            return true;
        }


        std::ostream& PageWidth::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PageWidth::streamName( std::ostream& os ) const
        {
            os << "page-width";
            return os;
        }


        std::ostream& PageWidth::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue PageWidth::getValue() const
        {
            return myValue;
        }


        void PageWidth::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool PageWidth::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
