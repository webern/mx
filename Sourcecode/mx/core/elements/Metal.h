// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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

        MX_FORWARD_DECLARE_ELEMENT( Metal )

        inline MetalPtr makeMetal() { return std::make_shared<Metal>(); }
		inline MetalPtr makeMetal( const MetalEnum& value ) { return std::make_shared<Metal>( value ); }
		inline MetalPtr makeMetal( MetalEnum&& value ) { return std::make_shared<Metal>( std::move( value ) ); }

        class Metal : public ElementInterface
        {
        public:
            Metal();
            Metal( const MetalEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetalEnum getValue() const;
            void setValue( const MetalEnum& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MetalEnum myValue;
        };
    }
}
