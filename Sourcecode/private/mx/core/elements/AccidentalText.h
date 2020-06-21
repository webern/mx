// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/AccidentalTextAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( AccidentalTextAttributes )
        MX_FORWARD_DECLARE_ELEMENT( AccidentalText )

        inline AccidentalTextPtr makeAccidentalText() { return std::make_shared<AccidentalText>(); }
		inline AccidentalTextPtr makeAccidentalText( const AccidentalValue& value ) { return std::make_shared<AccidentalText>( value ); }
		inline AccidentalTextPtr makeAccidentalText( AccidentalValue&& value ) { return std::make_shared<AccidentalText>( std::move( value ) ); }

        class AccidentalText : public ElementInterface
        {
        public:
            AccidentalText();
            AccidentalText( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalTextAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalTextAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            AccidentalValue myValue;
            AccidentalTextAttributesPtr myAttributes;
        };
    }
}
