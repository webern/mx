// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StickMaterial.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StickMaterial::StickMaterial()
        :myValue( StickMaterialEnum::medium )
        {}


        StickMaterial::StickMaterial( const StickMaterialEnum& value )
        :myValue( value )
        {}


        bool StickMaterial::hasAttributes() const
        {
            return false;
        }


        bool StickMaterial::hasContents() const
        {
            return true;
        }


        std::ostream& StickMaterial::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& StickMaterial::streamName( std::ostream& os ) const
        {
            os << "stick-material";
            return os;
        }


        std::ostream& StickMaterial::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StickMaterialEnum StickMaterial::getValue() const
        {
            return myValue;
        }


        void StickMaterial::setValue( const StickMaterialEnum& value )
        {
            myValue = value;
        }


        bool StickMaterial::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseStickMaterialEnum( xelement.getValue() );
            return true;
        }

    }
}
