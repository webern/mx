// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/DirectionType.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DirectionType::DirectionType()
        :myChoice( Choice::rehearsal )
        ,myRehearsalSet()
        ,mySegnoSet()
        ,myWordsSet()
        ,myCodaSet()
        ,myWedge( makeWedge() )
        ,myDynamicsSet()
        ,myDashes( makeDashes() )
        ,myBracket( makeBracket() )
        ,myPedal( makePedal() )
        ,myMetronome( makeMetronome() )
        ,myOctaveShift( makeOctaveShift() )
        ,myHarpPedals( makeHarpPedals() )
        ,myDamp( makeDamp() )
        ,myDampAll( makeDampAll() )
        ,myEyeglasses( makeEyeglasses() )
        ,myStringMute( makeStringMute() )
        ,myScordatura( makeScordatura() )
        ,myImage( makeImage() )
        ,myPrincipalVoice( makePrincipalVoice() )
        ,myAccordionRegistration( makeAccordionRegistration() )
        ,myPercussionSet()
        ,myOtherDirection( makeOtherDirection() )
        {
            myRehearsalSet.push_back( makeRehearsal() );
            mySegnoSet.push_back( makeSegno() );
            myWordsSet.push_back( makeWords() );
            myCodaSet.push_back( makeCoda() );
            myDynamicsSet.push_back( makeDynamics() );
            myPercussionSet.push_back( makePercussion() );
        }


        bool DirectionType::hasAttributes() const
        {
            return false;
        }


        std::ostream& DirectionType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& DirectionType::streamName( std::ostream& os ) const
        {
            os << "direction-type";
            return os;
        }


        bool DirectionType::hasContents() const
        {
            return true;
        }


        std::ostream& DirectionType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            //streamOpenTag( os );
            //os << std::endl;
            switch ( myChoice )
            {
                case Choice::rehearsal:
                {
                    for( auto x : myRehearsalSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::segno:
                {
                    for( auto x : mySegnoSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::words:
                {
                    for( auto x : myWordsSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::coda:
                {
                    for( auto x : myCodaSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::wedge:
                {
                    os << std::endl;
                    myWedge->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::dynamics:
                {
                    for( auto x : myDynamicsSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::dashes:
                {
                    os << std::endl;
                    myDashes->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::bracket:
                {
                    os << std::endl;
                    myBracket->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::pedal:
                {
                    os << std::endl;
                    myPedal->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::metronome:
                {
                    os << std::endl;
                    myMetronome->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::octaveShift:
                {
                    os << std::endl;
                    myOctaveShift->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::harpPedals:
                {
                    os << std::endl;
                    myHarpPedals->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::damp:
                {
                    os << std::endl;
                    myDamp->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::dampAll:
                {
                    os << std::endl;
                    myDampAll->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::eyeglasses:
                {
                    os << std::endl;
                    myEyeglasses->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::stringMute:
                {
                    os << std::endl;
                    myStringMute->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::scordatura:
                {
                    os << std::endl;
                    myScordatura->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::image:
                {
                    os << std::endl;
                    myImage->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::principalVoice:
                {
                    os << std::endl;
                    myPrincipalVoice->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::accordionRegistration:
                {
                    os << std::endl;
                    myAccordionRegistration->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::percussion:
                {
                    for( auto x : myPercussionSet )
                    {
                        os << std::endl;
                        x->toStream( os, indentLevel+1 );
                    }
                }
                    break;
                case Choice::otherDirection:
                {
                    os << std::endl;
                    myOtherDirection->toStream( os, indentLevel+1 );
                }
                    break;
                default:
                    break;
            }
            //os << std::endl;
            //streamCloseTag( os );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        DirectionType::Choice DirectionType::getChoice() const
        {
            return myChoice;
        }


        void DirectionType::setChoice( const DirectionType::Choice value )
        {
            myChoice = value;
        }


        const RehearsalSet& DirectionType::getRehearsalSet() const
        {
            return myRehearsalSet;
        }


        void DirectionType::removeRehearsal( const RehearsalSetIterConst& value )
        {
            if ( value != myRehearsalSet.cend() )
            {
                if ( myRehearsalSet.size() > 1 )
                {
                    myRehearsalSet.erase( value );
                }
            }
        }


        void DirectionType::addRehearsal( const RehearsalPtr& value )
        {
            if ( value )
            {
                myRehearsalSet.push_back( value );
            }
        }


        void DirectionType::clearRehearsalSet()
        {
            myRehearsalSet.clear();
            while( myRehearsalSet.size() < 1 )
            {
                myRehearsalSet.push_back( makeRehearsal() );
            }
        }


        RehearsalPtr DirectionType::getRehearsal( const RehearsalSetIterConst& setIterator ) const
        {
            if( setIterator != myRehearsalSet.cend() )
            {
                return *setIterator;
            }
            return RehearsalPtr();
        }


        const SegnoSet& DirectionType::getSegnoSet() const
        {
            return mySegnoSet;
        }


        void DirectionType::removeSegno( const SegnoSetIterConst& value )
        {
            if ( value != mySegnoSet.cend() )
            {
                if ( mySegnoSet.size() > 1 )
                {
                    mySegnoSet.erase( value );
                }
            }
        }


        void DirectionType::addSegno( const SegnoPtr& value )
        {
            if ( value )
            {
                mySegnoSet.push_back( value );
            }
        }


        void DirectionType::clearSegnoSet()
        {
            mySegnoSet.clear();
            while( mySegnoSet.size() < 1 )
            {
                mySegnoSet.push_back( makeSegno() );
            }
        }


        SegnoPtr DirectionType::getSegno( const SegnoSetIterConst& setIterator ) const
        {
            if( setIterator != mySegnoSet.cend() )
            {
                return *setIterator;
            }
            return SegnoPtr();
        }


        const WordsSet& DirectionType::getWordsSet() const
        {
            return myWordsSet;
        }


        void DirectionType::removeWords( const WordsSetIterConst& value )
        {
            if ( value != myWordsSet.cend() )
            {
                if ( myWordsSet.size() > 1 )
                {
                    myWordsSet.erase( value );
                }
            }
        }


        void DirectionType::addWords( const WordsPtr& value )
        {
            if ( value )
            {
                myWordsSet.push_back( value );
            }
        }


        void DirectionType::clearWordsSet()
        {
            myWordsSet.clear();
            while( myWordsSet.size() < 1 )
            {
                myWordsSet.push_back( makeWords() );
            }
        }


        WordsPtr DirectionType::getWords( const WordsSetIterConst& setIterator ) const
        {
            if( setIterator != myWordsSet.cend() )
            {
                return *setIterator;
            }
            return WordsPtr();
        }


        const CodaSet& DirectionType::getCodaSet() const
        {
            return myCodaSet;
        }


        void DirectionType::removeCoda( const CodaSetIterConst& value )
        {
            if ( value != myCodaSet.cend() )
            {
                if ( myCodaSet.size() > 1 )
                {
                    myCodaSet.erase( value );
                }
            }
        }


        void DirectionType::addCoda( const CodaPtr& value )
        {
            if ( value )
            {
                myCodaSet.push_back( value );
            }
        }


        void DirectionType::clearCodaSet()
        {
            myCodaSet.clear();
            while( myCodaSet.size() < 1 )
            {
                myCodaSet.push_back( makeCoda() );
            }
        }


        CodaPtr DirectionType::getCoda( const CodaSetIterConst& setIterator ) const
        {
            if( setIterator != myCodaSet.cend() )
            {
                return *setIterator;
            }
            return CodaPtr();
        }


        WedgePtr DirectionType::getWedge() const
        {
            return myWedge;
        }


        void DirectionType::setWedge( const WedgePtr& value )
        {
            if( value )
            {
                myWedge = value;
            }
        }


        const DynamicsSet& DirectionType::getDynamicsSet() const
        {
            return myDynamicsSet;
        }


        void DirectionType::removeDynamics( const DynamicsSetIterConst& value )
        {
            if ( value != myDynamicsSet.cend() )
            {
                if ( myDynamicsSet.size() > 1 )
                {
                    myDynamicsSet.erase( value );
                }
            }
        }


        void DirectionType::addDynamics( const DynamicsPtr& value )
        {
            if ( value )
            {
                myDynamicsSet.push_back( value );
            }
        }


        void DirectionType::clearDynamicsSet()
        {
            myDynamicsSet.clear();
            while( myDynamicsSet.size() < 1 )
            {
                myDynamicsSet.push_back( makeDynamics() );
            }
        }


        DynamicsPtr DirectionType::getDynamics( const DynamicsSetIterConst& setIterator ) const
        {
            if( setIterator != myDynamicsSet.cend() )
            {
                return *setIterator;
            }
            return DynamicsPtr();
        }


        DashesPtr DirectionType::getDashes() const
        {
            return myDashes;
        }


        void DirectionType::setDashes( const DashesPtr& value )
        {
            if( value )
            {
                myDashes = value;
            }
        }


        BracketPtr DirectionType::getBracket() const
        {
            return myBracket;
        }


        void DirectionType::setBracket( const BracketPtr& value )
        {
            if( value )
            {
                myBracket = value;
            }
        }


        PedalPtr DirectionType::getPedal() const
        {
            return myPedal;
        }


        void DirectionType::setPedal( const PedalPtr& value )
        {
            if( value )
            {
                myPedal = value;
            }
        }


        MetronomePtr DirectionType::getMetronome() const
        {
            return myMetronome;
        }


        void DirectionType::setMetronome( const MetronomePtr& value )
        {
            if( value )
            {
                myMetronome = value;
            }
        }


        OctaveShiftPtr DirectionType::getOctaveShift() const
        {
            return myOctaveShift;
        }


        void DirectionType::setOctaveShift( const OctaveShiftPtr& value )
        {
            if( value )
            {
                myOctaveShift = value;
            }
        }


        HarpPedalsPtr DirectionType::getHarpPedals() const
        {
            return myHarpPedals;
        }


        void DirectionType::setHarpPedals( const HarpPedalsPtr& value )
        {
            if( value )
            {
                myHarpPedals = value;
            }
        }


        DampPtr DirectionType::getDamp() const
        {
            return myDamp;
        }


        void DirectionType::setDamp( const DampPtr& value )
        {
            if( value )
            {
                myDamp = value;
            }
        }


        DampAllPtr DirectionType::getDampAll() const
        {
            return myDampAll;
        }


        void DirectionType::setDampAll( const DampAllPtr& value )
        {
            if( value )
            {
                myDampAll = value;
            }
        }


        EyeglassesPtr DirectionType::getEyeglasses() const
        {
            return myEyeglasses;
        }


        void DirectionType::setEyeglasses( const EyeglassesPtr& value )
        {
            if( value )
            {
                myEyeglasses = value;
            }
        }


        StringMutePtr DirectionType::getStringMute() const
        {
            return myStringMute;
        }


        void DirectionType::setStringMute( const StringMutePtr& value )
        {
            if( value )
            {
                myStringMute = value;
            }
        }


        ScordaturaPtr DirectionType::getScordatura() const
        {
            return myScordatura;
        }


        void DirectionType::setScordatura( const ScordaturaPtr& value )
        {
            if( value )
            {
                myScordatura = value;
            }
        }


        ImagePtr DirectionType::getImage() const
        {
            return myImage;
        }


        void DirectionType::setImage( const ImagePtr& value )
        {
            if( value )
            {
                myImage = value;
            }
        }


        PrincipalVoicePtr DirectionType::getPrincipalVoice() const
        {
            return myPrincipalVoice;
        }


        void DirectionType::setPrincipalVoice( const PrincipalVoicePtr& value )
        {
            if( value )
            {
                myPrincipalVoice = value;
            }
        }


        AccordionRegistrationPtr DirectionType::getAccordionRegistration() const
        {
            return myAccordionRegistration;
        }


        void DirectionType::setAccordionRegistration( const AccordionRegistrationPtr& value )
        {
            if( value )
            {
                myAccordionRegistration = value;
            }
        }


        const PercussionSet& DirectionType::getPercussionSet() const
        {
            return myPercussionSet;
        }


        void DirectionType::removePercussion( const PercussionSetIterConst& value )
        {
            if ( value != myPercussionSet.cend() )
            {
                if ( myPercussionSet.size() > 1 )
                {
                    myPercussionSet.erase( value );
                }
            }
        }


        void DirectionType::addPercussion( const PercussionPtr& value )
        {
            if ( value )
            {
                myPercussionSet.push_back( value );
            }
        }


        void DirectionType::clearPercussionSet()
        {
            myPercussionSet.clear();
            while( myPercussionSet.size() < 1 )
            {
                myPercussionSet.push_back( makePercussion() );
            }
        }


        PercussionPtr DirectionType::getPercussion( const PercussionSetIterConst& setIterator ) const
        {
            if( setIterator != myPercussionSet.cend() )
            {
                return *setIterator;
            }
            return PercussionPtr();
        }


        OtherDirectionPtr DirectionType::getOtherDirection() const
        {
            return myOtherDirection;
        }


        void DirectionType::setOtherDirection( const OtherDirectionPtr& value )
        {
            if( value )
            {
                myOtherDirection = value;
            }
        }


        MX_FROM_XELEMENT_UNUSED( DirectionType );

    }
}
