// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/AccidentalAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( AccidentalAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Accidental )

        inline AccidentalPtr makeAccidental() { return std::make_shared<Accidental>(); }
		inline AccidentalPtr makeAccidental( const AccidentalValue& value ) { return std::make_shared<Accidental>( value ); }
		inline AccidentalPtr makeAccidental( AccidentalValue&& value ) { return std::make_shared<Accidental>( std::move( value ) ); }

        class Accidental : public ElementInterface
        {
        public:
            Accidental();
            Accidental( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            AccidentalValue myValue;
            AccidentalAttributesPtr myAttributes;
        };
    }
}
