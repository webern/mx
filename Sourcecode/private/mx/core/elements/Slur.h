// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/SlurAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( SlurAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Slur )

        inline SlurPtr makeSlur() { return std::make_shared<Slur>(); }

        class Slur : public ElementInterface
        {
        public:
            Slur();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlurAttributesPtr getAttributes() const;
            void setAttributes( const SlurAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            SlurAttributesPtr myAttributes;
        };
    }
}
