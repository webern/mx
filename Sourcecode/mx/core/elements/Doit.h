// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/EmptyLineAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( EmptyLineAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Doit )

        inline DoitPtr makeDoit() { return std::make_shared<Doit>(); }

        class Doit : public ElementInterface
        {
        public:
            Doit();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyLineAttributesPtr getAttributes() const;
            void setAttributes( const EmptyLineAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            EmptyLineAttributesPtr myAttributes;
        };
    }
}
