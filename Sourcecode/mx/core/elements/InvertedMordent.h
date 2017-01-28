// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/InvertedMordentAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( InvertedMordentAttributes )
        MX_FORWARD_DECLARE_ELEMENT( InvertedMordent )

        inline InvertedMordentPtr makeInvertedMordent() { return std::make_shared<InvertedMordent>(); }

        class InvertedMordent : public ElementInterface
        {
        public:
            InvertedMordent();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InvertedMordentAttributesPtr getAttributes() const;
            void setAttributes( const InvertedMordentAttributesPtr& value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            InvertedMordentAttributesPtr myAttributes;
        };
    }
}
