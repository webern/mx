// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ExtendAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ExtendAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Extend )

        inline ExtendPtr makeExtend() { return std::make_shared<Extend>(); }

        class Extend : public ElementInterface
        {
        public:
            Extend();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ExtendAttributesPtr getAttributes() const;
            void setAttributes( const ExtendAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ExtendAttributesPtr myAttributes;
        };
    }
}
