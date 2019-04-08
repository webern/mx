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

        MX_FORWARD_DECLARE_ELEMENT( Wood )

        inline WoodPtr makeWood() { return std::make_shared<Wood>(); }
		inline WoodPtr makeWood( const WoodEnum& value ) { return std::make_shared<Wood>( value ); }
		inline WoodPtr makeWood( WoodEnum&& value ) { return std::make_shared<Wood>( std::move( value ) ); }

        class Wood : public ElementInterface
        {
        public:
            Wood();
            Wood( const WoodEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            WoodEnum getValue() const;
            void setValue( const WoodEnum& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            WoodEnum myValue;
        };
    }
}
