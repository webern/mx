// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
        MX_FORWARD_DECLARE_ELEMENT( TrillMark )

        inline TrillMarkPtr makeTrillMark() { return std::make_shared<TrillMark>(); }

        class TrillMark : public ElementInterface
        {
        public:
            TrillMark();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyTrillSoundAttributesPtr getAttributes() const;
            void setAttributes( const EmptyTrillSoundAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            EmptyTrillSoundAttributesPtr myAttributes;
        };
    }
}
