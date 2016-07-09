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

        MX_FORWARD_DECLARE_ELEMENT( MidiChannel )

        inline MidiChannelPtr makeMidiChannel() { return std::make_shared<MidiChannel>(); }
		inline MidiChannelPtr makeMidiChannel( const Midi16& value ) { return std::make_shared<MidiChannel>( value ); }
		inline MidiChannelPtr makeMidiChannel( Midi16&& value ) { return std::make_shared<MidiChannel>( std::move( value ) ); }

        class MidiChannel : public ElementInterface
        {
        public:
            MidiChannel();
            MidiChannel( const Midi16& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Midi16 getValue() const;
            void setValue( const Midi16& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Midi16 myValue;
        };
    }
}
