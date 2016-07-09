// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"
#include "mx/core/elements/KeyOctaveAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( KeyOctaveAttributes )
        MX_FORWARD_DECLARE_ELEMENT( KeyOctave )

        inline KeyOctavePtr makeKeyOctave() { return std::make_shared<KeyOctave>(); }
		inline KeyOctavePtr makeKeyOctave( const OctaveValue& value ) { return std::make_shared<KeyOctave>( value ); }
		inline KeyOctavePtr makeKeyOctave( OctaveValue&& value ) { return std::make_shared<KeyOctave>( std::move( value ) ); }

        class KeyOctave : public ElementInterface
        {
        public:
            KeyOctave();
            KeyOctave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyOctaveAttributesPtr getAttributes() const;
            void setAttributes( const KeyOctaveAttributesPtr& attributes );
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            OctaveValue myValue;
            KeyOctaveAttributesPtr myAttributes;
        };
    }
}
