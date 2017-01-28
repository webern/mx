// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/AccidentalMarkAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( AccidentalMarkAttributes )
        MX_FORWARD_DECLARE_ELEMENT( AccidentalMark )

        inline AccidentalMarkPtr makeAccidentalMark() { return std::make_shared<AccidentalMark>(); }
		inline AccidentalMarkPtr makeAccidentalMark( const AccidentalValue& value ) { return std::make_shared<AccidentalMark>( value ); }
		inline AccidentalMarkPtr makeAccidentalMark( AccidentalValue&& value ) { return std::make_shared<AccidentalMark>( std::move( value ) ); }

        class AccidentalMark : public ElementInterface
        {
        public:
            AccidentalMark();
            AccidentalMark( const AccidentalValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccidentalMarkAttributesPtr getAttributes() const;
            void setAttributes( const AccidentalMarkAttributesPtr& attributes );
            AccidentalValue getValue() const;
            void setValue( const AccidentalValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            AccidentalValue myValue;
            AccidentalMarkAttributesPtr myAttributes;
        };
    }
}
