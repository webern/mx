// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Play.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Ipa.h"
#include "mx/core/elements/Mute.h"
#include "mx/core/elements/OtherPlay.h"
#include "mx/core/elements/SemiPitched.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Play::Play()
        :myAttributes( std::make_shared<PlayAttributes>() )
        ,myChoice( Choice::ipa )
        ,myIpa( makeIpa() )
        ,myMute( makeMute() )
        ,mySemiPitched( makeSemiPitched() )
        ,myOtherPlay( makeOtherPlay() )
        {}


        bool Play::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Play::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Play::streamName( std::ostream& os ) const
        {
            os << "play";
            return os;
        }


        bool Play::hasContents() const
        {
            return true;
        }


        std::ostream& Play::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            switch ( myChoice )
            {
                case Choice::ipa:
                {
                    myIpa->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::mute:
                {
                    myMute->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::semiPitched:
                {
                    mySemiPitched->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::otherPlay:
                {
                    myOtherPlay->toStream( os, indentLevel+1 );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        PlayAttributesPtr Play::getAttributes() const
        {
            return myAttributes;
        }


        void Play::setAttributes( const PlayAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        Play::Choice Play::getChoice() const
        {
            return myChoice;
        }


        void Play::setChoice( const Play::Choice value )
        {
            myChoice = value;
        }


        IpaPtr Play::getIpa() const
        {
            return myIpa;
        }


        void Play::setIpa( const IpaPtr& value )
        {
            if( value )
            {
                myIpa = value;
            }
        }


        MutePtr Play::getMute() const
        {
            return myMute;
        }


        void Play::setMute( const MutePtr& value )
        {
            if( value )
            {
                myMute = value;
            }
        }


        SemiPitchedPtr Play::getSemiPitched() const
        {
            return mySemiPitched;
        }


        void Play::setSemiPitched( const SemiPitchedPtr& value )
        {
            if( value )
            {
                mySemiPitched = value;
            }
        }


        OtherPlayPtr Play::getOtherPlay() const
        {
            return myOtherPlay;
        }


        void Play::setOtherPlay( const OtherPlayPtr& value )
        {
            if( value )
            {
                myOtherPlay = value;
            }
        }


        bool Play::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            if( xelement.getName() == "ipa" )
            {
                myChoice = Choice::ipa;
                return getIpa()->fromXElement( message, xelement );
            }
            else if( xelement.getName() == "mute" )
            {
                myChoice = Choice::mute;
                return getMute()->fromXElement( message, xelement );
            }
            else if( xelement.getName() == "semi-pitched" )
            {
                myChoice = Choice::semiPitched;
                return getSemiPitched()->fromXElement( message, xelement );
            }
            else if( xelement.getName() == "other-play" )
            {
                myChoice = Choice::otherPlay;
                return getOtherPlay()->fromXElement( message, xelement );
            }
            
            message << "Encoding: '" << xelement.getName() << "' is not valid" << std::endl;
            return false;
        }

    }
}
