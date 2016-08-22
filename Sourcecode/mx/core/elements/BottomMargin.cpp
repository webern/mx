// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BottomMargin.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BottomMargin::BottomMargin()
        :myValue()
        {}


        BottomMargin::BottomMargin( const TenthsValue& value )
        :myValue( value )
        {}


        bool BottomMargin::hasAttributes() const
        {
            return false;
        }


        bool BottomMargin::hasContents() const
        {
            return true;
        }


        std::ostream& BottomMargin::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BottomMargin::streamName( std::ostream& os ) const
        {
            os << "bottom-margin";
            return os;
        }


        std::ostream& BottomMargin::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue BottomMargin::getValue() const
        {
            return myValue;
        }


        void BottomMargin::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool BottomMargin::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
