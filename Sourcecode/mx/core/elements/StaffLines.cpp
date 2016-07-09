// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StaffLines.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffLines::StaffLines()
        :myValue()
        {}


        StaffLines::StaffLines( const NonNegativeInteger& value )
        :myValue( value )
        {}


        bool StaffLines::hasAttributes() const
        {
            return false;
        }


        bool StaffLines::hasContents() const
        {
            return true;
        }


        std::ostream& StaffLines::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StaffLines::streamName( std::ostream& os ) const
        {
            os << "staff-lines";
            return os;
        }


        std::ostream& StaffLines::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NonNegativeInteger StaffLines::getValue() const
        {
            return myValue;
        }


        void StaffLines::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool StaffLines::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
