// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/InstrumentAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( InstrumentAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Instrument )

        inline InstrumentPtr makeInstrument() { return std::make_shared<Instrument>(); }

        class Instrument : public ElementInterface
        {
        public:
            Instrument();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            InstrumentAttributesPtr getAttributes() const;
            void setAttributes( const InstrumentAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            InstrumentAttributesPtr myAttributes;
        };
    }
}
