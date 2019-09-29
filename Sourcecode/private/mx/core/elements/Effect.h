// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Effect )

        inline EffectPtr makeEffect() { return std::make_shared<Effect>(); }
		inline EffectPtr makeEffect( const EffectEnum& value ) { return std::make_shared<Effect>( value ); }
		inline EffectPtr makeEffect( EffectEnum&& value ) { return std::make_shared<Effect>( std::move( value ) ); }

        class Effect : public ElementInterface
        {
        public:
            Effect();
            Effect( const EffectEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EffectEnum getValue() const;
            void setValue( const EffectEnum& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            EffectEnum myValue;
        };
    }
}
