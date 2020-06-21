// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Duration.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Duration::Duration()
        :myValue(1.0)
        {}


        Duration::Duration( const PositiveDivisionsValue& value )
        :myValue( value )
        {}


        bool Duration::hasAttributes() const
        {
            return false;
        }


        bool Duration::hasContents() const
        {
            return true;
        }


        std::ostream& Duration::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Duration::streamName( std::ostream& os ) const
        {
            os << "duration";
            return os;
        }


        std::ostream& Duration::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PositiveDivisionsValue Duration::getValue() const
        {
            return myValue;
        }


        void Duration::setValue( const PositiveDivisionsValue& value )
        {
            myValue = value;
        }


        bool Duration::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
