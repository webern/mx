// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/HeelAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HeelAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Heel )

        inline HeelPtr makeHeel() { return std::make_shared<Heel>(); }

        class Heel : public ElementInterface
        {
        public:
            Heel();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HeelAttributesPtr getAttributes() const;
            void setAttributes( const HeelAttributesPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            HeelAttributesPtr myAttributes;
        };
    }
}
