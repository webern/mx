// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/BreathMarkAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BreathMarkAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BreathMark )

        inline BreathMarkPtr makeBreathMark() { return std::make_shared<BreathMark>(); }
		inline BreathMarkPtr makeBreathMark( const BreathMarkValue& value ) { return std::make_shared<BreathMark>( value ); }
		inline BreathMarkPtr makeBreathMark( BreathMarkValue&& value ) { return std::make_shared<BreathMark>( std::move( value ) ); }

        class BreathMark : public ElementInterface
        {
        public:
            BreathMark();
            BreathMark( const BreathMarkValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BreathMarkAttributesPtr getAttributes() const;
            void setAttributes( const BreathMarkAttributesPtr& attributes );
            BreathMarkValue getValue() const;
            void setValue( const BreathMarkValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BreathMarkValue myValue;
            BreathMarkAttributesPtr myAttributes;
        };
    }
}
