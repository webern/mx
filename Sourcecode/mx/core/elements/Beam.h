// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/BeamAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BeamAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Beam )

        inline BeamPtr makeBeam() { return std::make_shared<Beam>(); }
		inline BeamPtr makeBeam( const BeamValue& value ) { return std::make_shared<Beam>( value ); }
		inline BeamPtr makeBeam( BeamValue&& value ) { return std::make_shared<Beam>( std::move( value ) ); }

        class Beam : public ElementInterface
        {
        public:
            Beam();
            Beam( const BeamValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeamAttributesPtr getAttributes() const;
            void setAttributes( const BeamAttributesPtr& attributes );
            BeamValue getValue() const;
            void setValue( const BeamValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BeamValue myValue;
            BeamAttributesPtr myAttributes;
        };
    }
}
