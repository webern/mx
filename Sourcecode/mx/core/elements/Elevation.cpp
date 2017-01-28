// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Elevation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Elevation::Elevation()
        :myValue()
        {}


        Elevation::Elevation( const RotationDegrees& value )
        :myValue( value )
        {}


        bool Elevation::hasAttributes() const
        {
            return false;
        }


        bool Elevation::hasContents() const
        {
            return true;
        }


        std::ostream& Elevation::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Elevation::streamName( std::ostream& os ) const
        {
            os << "elevation";
            return os;
        }


        std::ostream& Elevation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RotationDegrees Elevation::getValue() const
        {
            return myValue;
        }


        void Elevation::setValue( const RotationDegrees& value )
        {
            myValue = value;
        }


        bool Elevation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
