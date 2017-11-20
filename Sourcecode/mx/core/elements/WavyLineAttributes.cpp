// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/WavyLineAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WavyLineAttributes::WavyLineAttributes()
        :type( StartStopContinue::start )
        ,number()
        ,defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,placement( AboveBelow::below )
        ,startNote( StartNote::main )
        ,trillStep()
        ,twoNoteTurn( TwoNoteTurn::none )
        ,accelerate( YesNo::no )
        ,beats()
        ,secondBeat()
        ,lastBeat()
        ,hasType( true )
        ,hasNumber( false )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasPlacement( false )
        ,hasStartNote( false )
        ,hasTrillStep( false )
        ,hasTwoNoteTurn( false )
        ,hasAccelerate( false )
        ,hasBeats( false )
        ,hasSecondBeat( false )
        ,hasLastBeat( false )
        {}


        bool WavyLineAttributes::hasValues() const
        {
            return hasType ||
            hasNumber ||
            hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasPlacement ||
            hasStartNote ||
            hasTrillStep ||
            hasTwoNoteTurn ||
            hasAccelerate ||
            hasBeats ||
            hasSecondBeat ||
            hasLastBeat;
        }


        std::ostream& WavyLineAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, type, "type", hasType );
                streamAttribute( os, number, "number", hasNumber );
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, placement, "placement", hasPlacement );
                streamAttribute( os, startNote, "start-note", hasStartNote );
                streamAttribute( os, trillStep, "trill-step", hasTrillStep );
                streamAttribute( os, twoNoteTurn, "two-note-turn", hasTwoNoteTurn );
                streamAttribute( os, accelerate, "accelerate", hasAccelerate );
                streamAttribute( os, beats, "beats", hasBeats );
                streamAttribute( os, secondBeat, "second-beat", hasSecondBeat );
                streamAttribute( os, lastBeat, "last-beat", hasLastBeat );
            }
            return os;
        }


        bool WavyLineAttributes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "WavyLineAttributes";
            bool isSuccess = true;
            bool isTypeFound = false;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, type, isTypeFound, "type", &parseStartStopContinue ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, number, hasNumber, "number" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, startNote, hasStartNote, "start-note", &parseStartNote ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, trillStep, hasTrillStep, "trill-step", &parseTrillStep ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, twoNoteTurn, hasTwoNoteTurn, "two-note-turn", &parseTwoNoteTurn ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, accelerate, hasAccelerate, "accelerate", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, beats, hasBeats, "beats" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, secondBeat, hasSecondBeat, "second-beat" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lastBeat, hasLastBeat, "last-beat" ) ) { continue; }
            }
        
            if( !isTypeFound )
            {
                isSuccess = false;
                message << className << ": 'number' is a required attribute but was not found" << std::endl;
            }
        
            return isSuccess;
        }

    }
}
