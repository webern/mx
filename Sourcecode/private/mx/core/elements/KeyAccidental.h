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

        MX_FORWARD_DECLARE_ELEMENT( KeyAccidental )

        inline KeyAccidentalPtr makeKeyAccidental() { return std::make_shared<KeyAccidental>(); }
		inline KeyAccidentalPtr makeKeyAccidental( const AccidentalValue& value ) { return std::make_shared<KeyAccidental>( value ); }
		inline KeyAccidentalPtr makeKeyAccidental( AccidentalValue&& value ) { return std::make_shared<KeyAccidental>( std::move( value ) ); }

        class KeyAccidental : public ElementInterface
        {
        public:
            KeyAccidental();
            KeyAccidental( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            AccidentalValue myValue;
        };
    }
}
