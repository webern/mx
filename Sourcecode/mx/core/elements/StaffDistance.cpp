// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StaffDistance.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffDistance::StaffDistance()
        :myValue()
        {}


        StaffDistance::StaffDistance( const TenthsValue& value )
        :myValue( value )
        {}


        bool StaffDistance::hasAttributes() const
        {
            return false;
        }


        bool StaffDistance::hasContents() const
        {
            return true;
        }


        std::ostream& StaffDistance::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StaffDistance::streamName( std::ostream& os ) const
        {
            os << "staff-distance";
            return os;
        }


        std::ostream& StaffDistance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TenthsValue StaffDistance::getValue() const
        {
            return myValue;
        }


        void StaffDistance::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool StaffDistance::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
