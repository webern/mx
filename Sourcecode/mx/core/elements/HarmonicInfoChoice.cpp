// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/HarmonicInfoChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BasePitch.h"
#include "mx/core/elements/SoundingPitch.h"
#include "mx/core/elements/TouchingPitch.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HarmonicInfoChoice::HarmonicInfoChoice()
        :myChoice( Choice::basePitch )
        ,myBasePitch( makeBasePitch() )
        ,myTouchingPitch( makeTouchingPitch() )
        ,mySoundingPitch( makeSoundingPitch() )
        {}


        bool HarmonicInfoChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& HarmonicInfoChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& HarmonicInfoChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool HarmonicInfoChoice::hasContents() const
        {
            return true;
        }


        std::ostream& HarmonicInfoChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::basePitch:
                    myBasePitch->toStream( os, indentLevel );
                    break;
                case Choice::touchingPitch:
                    myTouchingPitch->toStream( os, indentLevel );
                    break;
                case Choice::soundingPitch:
                    mySoundingPitch->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        HarmonicInfoChoice::Choice HarmonicInfoChoice::getChoice() const
        {
            return myChoice;
        }


        void HarmonicInfoChoice::setChoice( const HarmonicInfoChoice::Choice value )
        {
            myChoice = value;
        }


        BasePitchPtr HarmonicInfoChoice::getBasePitch() const
        {
            return myBasePitch;
        }


        void HarmonicInfoChoice::setBasePitch( const BasePitchPtr& value )
        {
            if ( value )
            {
                myBasePitch = value;
            }
        }


        TouchingPitchPtr HarmonicInfoChoice::getTouchingPitch() const
        {
            return myTouchingPitch;
        }


        void HarmonicInfoChoice::setTouchingPitch( const TouchingPitchPtr& value )
        {
            if ( value )
            {
                myTouchingPitch = value;
            }
        }


        SoundingPitchPtr HarmonicInfoChoice::getSoundingPitch() const
        {
            return mySoundingPitch;
        }


        void HarmonicInfoChoice::setSoundingPitch( const SoundingPitchPtr& value )
        {
            if ( value )
            {
                mySoundingPitch = value;
            }
        }

        
        MX_FROM_XELEMENT_UNUSED( HarmonicInfoChoice );
        
    }
}
