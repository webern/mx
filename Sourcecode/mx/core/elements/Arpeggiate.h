// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ArpeggiateAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ArpeggiateAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Arpeggiate )

        inline ArpeggiatePtr makeArpeggiate() { return std::make_shared<Arpeggiate>(); }

        class Arpeggiate : public ElementInterface
        {
        public:
            Arpeggiate();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArpeggiateAttributesPtr getAttributes() const;
            void setAttributes( const ArpeggiateAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ArpeggiateAttributesPtr myAttributes;
        };
    }
}
