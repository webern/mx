// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/EmptyPlacementAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( EmptyPlacementAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Staccato )

        inline StaccatoPtr makeStaccato() { return std::make_shared<Staccato>(); }

        class Staccato : public ElementInterface
        {
        public:
            Staccato();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPlacementAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPlacementAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            EmptyPlacementAttributesPtr myAttributes;
        };
    }
}
