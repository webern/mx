// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Glass.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Glass::Glass()
        :myValue( GlassEnum::windChimes )
        {}


        Glass::Glass( const GlassEnum& value )
        :myValue( value )
        {}


        bool Glass::hasAttributes() const
        {
            return false;
        }


        bool Glass::hasContents() const
        {
            return true;
        }


        std::ostream& Glass::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Glass::streamName( std::ostream& os ) const
        {
            os << "glass";
            return os;
        }


        std::ostream& Glass::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GlassEnum Glass::getValue() const
        {
            return myValue;
        }


        void Glass::setValue( const GlassEnum& value )
        {
            myValue = value;
        }


        bool Glass::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseGlassEnum( xelement.getValue() );
            return true;
        }

    }
}
