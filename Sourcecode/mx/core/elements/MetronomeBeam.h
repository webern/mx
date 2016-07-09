// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/MetronomeBeamAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MetronomeBeamAttributes )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeBeam )

        inline MetronomeBeamPtr makeMetronomeBeam() { return std::make_shared<MetronomeBeam>(); }
		inline MetronomeBeamPtr makeMetronomeBeam( const BeamValue& value ) { return std::make_shared<MetronomeBeam>( value ); }
		inline MetronomeBeamPtr makeMetronomeBeam( BeamValue&& value ) { return std::make_shared<MetronomeBeam>( std::move( value ) ); }

        class MetronomeBeam : public ElementInterface
        {
        public:
            MetronomeBeam();
            MetronomeBeam( const BeamValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MetronomeBeamAttributesPtr getAttributes() const;
            void setAttributes( const MetronomeBeamAttributesPtr& attributes );
            BeamValue getValue() const;
            void setValue( const BeamValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BeamValue myValue;
            MetronomeBeamAttributesPtr myAttributes;
        };
    }
}
