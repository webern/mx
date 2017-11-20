// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PageHeight.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PageHeight::PageHeight()
        :myValue()
        {}


        PageHeight::PageHeight( const TenthsValue& value )
        :myValue( value )
        {}


        bool PageHeight::hasAttributes() const
        {
            return false;
        }


        bool PageHeight::hasContents() const
        {
            return true;
        }


        std::ostream& PageHeight::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PageHeight::streamName( std::ostream& os ) const
        {
            os << "page-height";
            return os;
        }


        std::ostream& PageHeight::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue PageHeight::getValue() const
        {
            return myValue;
        }


        void PageHeight::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool PageHeight::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
