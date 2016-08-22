// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/EmptyTrillSoundAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( EmptyTrillSoundAttributes )
        MX_FORWARD_DECLARE_ELEMENT( VerticalTurn )

        inline VerticalTurnPtr makeVerticalTurn() { return std::make_shared<VerticalTurn>(); }

        class VerticalTurn : public ElementInterface
        {
        public:
            VerticalTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyTrillSoundAttributesPtr getAttributes() const;
            void setAttributes( const EmptyTrillSoundAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            EmptyTrillSoundAttributesPtr myAttributes;
        };
    }
}
