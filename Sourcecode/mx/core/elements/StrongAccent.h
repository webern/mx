// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/StrongAccentAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( StrongAccentAttributes )
        MX_FORWARD_DECLARE_ELEMENT( StrongAccent )

        inline StrongAccentPtr makeStrongAccent() { return std::make_shared<StrongAccent>(); }

        class StrongAccent : public ElementInterface
        {
        public:
            StrongAccent();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StrongAccentAttributesPtr getAttributes() const;
            void setAttributes( const StrongAccentAttributesPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StrongAccentAttributesPtr myAttributes;
        };
    }
}
