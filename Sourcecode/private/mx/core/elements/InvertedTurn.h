// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/InvertedTurnAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( InvertedTurnAttributes )
        MX_FORWARD_DECLARE_ELEMENT( InvertedTurn )

        inline InvertedTurnPtr makeInvertedTurn() { return std::make_shared<InvertedTurn>(); }

        class InvertedTurn : public ElementInterface
        {
        public:
            InvertedTurn();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InvertedTurnAttributesPtr getAttributes() const;
            void setAttributes( const InvertedTurnAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            InvertedTurnAttributesPtr myAttributes;
        };
    }
}
