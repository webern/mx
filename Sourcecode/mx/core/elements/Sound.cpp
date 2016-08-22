// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Sound.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MidiDevice.h"
#include "mx/core/elements/MidiInstrument.h"
#include "mx/core/elements/Play.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Sound::Sound()
        :myAttributes( std::make_shared<SoundAttributes>() )
        ,myMidiDevice( makeMidiDevice() )
        ,myHasMidiDevice( false )
        ,myMidiInstrument( makeMidiInstrument() )
        ,myHasMidiInstrument( false )
        ,myPlay( makePlay() )
        ,myHasPlay( false )
        {}


        bool Sound::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Sound::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Sound::streamName( std::ostream& os ) const
        {
            os << "sound";
            return os;
        }


        bool Sound::hasContents() const
        {
            return myHasMidiDevice || myHasMidiInstrument || myHasPlay;
        }


        std::ostream& Sound::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasMidiDevice )
            {
                os << std::endl;
                myMidiDevice->toStream( os, indentLevel+1 );
            }
            if ( myMidiInstrument )
            {
                os << std::endl;
                myMidiInstrument->toStream( os, indentLevel+1 );
            }
            if ( myHasPlay )
            {
                os << std::endl;
                myPlay->toStream( os, indentLevel+1 );
            }
            if ( hasContents() )
            {
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        SoundAttributesPtr Sound::getAttributes() const
        {
            return myAttributes;
        }


        void Sound::setAttributes( const SoundAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        MidiDevicePtr Sound::getMidiDevice() const
        {
            return myMidiDevice;
        }


        void Sound::setMidiDevice( const MidiDevicePtr& value )
        {
            if( value )
            {
                myMidiDevice = value;
            }
        }


        bool Sound::getHasMidiDevice() const
        {
            return myHasMidiDevice;
        }


        void Sound::setHasMidiDevice( const bool value )
        {
            myHasMidiDevice = value;
        }


        MidiInstrumentPtr Sound::getMidiInstrument() const
        {
            return myMidiInstrument;
        }


        void Sound::setMidiInstrument( const MidiInstrumentPtr& value )
        {
            if( value )
            {
                myMidiInstrument = value;
            }
        }


        bool Sound::getHasMidiInstrument() const
        {
            return myHasMidiInstrument;
        }


        void Sound::setHasMidiInstrument( const bool value )
        {
            myHasMidiInstrument = value;
        }


        PlayPtr Sound::getPlay() const
        {
            return myPlay;
        }


        void Sound::setPlay( const PlayPtr& value )
        {
            if( value )
            {
                myPlay = value;
            }
        }


        bool Sound::getHasPlay() const
        {
            return myHasPlay;
        }


        void Sound::setHasPlay( const bool value )
        {
            myHasPlay = value;
        }


        bool Sound::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myMidiDevice, myHasMidiDevice ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMidiInstrument, myHasMidiInstrument ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPlay, myHasPlay ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
