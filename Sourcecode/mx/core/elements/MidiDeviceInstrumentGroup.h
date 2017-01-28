// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( MidiDevice )
        MX_FORWARD_DECLARE_ELEMENT( MidiInstrument )
        MX_FORWARD_DECLARE_ELEMENT( MidiDeviceInstrumentGroup )

        inline MidiDeviceInstrumentGroupPtr makeMidiDeviceInstrumentGroup() { return std::make_shared<MidiDeviceInstrumentGroup>(); }

        class MidiDeviceInstrumentGroup : public ElementInterface
        {
        public:
            MidiDeviceInstrumentGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MidiDevice minOccurs = 0, maxOccurs = 1 _________ */
            MidiDevicePtr getMidiDevice() const;
            void setMidiDevice( const MidiDevicePtr& value );
            bool getHasMidiDevice() const;
            void setHasMidiDevice( const bool value );

            /* _________ MidiInstrument minOccurs = 0, maxOccurs = 1 _________ */
            MidiInstrumentPtr getMidiInstrument() const;
            void setMidiInstrument( const MidiInstrumentPtr& value );
            bool getHasMidiInstrument() const;
            void setHasMidiInstrument( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MidiDevicePtr myMidiDevice;
            bool myHasMidiDevice;
            MidiInstrumentPtr myMidiInstrument;
            bool myHasMidiInstrument;
        };
    }
}
