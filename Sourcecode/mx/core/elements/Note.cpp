// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Note.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Grace.h"
#include "mx/core/elements/Cue.h"
#include "mx/core/elements/Instrument.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Lyric.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/NoteheadText.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Play.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Stem.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Note::Note()
        :myAttributes( std::make_shared<NoteAttributes>() )
        ,myNoteChoice( makeNoteChoice() )
        ,myInstrument( makeInstrument() )
        ,myHasInstrument( false )
        ,myEditorialVoiceGroup( makeEditorialVoiceGroup() )
        ,myType( makeType() )
        ,myHasType( false )
        ,myDotSet()
        ,myAccidental( makeAccidental() )
        ,myHasAccidental( false )
        ,myTimeModification( makeTimeModification() )
        ,myHasTimeModification( false )
        ,myStem( makeStem() )
        ,myHasStem( false )
        ,myNotehead( makeNotehead() )
        ,myHasNotehead( false )
        ,myNoteheadText( makeNoteheadText() )
        ,myHasNoteheadText( false )
        ,myStaff( makeStaff() )
        ,myHasStaff( false )
        ,myBeamSet()
        ,myNotationsSet()
        ,myLyricSet()
        ,myPlay( makePlay() )
        ,myHasPlay( false )
        {}


        bool Note::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Note::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Note::streamName( std::ostream& os ) const
        {
            os << "note";
            return os;
        }


        bool Note::hasContents() const
        {
            return true;
        }


        std::ostream& Note::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myNoteChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            if ( myHasInstrument )
            {
                os << std::endl;
                myInstrument->toStream( os, indentLevel+1 );
            }
            if ( myEditorialVoiceGroup->hasContents() )
            {
                os << std::endl;
                myEditorialVoiceGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasType )
            {
                os << std::endl;
                myType->toStream( os, indentLevel+1 );
            }
            for ( auto x : myDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasAccidental )
            {
                os << std::endl;
                myAccidental->toStream( os, indentLevel+1 );
            }
            if ( myHasTimeModification )
            {
                os << std::endl;
                myTimeModification->toStream( os, indentLevel+1 );
            }
            if ( myHasStem )
            {
                os << std::endl;
                myStem->toStream( os, indentLevel+1 );
            }
            if ( myHasNotehead )
            {
                os << std::endl;
                myNotehead->toStream( os, indentLevel+1 );
            }
            if ( myHasNoteheadText )
            {
                os << std::endl;
                myNoteheadText->toStream( os, indentLevel+1 );
            }
            if ( myHasStaff )
            {
                os << std::endl;
                myStaff->toStream( os, indentLevel+1 );
            }
            for ( auto x : myBeamSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myNotationsSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myLyricSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasPlay )
            {
                os << std::endl;
                myPlay->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        NoteAttributesPtr Note::getAttributes() const
        {
            return myAttributes;
        }


        void Note::setAttributes( const NoteAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NoteChoicePtr Note::getNoteChoice() const
        {
            return myNoteChoice;
        }


        void Note::setNoteChoice( const NoteChoicePtr& value )
        {
            if( value )
            {
                myNoteChoice = value;
            }
        }


        InstrumentPtr Note::getInstrument() const
        {
            return myInstrument;
        }


        void Note::setInstrument( const InstrumentPtr& value )
        {
            if( value )
            {
                myInstrument = value;
            }
        }


        bool Note::getHasInstrument() const
        {
            return myHasInstrument;
        }


        void Note::setHasInstrument( const bool value )
        {
            myHasInstrument = value;
        }


        EditorialVoiceGroupPtr Note::getEditorialVoiceGroup() const
        {
            return myEditorialVoiceGroup;
        }


        void Note::setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value )
        {
            if ( value )
            {
                myEditorialVoiceGroup = value;
            }
        }


        TypePtr Note::getType() const
        {
            return myType;
        }


        void Note::setType( const TypePtr& value )
        {
            if( value )
            {
                myType = value;
            }
        }


        bool Note::getHasType() const
        {
            return myHasType;
        }


        void Note::setHasType( const bool value )
        {
            myHasType = value;
        }


        const DotSet& Note::getDotSet() const
        {
            return myDotSet;
        }


        void Note::removeDot( const DotSetIterConst& value )
        {
            if ( value != myDotSet.cend() )
            {
                myDotSet.erase( value );
            }
        }


        void Note::addDot( const DotPtr& value )
        {
            if ( value )
            {
                myDotSet.push_back( value );
            }
        }


        void Note::clearDotSet()
        {
            myDotSet.clear();
        }


        DotPtr Note::getDot( const DotSetIterConst& setIterator ) const
        {
            if( setIterator != myDotSet.cend() )
            {
                return *setIterator;
            }
            return DotPtr();
        }


        AccidentalPtr Note::getAccidental() const
        {
            return myAccidental;
        }


        void Note::setAccidental( const AccidentalPtr& value )
        {
            if( value )
            {
                myAccidental = value;
            }
        }


        bool Note::getHasAccidental() const
        {
            return myHasAccidental;
        }


        void Note::setHasAccidental( const bool value )
        {
            myHasAccidental = value;
        }


        TimeModificationPtr Note::getTimeModification() const
        {
            return myTimeModification;
        }


        void Note::setTimeModification( const TimeModificationPtr& value )
        {
            if( value )
            {
                myTimeModification = value;
            }
        }


        bool Note::getHasTimeModification() const
        {
            return myHasTimeModification;
        }


        void Note::setHasTimeModification( const bool value )
        {
            myHasTimeModification = value;
        }


        StemPtr Note::getStem() const
        {
            return myStem;
        }


        void Note::setStem( const StemPtr& value )
        {
            if( value )
            {
                myStem = value;
            }
        }


        bool Note::getHasStem() const
        {
            return myHasStem;
        }


        void Note::setHasStem( const bool value )
        {
            myHasStem = value;
        }


        NoteheadPtr Note::getNotehead() const
        {
            return myNotehead;
        }


        void Note::setNotehead( const NoteheadPtr& value )
        {
            if( value )
            {
                myNotehead = value;
            }
        }


        bool Note::getHasNotehead() const
        {
            return myHasNotehead;
        }


        void Note::setHasNotehead( const bool value )
        {
            myHasNotehead = value;
        }


        NoteheadTextPtr Note::getNoteheadText() const
        {
            return myNoteheadText;
        }


        void Note::setNoteheadText( const NoteheadTextPtr& value )
        {
            if( value )
            {
                myNoteheadText = value;
            }
        }


        bool Note::getHasNoteheadText() const
        {
            return myHasNoteheadText;
        }


        void Note::setHasNoteheadText( const bool value )
        {
            myHasNoteheadText = value;
        }


        StaffPtr Note::getStaff() const
        {
            return myStaff;
        }


        void Note::setStaff( const StaffPtr& value )
        {
            if ( value )
            {
                myStaff = value;
            }
        }


        bool Note::getHasStaff() const
        {
            return myHasStaff;
        }


        void Note::setHasStaff( const bool value )
        {
            myHasStaff = value;
        }


        const BeamSet& Note::getBeamSet() const
        {
            return myBeamSet;
        }


        void Note::removeBeam( const BeamSetIterConst& value )
        {
            if ( value != myBeamSet.cend() )
            {
                myBeamSet.erase( value );
            }
        }


        void Note::addBeam( const BeamPtr& value )
        {
            if ( value )
            {
                if ( myBeamSet.size() < 8 )
                {
                    myBeamSet.push_back( value );
                }
            }
        }


        void Note::clearBeamSet()
        {
            myBeamSet.clear();
        }


        BeamPtr Note::getBeam( const BeamSetIterConst& setIterator ) const
        {
            if( setIterator != myBeamSet.cend() )
            {
                return *setIterator;
            }
            return BeamPtr();
        }


        const NotationsSet& Note::getNotationsSet() const
        {
            return myNotationsSet;
        }


        void Note::removeNotations( const NotationsSetIterConst& value )
        {
            if ( value != myNotationsSet.cend() )
            {
                myNotationsSet.erase( value );
            }
        }


        void Note::addNotations( const NotationsPtr& value )
        {
            if ( value )
            {
                myNotationsSet.push_back( value );
            }
        }


        void Note::clearNotationsSet()
        {
            myNotationsSet.clear();
        }


        NotationsPtr Note::getNotations( const NotationsSetIterConst& setIterator ) const
        {
            if( setIterator != myNotationsSet.cend() )
            {
                return *setIterator;
            }
            return NotationsPtr();
        }


        const LyricSet& Note::getLyricSet() const
        {
            return myLyricSet;
        }


        void Note::removeLyric( const LyricSetIterConst& value )
        {
            if ( value != myLyricSet.cend() )
            {
                myLyricSet.erase( value );
            }
        }


        void Note::addLyric( const LyricPtr& value )
        {
            if ( value )
            {
                myLyricSet.push_back( value );
            }
        }


        void Note::clearLyricSet()
        {
            myLyricSet.clear();
        }


        LyricPtr Note::getLyric( const LyricSetIterConst& setIterator ) const
        {
            if( setIterator != myLyricSet.cend() )
            {
                return *setIterator;
            }
            return LyricPtr();
        }


        PlayPtr Note::getPlay() const
        {
            return myPlay;
        }


        void Note::setPlay( const PlayPtr& value )
        {
            if( value )
            {
                myPlay = value;
            }
        }


        bool Note::getHasPlay() const
        {
            return myHasPlay;
        }


        void Note::setHasPlay( const bool value )
        {
            myHasPlay = value;
        }
        
        
        bool Note::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isNoteChoiceFound = false;
            bool isFirstDotAdded = false;
            bool isFirstBeamAdded = false;
            bool isFirstNotationsAdded = false;
            bool isFirstLyricAdded = false;
            
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if(   elementName == "pitch"
                   || elementName == "unpitched"
                   || elementName == "rest"
                   || elementName == "chord"
                   || elementName == "grace"
                   || elementName == "cue" )
                {
                    isNoteChoiceFound = true;
                    isSuccess &= parseNoteChoice( message, xelement, it );
                    --it;
                }
                else if( elementName == "instrument" )
                {
                    myHasInstrument = true;
                    isSuccess &= myInstrument->fromXElement( message, *it );
                }
                else if(   elementName == "footnote"
                        || elementName == "level"
                        || elementName == "voice" )
                {
                    isSuccess &= parseEditorialVoiceGroup( message, xelement, it );
                }
                else if( elementName == "type" )
                {
                    myHasType = true;
                    isSuccess &= myType->fromXElement( message, *it );
                }
                else if( elementName == "dot" )
                {
                    auto dot = makeDot();
                    isSuccess &= dot->fromXElement( message, *it );
                    
                    if( !isFirstDotAdded && myDotSet.size() == 1 )
                    {
                        *( myDotSet.begin() ) = dot;
                        isFirstDotAdded = true;
                    }
                    else
                    {
                        myDotSet.push_back( dot );
                        isFirstDotAdded = true;
                    }
                }
                else if( elementName == "accidental" )
                {
                    myHasAccidental = true;
                    isSuccess &= myAccidental->fromXElement( message, *it );
                }
                else if( elementName == "time-modification" )
                {
                    myHasTimeModification = true;
                    isSuccess &= myTimeModification->fromXElement( message, *it );
                }
                else if( elementName == "stem" )
                {
                    myHasStem = true;
                    isSuccess &= myStem->fromXElement( message, *it );
                }
                else if( elementName == "notehead" )
                {
                    myHasNotehead = true;
                    isSuccess &= myNotehead->fromXElement( message, *it );
                }
                else if( elementName == "notehead-text" )
                {
                    myHasNoteheadText = true;
                    isSuccess &= myNoteheadText->fromXElement( message, *it );
                }
                else if( elementName == "staff" )
                {
                    myHasStaff = true;
                    isSuccess &= myStaff->fromXElement( message, *it );
                }
                else if( elementName == "beam" )
                {
                    auto beam = makeBeam();
                    isSuccess &= beam->fromXElement( message, *it );
                    
                    if( !isFirstBeamAdded && myBeamSet.size() == 1 )
                    {
                        *( myBeamSet.begin() ) = beam;
                        isFirstBeamAdded = true;
                    }
                    else
                    {
                        myBeamSet.push_back( beam );
                        isFirstBeamAdded = true;
                    }
                }
                else if( elementName == "notations" )
                {
                    auto notations = makeNotations();
                    isSuccess &= notations->fromXElement( message, *it );
                    
                    if( !isFirstNotationsAdded && myNotationsSet.size() == 1 )
                    {
                        *( myNotationsSet.begin() ) = notations;
                        isFirstNotationsAdded = true;
                    }
                    else
                    {
                        myNotationsSet.push_back( notations );
                        isFirstNotationsAdded = true;
                    }
                }
                else if( elementName == "lyric" )
                {
                    auto lyric = makeLyric();
                    isSuccess &= lyric->fromXElement( message, *it );
                    
                    if( !isFirstLyricAdded && myLyricSet.size() == 1 )
                    {
                        *( myLyricSet.begin() ) = lyric;
                        isFirstLyricAdded = true;
                    }
                    else
                    {
                        myLyricSet.push_back( lyric );
                        isFirstLyricAdded = true;
                    }
                }
                else if( elementName == "play" )
                {
                    myHasPlay = true;
                    isSuccess &= myPlay->fromXElement( message, *it );
                }
                else
                {
                    isSuccess = false;
                    message << "Note: unexpected element '" << elementName << "'" << std::endl;
                }
            }
            
            if( !isNoteChoiceFound )
            {
                isSuccess = false;
                message << "Note: 'note-choice' elements were required but not found" << std::endl;
            }
            
            MX_RETURN_IS_SUCCESS;
        }

        
        bool Note::parseNoteChoice( std::ostream& message, xml::XElement& noteElement, xml::XElementIterator& iter )
        {
            if( iter == noteElement.end() )
            {
                return false;
            }
            
            bool isSuccess = true;
            
            
            const std::string elementName = iter->getName();
            FullNoteGroupPtr fullNoteGroup = nullptr;
            
            if( elementName == "grace" )
            {
                myNoteChoice->setChoice( NoteChoice::Choice::grace );
                isSuccess &= myNoteChoice->getGraceNoteGroup()->getGrace()->fromXElement( message, *iter );
                fullNoteGroup = myNoteChoice->getGraceNoteGroup()->getFullNoteGroup();
                ++iter;
            }
            else if ( elementName == "cue" )
            {
                myNoteChoice->setChoice( NoteChoice::Choice::cue );
                isSuccess &= myNoteChoice->getCueNoteGroup()->getCue()->fromXElement( message, *iter );
                fullNoteGroup = myNoteChoice->getCueNoteGroup()->getFullNoteGroup();
                ++iter;
            }
            else
            {
                myNoteChoice->setChoice( NoteChoice::Choice::normal );
                fullNoteGroup = myNoteChoice->getNormalNoteGroup()->getFullNoteGroup();
            }
            
            // we should now be pointing at the full note group
            isSuccess &= parseFullNoteGroup( message, noteElement, iter, fullNoteGroup );
            
            // NormalNoteGroups and CueNoteGroups require a duration element
            if( elementName != "grace" )
            {
                if( iter == noteElement.end() || iter->getName() != "duration" )
                {
                    message << "Note: parseNoteChoice - a 'duration' element was required but not found" << std::endl;
                    return false;
                }
                
                if( myNoteChoice->getChoice() == NoteChoice::Choice::normal )
                {
                    myNoteChoice->getNormalNoteGroup()->getDuration()->fromXElement( message, *iter );
                }
                else if( myNoteChoice->getChoice() == NoteChoice::Choice::cue )
                {
                    myNoteChoice->getCueNoteGroup()->getDuration()->fromXElement( message, *iter );
                }
                ++iter;
            }
            
            // additional stuff is optional so we may be at the end iter
            if( iter == noteElement.end() )
            {
                MX_RETURN_IS_SUCCESS;
            }
            
            // now we may be pointing at tie elements, but only if the choice is normal or grace
            if( myNoteChoice->getChoice() == NoteChoice::Choice::normal || myNoteChoice->getChoice() == NoteChoice::Choice::grace  )
            {
                std::string possibleTieElementName = iter->getName();
                while( iter != noteElement.end() && iter->getName() == "tie" )
                {
                    auto tie = makeTie();
                    isSuccess &= tie->fromXElement( message, *iter );
                    if( myNoteChoice->getChoice() == NoteChoice::Choice::normal )
                    {
                        myNoteChoice->getNormalNoteGroup()->addTie( tie );
                    }
                    else if ( myNoteChoice->getChoice() == NoteChoice::Choice::grace )
                    {
                        myNoteChoice->getGraceNoteGroup()->addTie( tie );
                    }
                    ++iter;
                }
            }
            else if( myNoteChoice->getChoice() == NoteChoice::Choice::cue )
            {
                MX_RETURN_IS_SUCCESS;
            }
            
            
            MX_RETURN_IS_SUCCESS;
        }
        
        
        bool Note::parseFullNoteGroup( std::ostream& message, xml::XElement& noteElement, xml::XElementIterator& iter, FullNoteGroupPtr& outFullNoteGroup )
        {
            if( iter == noteElement.end() )
            {
                return false;
            }
            
            bool isSuccess = true;
            
            if( iter->getName() == "chord" )
            {
                outFullNoteGroup->setHasChord( true );
                ++iter;
            }
            
            // we should now be pointing at the FullNoteTypeChoice
            if( iter == noteElement.end() )
            {
                message << "Note: parseFullNoteGroup did not find the FullNoteChoice" << std::endl;
                return false;
            }
            
            const std::string noteChoiceElementName = iter->getName();
            
            if( noteChoiceElementName == "pitch" )
            {
                outFullNoteGroup->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
                isSuccess &= outFullNoteGroup->getFullNoteTypeChoice()->getPitch()->fromXElement( message, *iter );
                ++iter;
            }
            else if( noteChoiceElementName == "unpitched" )
            {
                outFullNoteGroup->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::unpitched );
                isSuccess &= outFullNoteGroup->getFullNoteTypeChoice()->getUnpitched()->fromXElement( message, *iter );
                ++iter;
            }
            else if( noteChoiceElementName == "rest" )
            {
                outFullNoteGroup->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::rest );
                isSuccess &= outFullNoteGroup->getFullNoteTypeChoice()->getRest()->fromXElement( message, *iter );
                ++iter;
            }
            else
            {
                message << "Note: parseFullNoteGroup encountered unexpected element '" << noteChoiceElementName << "'" << std::endl;
                isSuccess = false;
                ++iter;
            }
            
            MX_RETURN_IS_SUCCESS;
        }
        
        
        bool Note::parseEditorialVoiceGroup( std::ostream& message, xml::XElement& noteElement, xml::XElementIterator& iter )
        {
            bool isSuccess = true;
            bool isIterIncremented = false;
            while( iter != noteElement.end() && ( iter->getName() == "footnote" || iter->getName() == "level" || iter->getName() == "voice" ) )
            {
                if( iter->getName() == "footnote" )
                {
                    myEditorialVoiceGroup->setHasFootnote( true );
                    isSuccess &= myEditorialVoiceGroup->getFootnote()->fromXElement( message, *iter );
                }
                else if ( iter->getName() == "level" )
                {
                    myEditorialVoiceGroup->setHasLevel( true );
                    isSuccess &= myEditorialVoiceGroup->getLevel()->fromXElement( message, *iter );
                }
                else if ( iter->getName() == "voice" )
                {
                    myEditorialVoiceGroup->setHasVoice( true );
                    isSuccess &= myEditorialVoiceGroup->getVoice()->fromXElement( message, *iter );
                }
                
                ++iter;
                isIterIncremented = true;
            }
            
            if( isIterIncremented )
            {
                --iter;
            }
            MX_RETURN_IS_SUCCESS;
        }
    }
}
