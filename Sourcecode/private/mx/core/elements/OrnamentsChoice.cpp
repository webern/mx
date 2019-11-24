// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OrnamentsChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DelayedInvertedTurn.h"
#include "mx/core/elements/DelayedTurn.h"
#include "mx/core/elements/InvertedMordent.h"
#include "mx/core/elements/InvertedTurn.h"
#include "mx/core/elements/Mordent.h"
#include "mx/core/elements/OtherOrnament.h"
#include "mx/core/elements/Schleifer.h"
#include "mx/core/elements/Shake.h"
#include "mx/core/elements/Tremolo.h"
#include "mx/core/elements/TrillMark.h"
#include "mx/core/elements/Turn.h"
#include "mx/core/elements/VerticalTurn.h"
#include "mx/core/elements/WavyLine.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OrnamentsChoice::OrnamentsChoice()
        :myChoice( Choice::trillMark )
        ,myTrillMark( makeTrillMark() )
        ,myTurn( makeTurn() )
        ,myDelayedTurn( makeDelayedTurn() )
        ,myInvertedTurn( makeInvertedTurn() )
        ,myDelayedInvertedTurn( makeDelayedInvertedTurn() )
        ,myVerticalTurn( makeVerticalTurn() )
        ,myShake( makeShake() )
        ,myWavyLine( makeWavyLine() )
        ,myMordent( makeMordent() )
        ,myInvertedMordent( makeInvertedMordent() )
        ,mySchleifer( makeSchleifer() )
        ,myTremolo( makeTremolo() )
        ,myOtherOrnament( makeOtherOrnament() )
        {}


        bool OrnamentsChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& OrnamentsChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& OrnamentsChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool OrnamentsChoice::hasContents() const
        {
            return true;
        }


        std::ostream& OrnamentsChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( isOneLineOnly );
            
            switch ( myChoice )
            {
                case Choice::trillMark:
                {
                    myTrillMark->toStream( os, indentLevel );
                }
                    break;
                case Choice::turn:
                {
                    myTurn->toStream( os, indentLevel );
                }
                    break;
                case Choice::delayedTurn:
                {
                    myDelayedTurn->toStream( os, indentLevel );
                }
                    break;
                case Choice::invertedTurn:
                {
                    myInvertedTurn->toStream( os, indentLevel );
                }
                    break;
                case Choice::delayedInvertedTurn:
                {
                    myDelayedInvertedTurn->toStream( os, indentLevel );
                }
                    break;
                case Choice::verticalTurn:
                {
                    myVerticalTurn->toStream( os, indentLevel );
                }
                    break;
                case Choice::shake:
                {
                    myShake->toStream( os, indentLevel );
                }
                    break;
                case Choice::wavyLine:
                {
                    myWavyLine->toStream( os, indentLevel );
                }
                    break;
                case Choice::mordent:
                {
                    myMordent->toStream( os, indentLevel );
                }
                    break;
                case Choice::invertedMordent:
                {
                    myInvertedMordent->toStream( os, indentLevel );
                }
                    break;
                case Choice::schleifer:
                {
                    mySchleifer->toStream( os, indentLevel );
                }
                    break;
                case Choice::tremolo:
                {
                    myTremolo->toStream( os, indentLevel );
                }
                    break;
                case Choice::otherOrnament:
                {
                    myOtherOrnament->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            return os;
        }


        OrnamentsChoice::Choice OrnamentsChoice::getChoice() const
        {
            return myChoice;
        }


        void OrnamentsChoice::setChoice( const OrnamentsChoice::Choice value )
        {
            myChoice = value;
        }


        TrillMarkPtr OrnamentsChoice::getTrillMark() const
        {
            return myTrillMark;
        }


        void OrnamentsChoice::setTrillMark( const TrillMarkPtr& value )
        {
            if( value )
            {
                myTrillMark = value;
            }
        }


        TurnPtr OrnamentsChoice::getTurn() const
        {
            return myTurn;
        }


        void OrnamentsChoice::setTurn( const TurnPtr& value )
        {
            if( value )
            {
                myTurn = value;
            }
        }


        DelayedTurnPtr OrnamentsChoice::getDelayedTurn() const
        {
            return myDelayedTurn;
        }


        void OrnamentsChoice::setDelayedTurn( const DelayedTurnPtr& value )
        {
            if( value )
            {
                myDelayedTurn = value;
            }
        }


        InvertedTurnPtr OrnamentsChoice::getInvertedTurn() const
        {
            return myInvertedTurn;
        }


        void OrnamentsChoice::setInvertedTurn( const InvertedTurnPtr& value )
        {
            if( value )
            {
                myInvertedTurn = value;
            }
        }


        DelayedInvertedTurnPtr OrnamentsChoice::getDelayedInvertedTurn() const
        {
            return myDelayedInvertedTurn;
        }


        void OrnamentsChoice::setDelayedInvertedTurn( const DelayedInvertedTurnPtr& value )
        {
            if( value )
            {
                myDelayedInvertedTurn = value;
            }
        }


        VerticalTurnPtr OrnamentsChoice::getVerticalTurn() const
        {
            return myVerticalTurn;
        }


        void OrnamentsChoice::setVerticalTurn( const VerticalTurnPtr& value )
        {
            if( value )
            {
                myVerticalTurn = value;
            }
        }


        ShakePtr OrnamentsChoice::getShake() const
        {
            return myShake;
        }


        void OrnamentsChoice::setShake( const ShakePtr& value )
        {
            if( value )
            {
                myShake = value;
            }
        }


        WavyLinePtr OrnamentsChoice::getWavyLine() const
        {
            return myWavyLine;
        }


        void OrnamentsChoice::setWavyLine( const WavyLinePtr& value )
        {
            if( value )
            {
                myWavyLine = value;
            }
        }


        MordentPtr OrnamentsChoice::getMordent() const
        {
            return myMordent;
        }


        void OrnamentsChoice::setMordent( const MordentPtr& value )
        {
            if( value )
            {
                myMordent = value;
            }
        }


        InvertedMordentPtr OrnamentsChoice::getInvertedMordent() const
        {
            return myInvertedMordent;
        }


        void OrnamentsChoice::setInvertedMordent( const InvertedMordentPtr& value )
        {
            if( value )
            {
                myInvertedMordent = value;
            }
        }


        SchleiferPtr OrnamentsChoice::getSchleifer() const
        {
            return mySchleifer;
        }


        void OrnamentsChoice::setSchleifer( const SchleiferPtr& value )
        {
            if( value )
            {
                mySchleifer = value;
            }
        }


        TremoloPtr OrnamentsChoice::getTremolo() const
        {
            return myTremolo;
        }


        void OrnamentsChoice::setTremolo( const TremoloPtr& value )
        {
            if( value )
            {
                myTremolo = value;
            }
        }


        OtherOrnamentPtr OrnamentsChoice::getOtherOrnament() const
        {
            return myOtherOrnament;
        }


        void OrnamentsChoice::setOtherOrnament( const OtherOrnamentPtr& value )
        {
            if( value )
            {
                myOtherOrnament = value;
            }
        }


        bool OrnamentsChoice::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            
            const std::string elementName = xelement.getName();
            
            if( elementName == "trill-mark" )
            {
                myChoice = Choice::trillMark;
                isSuccess &= getTrillMark()->fromXElement( message, xelement );
            }
            else if (  elementName == "turn" )
            {
                myChoice = Choice::turn;
                isSuccess &= getTurn()->fromXElement( message, xelement );
            }
            else if (  elementName == "delayed-turn" )
            {
                myChoice = Choice::delayedTurn;
                isSuccess &= getDelayedTurn()->fromXElement( message, xelement );
            }
            else if (  elementName == "inverted-turn" )
            {
                myChoice = Choice::invertedTurn;
                isSuccess &= getInvertedTurn()->fromXElement( message, xelement );
            }
            else if (  elementName == "delayed-inverted-turn" )
            {
                myChoice = Choice::delayedInvertedTurn;
                isSuccess &= getDelayedInvertedTurn()->fromXElement( message, xelement );
            }
            else if (  elementName == "vertical-turn" )
            {
                myChoice = Choice::verticalTurn;
                isSuccess &= getVerticalTurn()->fromXElement( message, xelement );
            }
            else if (  elementName == "shake" )
            {
                myChoice = Choice::shake;
                isSuccess &= getShake()->fromXElement( message, xelement );
            }
            else if (  elementName == "wavy-line" )
            {
                myChoice = Choice::wavyLine;
                isSuccess &= getWavyLine()->fromXElement( message, xelement );
            }
            else if (  elementName == "mordent" )
            {
                myChoice = Choice::mordent;
                isSuccess &= getMordent()->fromXElement( message, xelement );
            }
            else if (  elementName == "inverted-mordent" )
            {
                myChoice = Choice::invertedMordent;
                isSuccess &= getInvertedMordent()->fromXElement( message, xelement );
            }
            else if (  elementName == "schleifer" )
            {
                myChoice = Choice::schleifer;
                isSuccess &= getSchleifer()->fromXElement( message, xelement );
            }
            else if (  elementName == "tremolo" )
            {
                myChoice = Choice::tremolo;
                isSuccess &= getTremolo()->fromXElement( message, xelement );
            }
            else if (  elementName == "other-ornament" )
            {
                myChoice = Choice::otherOrnament;
                isSuccess &= getOtherOrnament()->fromXElement( message, xelement );
            }
            else
            {
                message << "OrnamentsChoice::fromXElement unrecognized element '" << elementName << "'" << std::endl;
                isSuccess = false;
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
