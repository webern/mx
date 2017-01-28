// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Volume.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Volume::Volume()
        :myValue()
        {}


        Volume::Volume( const Percent& value )
        :myValue( value )
        {}


        bool Volume::hasAttributes() const
        {
            return false;
        }


        bool Volume::hasContents() const
        {
            return true;
        }


        std::ostream& Volume::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Volume::streamName( std::ostream& os ) const
        {
            os << "volume";
            return os;
        }


        std::ostream& Volume::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Percent Volume::getValue() const
        {
            return myValue;
        }


        void Volume::setValue( const Percent& value )
        {
            myValue = value;
        }


        bool Volume::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
