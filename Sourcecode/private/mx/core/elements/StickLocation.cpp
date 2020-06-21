// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StickLocation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StickLocation::StickLocation()
        :myValue( StickLocationEnum::center )
        {}


        StickLocation::StickLocation( const StickLocationEnum& value )
        :myValue( value )
        {}


        bool StickLocation::hasAttributes() const
        {
            return false;
        }


        bool StickLocation::hasContents() const
        {
            return true;
        }


        std::ostream& StickLocation::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StickLocation::streamName( std::ostream& os ) const
        {
            os << "stick-location";
            return os;
        }


        std::ostream& StickLocation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StickLocationEnum StickLocation::getValue() const
        {
            return myValue;
        }


        void StickLocation::setValue( const StickLocationEnum& value )
        {
            myValue = value;
        }


        bool StickLocation::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseStickLocationEnum( xelement.getValue() );
            return true;
        }

    }
}
