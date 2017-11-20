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

        MX_FORWARD_DECLARE_ELEMENT( Pan )

        inline PanPtr makePan() { return std::make_shared<Pan>(); }
		inline PanPtr makePan( const RotationDegrees& value ) { return std::make_shared<Pan>( value ); }
		inline PanPtr makePan( RotationDegrees&& value ) { return std::make_shared<Pan>( std::move( value ) ); }

        class Pan : public ElementInterface
        {
        public:
            Pan();
            Pan( const RotationDegrees& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RotationDegrees getValue() const;
            void setValue( const RotationDegrees& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            RotationDegrees myValue;
        };
    }
}
