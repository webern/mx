// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Unpitched.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FullNoteTypeChoice::FullNoteTypeChoice()
        :myChoice( Choice::pitch )
        ,myPitch( makePitch() )
        ,myUnpitched( makeUnpitched() )
        ,myRest( makeRest() )
        {}


        bool FullNoteTypeChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& FullNoteTypeChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& FullNoteTypeChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool FullNoteTypeChoice::hasContents() const
        {
            return true;
        }


        std::ostream& FullNoteTypeChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            
            MX_UNUSED( indentLevel );
            MX_UNUSED( isOneLineOnly );
            
            switch ( myChoice )
            {
                case Choice::pitch:
                    getPitch()->toStream( os, indentLevel );
                    break;
                case Choice::unpitched:
                    getUnpitched()->toStream( os, indentLevel );
                    break;
                case Choice::rest:
                    getRest()->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            return os;
        }


        FullNoteTypeChoice::Choice FullNoteTypeChoice::getChoice() const
        {
            return myChoice;
        }


        void FullNoteTypeChoice::setChoice( const FullNoteTypeChoice::Choice value )
        {
            myChoice = value;
        }


        PitchPtr FullNoteTypeChoice::getPitch() const
        {
            MX_LOCK
            MX_JIT_ALLOCATE( Pitch );
            return myPitch;
        }


        void FullNoteTypeChoice::setPitch( const PitchPtr& value )
        {
            if ( value )
            {
                myPitch = value;
            }
        }


        UnpitchedPtr FullNoteTypeChoice::getUnpitched() const
        {
            MX_LOCK
            MX_JIT_ALLOCATE( Unpitched );
            return myUnpitched;
        }


        void FullNoteTypeChoice::setUnpitched( const UnpitchedPtr& value )
        {
            if ( value )
            {
                myUnpitched = value;
            }
        }


        RestPtr FullNoteTypeChoice::getRest() const
        {
            MX_LOCK
            MX_JIT_ALLOCATE( Rest );
            return myRest;
        }


        void FullNoteTypeChoice::setRest( const RestPtr& value )
        {
            if ( value )
            {
                myRest = value;
            }
        }
        
        MX_FROM_XELEMENT_UNUSED( FullNoteTypeChoice );
    }
}
