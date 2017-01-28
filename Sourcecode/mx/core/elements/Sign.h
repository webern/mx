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

        MX_FORWARD_DECLARE_ELEMENT( Sign )

        inline SignPtr makeSign() { return std::make_shared<Sign>(); }
		inline SignPtr makeSign( const ClefSign& value ) { return std::make_shared<Sign>( value ); }
		inline SignPtr makeSign( ClefSign&& value ) { return std::make_shared<Sign>( std::move( value ) ); }

        class Sign : public ElementInterface
        {
        public:
            Sign();
            Sign( const ClefSign& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ClefSign getValue() const;
            void setValue( const ClefSign& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ClefSign myValue;
        };
    }
}
