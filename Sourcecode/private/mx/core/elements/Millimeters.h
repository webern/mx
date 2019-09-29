// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Decimals.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Millimeters )

        inline MillimetersPtr makeMillimeters() { return std::make_shared<Millimeters>(); }
		inline MillimetersPtr makeMillimeters( const MillimetersValue& value ) { return std::make_shared<Millimeters>( value ); }
		inline MillimetersPtr makeMillimeters( MillimetersValue&& value ) { return std::make_shared<Millimeters>( std::move( value ) ); }

        class Millimeters : public ElementInterface
        {
        public:
            Millimeters();
            Millimeters( const MillimetersValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MillimetersValue getValue() const;
            void setValue( const MillimetersValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            MillimetersValue myValue;
        };
    }
}
