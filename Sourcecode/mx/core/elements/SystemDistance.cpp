// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SystemDistance.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SystemDistance::SystemDistance()
        :myValue()
        {}


        SystemDistance::SystemDistance( const TenthsValue& value )
        :myValue( value )
        {}


        bool SystemDistance::hasAttributes() const
        {
            return false;
        }


        bool SystemDistance::hasContents() const
        {
            return true;
        }


        std::ostream& SystemDistance::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SystemDistance::streamName( std::ostream& os ) const
        {
            os << "system-distance";
            return os;
        }


        std::ostream& SystemDistance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue SystemDistance::getValue() const
        {
            return myValue;
        }


        void SystemDistance::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool SystemDistance::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }
    }
}
