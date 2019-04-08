// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/TupletDotAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TupletDotAttributes )
        MX_FORWARD_DECLARE_ELEMENT( TupletDot )

        inline TupletDotPtr makeTupletDot() { return std::make_shared<TupletDot>(); }

        class TupletDot : public ElementInterface
        {
        public:
            TupletDot();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TupletDotAttributesPtr getAttributes() const;
            void setAttributes( const TupletDotAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            TupletDotAttributesPtr myAttributes;
        };
    }
}
