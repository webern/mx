// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/BarStyleAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BarStyleAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BarStyle )

        inline BarStylePtr makeBarStyle() { return std::make_shared<BarStyle>(); }
		inline BarStylePtr makeBarStyle( const BarStyleEnum& value ) { return std::make_shared<BarStyle>( value ); }
		inline BarStylePtr makeBarStyle( BarStyleEnum&& value ) { return std::make_shared<BarStyle>( std::move( value ) ); }

        class BarStyle : public ElementInterface
        {
        public:
            BarStyle();
            BarStyle( const BarStyleEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BarStyleAttributesPtr getAttributes() const;
            void setAttributes( const BarStyleAttributesPtr& attributes );
            BarStyleEnum getValue() const;
            void setValue( const BarStyleEnum& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BarStyleEnum myValue;
            BarStyleAttributesPtr myAttributes;
        };
    }
}
