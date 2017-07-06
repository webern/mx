// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"
#include "mx/core/elements/FirstFretAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( FirstFretAttributes )
        MX_FORWARD_DECLARE_ELEMENT( FirstFret )

        inline FirstFretPtr makeFirstFret() { return std::make_shared<FirstFret>(); }
		inline FirstFretPtr makeFirstFret( const PositiveInteger& value ) { return std::make_shared<FirstFret>( value ); }
		inline FirstFretPtr makeFirstFret( PositiveInteger&& value ) { return std::make_shared<FirstFret>( std::move( value ) ); }

        class FirstFret : public ElementInterface
        {
        public:
            FirstFret();
            FirstFret( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FirstFretAttributesPtr getAttributes() const;
            void setAttributes( const FirstFretAttributesPtr& attributes );
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveInteger myValue;
            FirstFretAttributesPtr myAttributes;
        };
    }
}
