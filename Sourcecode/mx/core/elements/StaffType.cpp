// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StaffType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffType::StaffType()
        :myValue( StaffTypeEnum::regular )
        {}


        StaffType::StaffType( const StaffTypeEnum& value )
        :myValue( value )
        {}


        bool StaffType::hasAttributes() const
        {
            return false;
        }


        bool StaffType::hasContents() const
        {
            return true;
        }


        std::ostream& StaffType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StaffType::streamName( std::ostream& os ) const
        {
            os << "staff-type";
            return os;
        }


        std::ostream& StaffType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StaffTypeEnum StaffType::getValue() const
        {
            return myValue;
        }


        void StaffType::setValue( const StaffTypeEnum& value )
        {
            myValue = value;
        }


        bool StaffType::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parseStaffTypeEnum( xelement.getValue() );
            return true;
        }

    }
}
