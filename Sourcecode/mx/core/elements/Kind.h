// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/KindAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( KindAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Kind )

        inline KindPtr makeKind() { return std::make_shared<Kind>(); }
		inline KindPtr makeKind( const KindValue& value ) { return std::make_shared<Kind>( value ); }
		inline KindPtr makeKind( KindValue&& value ) { return std::make_shared<Kind>( std::move( value ) ); }

        class Kind : public ElementInterface
        {
        public:
            Kind();
            Kind( const KindValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KindAttributesPtr getAttributes() const;
            void setAttributes( const KindAttributesPtr& attributes );
            KindValue getValue() const;
            void setValue( const KindValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            KindValue myValue;
            KindAttributesPtr myAttributes;
        };
    }
}
