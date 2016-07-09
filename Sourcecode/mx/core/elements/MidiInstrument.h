// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/MidiInstrumentAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MidiInstrumentAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Elevation )
        MX_FORWARD_DECLARE_ELEMENT( MidiBank )
        MX_FORWARD_DECLARE_ELEMENT( MidiChannel )
        MX_FORWARD_DECLARE_ELEMENT( MidiName )
        MX_FORWARD_DECLARE_ELEMENT( MidiProgram )
        MX_FORWARD_DECLARE_ELEMENT( MidiUnpitched )
        MX_FORWARD_DECLARE_ELEMENT( Pan )
        MX_FORWARD_DECLARE_ELEMENT( Volume )
        MX_FORWARD_DECLARE_ELEMENT( MidiInstrument )

        inline MidiInstrumentPtr makeMidiInstrument() { return std::make_shared<MidiInstrument>(); }

        class MidiInstrument : public ElementInterface
        {
        public:
            MidiInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MidiInstrumentAttributesPtr getAttributes() const;
            void setAttributes( const MidiInstrumentAttributesPtr& value );

            /* _________ MidiChannel minOccurs = 0, maxOccurs = 1 _________ */
            MidiChannelPtr getMidiChannel() const;
            void setMidiChannel( const MidiChannelPtr& value );
            bool getHasMidiChannel() const;
            void setHasMidiChannel( const bool value );

            /* _________ MidiName minOccurs = 0, maxOccurs = 1 _________ */
            MidiNamePtr getMidiName() const;
            void setMidiName( const MidiNamePtr& value );
            bool getHasMidiName() const;
            void setHasMidiName( const bool value );

            /* _________ MidiBank minOccurs = 0, maxOccurs = 1 _________ */
            MidiBankPtr getMidiBank() const;
            void setMidiBank( const MidiBankPtr& value );
            bool getHasMidiBank() const;
            void setHasMidiBank( const bool value );

            /* _________ MidiProgram minOccurs = 0, maxOccurs = 1 _________ */
            MidiProgramPtr getMidiProgram() const;
            void setMidiProgram( const MidiProgramPtr& value );
            bool getHasMidiProgram() const;
            void setHasMidiProgram( const bool value );

            /* _________ MidiUnpitched minOccurs = 0, maxOccurs = 1 _________ */
            MidiUnpitchedPtr getMidiUnpitched() const;
            void setMidiUnpitched( const MidiUnpitchedPtr& value );
            bool getHasMidiUnpitched() const;
            void setHasMidiUnpitched( const bool value );

            /* _________ Volume minOccurs = 0, maxOccurs = 1 _________ */
            VolumePtr getVolume() const;
            void setVolume( const VolumePtr& value );
            bool getHasVolume() const;
            void setHasVolume( const bool value );

            /* _________ Pan minOccurs = 0, maxOccurs = 1 _________ */
            PanPtr getPan() const;
            void setPan( const PanPtr& value );
            bool getHasPan() const;
            void setHasPan( const bool value );

            /* _________ Elevation minOccurs = 0, maxOccurs = 1 _________ */
            ElevationPtr getElevation() const;
            void setElevation( const ElevationPtr& value );
            bool getHasElevation() const;
            void setHasElevation( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MidiInstrumentAttributesPtr myAttributes;
            MidiChannelPtr myMidiChannel;
            bool myHasMidiChannel;
            MidiNamePtr myMidiName;
            bool myHasMidiName;
            MidiBankPtr myMidiBank;
            bool myHasMidiBank;
            MidiProgramPtr myMidiProgram;
            bool myHasMidiProgram;
            MidiUnpitchedPtr myMidiUnpitched;
            bool myHasMidiUnpitched;
            VolumePtr myVolume;
            bool myHasVolume;
            PanPtr myPan;
            bool myHasPan;
            ElevationPtr myElevation;
            bool myHasElevation;
        };
    }
}
