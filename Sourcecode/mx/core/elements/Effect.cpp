// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Effect.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Effect::Effect()
        :myValue( EffectEnum::anvil )
        {}


        Effect::Effect( const EffectEnum& value )
        :myValue( value )
        {}


        bool Effect::hasAttributes() const
        {
            return false;
        }


        bool Effect::hasContents() const
        {
            return true;
        }


        std::ostream& Effect::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Effect::streamName( std::ostream& os ) const
        {
            os << "effect";
            return os;
        }


        std::ostream& Effect::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        EffectEnum Effect::getValue() const
        {
            return myValue;
        }


        void Effect::setValue( const EffectEnum& value )
        {
            myValue = value;
        }


        bool Effect::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseEffectEnum( xelement.getValue() );
            return true;
        }

    }
}
