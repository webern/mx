// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ToeAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ToeAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Toe )

        inline ToePtr makeToe() { return std::make_shared<Toe>(); }

        class Toe : public ElementInterface
        {
        public:
            Toe();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ToeAttributesPtr getAttributes() const;
            void setAttributes( const ToeAttributesPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ToeAttributesPtr myAttributes;
        };
    }
}
