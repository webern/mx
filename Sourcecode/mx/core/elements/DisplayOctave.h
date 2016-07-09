// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( DisplayOctave )

        inline DisplayOctavePtr makeDisplayOctave() { return std::make_shared<DisplayOctave>(); }
		inline DisplayOctavePtr makeDisplayOctave( const OctaveValue& value ) { return std::make_shared<DisplayOctave>( value ); }
		inline DisplayOctavePtr makeDisplayOctave( OctaveValue&& value ) { return std::make_shared<DisplayOctave>( std::move( value ) ); }

        class DisplayOctave : public ElementInterface
        {
        public:
            DisplayOctave();
            DisplayOctave( const OctaveValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveValue getValue() const;
            void setValue( const OctaveValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            OctaveValue myValue;
        };
    }
}
