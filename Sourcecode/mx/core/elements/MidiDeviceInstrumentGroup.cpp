// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MidiDeviceInstrumentGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MidiDevice.h"
#include "mx/core/elements/MidiInstrument.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiDeviceInstrumentGroup::MidiDeviceInstrumentGroup()
        :myMidiDevice( makeMidiDevice() )
        ,myHasMidiDevice( false )
        ,myMidiInstrument( makeMidiInstrument() )
        ,myHasMidiInstrument( false )
        {}


        bool MidiDeviceInstrumentGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& MidiDeviceInstrumentGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MidiDeviceInstrumentGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool MidiDeviceInstrumentGroup::hasContents() const
        {
            return myHasMidiDevice || myHasMidiInstrument;
        }


        std::ostream& MidiDeviceInstrumentGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( this->hasContents() )
            {
                if ( myHasMidiDevice )
                {
                    myMidiDevice->toStream( os, indentLevel );
                    if ( myHasMidiInstrument )
                    {
                        os << std::endl;
                    }
                }
                if ( myHasMidiInstrument )
                {
                    myMidiInstrument->toStream( os, indentLevel );
                }
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        MidiDevicePtr MidiDeviceInstrumentGroup::getMidiDevice() const
        {
            return myMidiDevice;
        }


        void MidiDeviceInstrumentGroup::setMidiDevice( const MidiDevicePtr& value )
        {
            if ( value )
            {
                myMidiDevice = value;
            }
        }


        bool MidiDeviceInstrumentGroup::getHasMidiDevice() const
        {
            return myHasMidiDevice;
        }


        void MidiDeviceInstrumentGroup::setHasMidiDevice( const bool value )
        {
            myHasMidiDevice = value;
        }


        MidiInstrumentPtr MidiDeviceInstrumentGroup::getMidiInstrument() const
        {
            return myMidiInstrument;
        }


        void MidiDeviceInstrumentGroup::setMidiInstrument( const MidiInstrumentPtr& value )
        {
            if ( value )
            {
                myMidiInstrument = value;
            }
        }


        bool MidiDeviceInstrumentGroup::getHasMidiInstrument() const
        {
            return myHasMidiInstrument;
        }


        void MidiDeviceInstrumentGroup::setHasMidiInstrument( const bool value )
        {
            myHasMidiInstrument = value;
        }
        
        
        MX_FROM_XELEMENT_UNUSED( MidiDeviceInstrumentGroup );

    }
}
