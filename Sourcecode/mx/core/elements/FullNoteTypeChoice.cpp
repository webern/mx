// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
                    myPitch->toStream( os, indentLevel );
                    break;
                case Choice::unpitched:
                    myUnpitched->toStream( os, indentLevel );
                    break;
                case Choice::rest:
                    myRest->toStream( os, indentLevel );
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
