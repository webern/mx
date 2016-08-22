// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MidiInstrument.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Elevation.h"
#include "mx/core/elements/MidiBank.h"
#include "mx/core/elements/MidiChannel.h"
#include "mx/core/elements/MidiName.h"
#include "mx/core/elements/MidiProgram.h"
#include "mx/core/elements/MidiUnpitched.h"
#include "mx/core/elements/Pan.h"
#include "mx/core/elements/Volume.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiInstrument::MidiInstrument()
        :myAttributes( std::make_shared<MidiInstrumentAttributes>() )
        ,myMidiChannel( makeMidiChannel() )
        ,myHasMidiChannel( false )
        ,myMidiName( makeMidiName() )
        ,myHasMidiName( false )
        ,myMidiBank( makeMidiBank() )
        ,myHasMidiBank( false )
        ,myMidiProgram( makeMidiProgram() )
        ,myHasMidiProgram( false )
        ,myMidiUnpitched( makeMidiUnpitched() )
        ,myHasMidiUnpitched( false )
        ,myVolume( makeVolume() )
        ,myHasVolume( false )
        ,myPan( makePan() )
        ,myHasPan( false )
        ,myElevation( makeElevation() )
        ,myHasElevation( false )
        {}


        bool MidiInstrument::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& MidiInstrument::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& MidiInstrument::streamName( std::ostream& os ) const
        {
            os << "midi-instrument";
            return os;
        }


        bool MidiInstrument::hasContents() const
        {
            return myHasMidiChannel
            || myHasMidiName
            || myHasMidiBank
            || myHasMidiProgram
            || myHasMidiUnpitched
            || myHasVolume
            || myHasPan
            || myHasElevation;
        }


        std::ostream& MidiInstrument::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasMidiChannel )
            {
                os << std::endl;
                myMidiChannel->toStream( os, indentLevel+1 );
            }
            if ( myHasMidiName )
            {
                os << std::endl;
                myMidiName->toStream( os, indentLevel+1 );
            }
            if ( myHasMidiBank )
            {
                os << std::endl;
                myMidiBank->toStream( os, indentLevel+1 );
            }
            if ( myHasMidiProgram )
            {
                os << std::endl;
                myMidiProgram->toStream( os, indentLevel+1 );
            }
            if ( myHasMidiUnpitched )
            {
                os << std::endl;
                myMidiUnpitched->toStream( os, indentLevel+1 );
            }
            if ( myHasVolume )
            {
                os << std::endl;
                myVolume->toStream( os, indentLevel+1 );
            }
            if ( myHasPan )
            {
                os << std::endl;
                myPan->toStream( os, indentLevel+1 );
            }
            if ( myHasElevation )
            {
                os << std::endl;
                myElevation->toStream( os, indentLevel+1 );
            }
            if( hasContents() )
            {
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        MidiInstrumentAttributesPtr MidiInstrument::getAttributes() const
        {
            return myAttributes;
        }


        void MidiInstrument::setAttributes( const MidiInstrumentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        MidiChannelPtr MidiInstrument::getMidiChannel() const
        {
            return myMidiChannel;
        }


        void MidiInstrument::setMidiChannel( const MidiChannelPtr& value )
        {
            if( value )
            {
                myMidiChannel = value;
            }
        }


        bool MidiInstrument::getHasMidiChannel() const
        {
            return myHasMidiChannel;
        }


        void MidiInstrument::setHasMidiChannel( const bool value )
        {
            myHasMidiChannel = value;
        }


        MidiNamePtr MidiInstrument::getMidiName() const
        {
            return myMidiName;
        }


        void MidiInstrument::setMidiName( const MidiNamePtr& value )
        {
            if( value )
            {
                myMidiName = value;
            }
        }


        bool MidiInstrument::getHasMidiName() const
        {
            return myHasMidiName;
        }


        void MidiInstrument::setHasMidiName( const bool value )
        {
            myHasMidiName = value;
        }


        MidiBankPtr MidiInstrument::getMidiBank() const
        {
            return myMidiBank;
        }


        void MidiInstrument::setMidiBank( const MidiBankPtr& value )
        {
            if( value )
            {
                myMidiBank = value;
            }
        }


        bool MidiInstrument::getHasMidiBank() const
        {
            return myHasMidiBank;
        }


        void MidiInstrument::setHasMidiBank( const bool value )
        {
            myHasMidiBank = value;
        }


        MidiProgramPtr MidiInstrument::getMidiProgram() const
        {
            return myMidiProgram;
        }


        void MidiInstrument::setMidiProgram( const MidiProgramPtr& value )
        {
            if( value )
            {
                myMidiProgram = value;
            }
        }


        bool MidiInstrument::getHasMidiProgram() const
        {
            return myHasMidiProgram;
        }


        void MidiInstrument::setHasMidiProgram( const bool value )
        {
            myHasMidiProgram = value;
        }


        MidiUnpitchedPtr MidiInstrument::getMidiUnpitched() const
        {
            return myMidiUnpitched;
        }


        void MidiInstrument::setMidiUnpitched( const MidiUnpitchedPtr& value )
        {
            if( value )
            {
                myMidiUnpitched = value;
            }
        }


        bool MidiInstrument::getHasMidiUnpitched() const
        {
            return myHasMidiUnpitched;
        }


        void MidiInstrument::setHasMidiUnpitched( const bool value )
        {
            myHasMidiUnpitched = value;
        }


        VolumePtr MidiInstrument::getVolume() const
        {
            return myVolume;
        }


        void MidiInstrument::setVolume( const VolumePtr& value )
        {
            if( value )
            {
                myVolume = value;
            }
        }


        bool MidiInstrument::getHasVolume() const
        {
            return myHasVolume;
        }


        void MidiInstrument::setHasVolume( const bool value )
        {
            myHasVolume = value;
        }


        PanPtr MidiInstrument::getPan() const
        {
            return myPan;
        }


        void MidiInstrument::setPan( const PanPtr& value )
        {
            if( value )
            {
                myPan = value;
            }
        }


        bool MidiInstrument::getHasPan() const
        {
            return myHasPan;
        }


        void MidiInstrument::setHasPan( const bool value )
        {
            myHasPan = value;
        }


        ElevationPtr MidiInstrument::getElevation() const
        {
            return myElevation;
        }


        void MidiInstrument::setElevation( const ElevationPtr& value )
        {
            if( value )
            {
                myElevation = value;
            }
        }


        bool MidiInstrument::getHasElevation() const
        {
            return myHasElevation;
        }


        void MidiInstrument::setHasElevation( const bool value )
        {
            myHasElevation = value;
        }


        bool MidiInstrument::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myMidiChannel, myHasMidiChannel ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMidiName, myHasMidiName ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMidiBank, myHasMidiBank ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMidiProgram, myHasMidiProgram ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMidiUnpitched, myHasMidiUnpitched ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myVolume, myHasVolume ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPan, myHasPan ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myElevation, myHasElevation ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
