// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
        :myAttributes( nullptr )
        ,myNoteChoice( nullptr )
        ,myInstrument( nullptr )
        ,myHasInstrument( false )
        ,myEditorialVoiceGroup( nullptr )
        ,myType( nullptr )
        ,myHasType( false )
        ,myDotSet()
        ,myAccidental( nullptr )
        ,myHasAccidental( false )
        ,myTimeModification( nullptr )
        ,myHasTimeModification( false )
        ,myStem( nullptr )
        ,myHasStem( false )
        ,myNotehead( nullptr )
        ,myHasNotehead( false )
        ,myNoteheadText( nullptr )
        ,myHasNoteheadText( false )
        ,myStaff( nullptr )
        ,myHasStaff( false )
        ,myBeamSet()
        ,myNotationsSet()
        ,myLyricSet()
        ,myPlay( nullptr )
        ,myHasPlay( false )
        {}


        bool Note::hasAttributes() const
        {
            return getAttributes()->hasValues();
        }


        std::ostream& Note::streamAttributes( std::ostream& os ) const
        {
            return getAttributes()->toStream( os );
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
            getNoteChoice()->streamContents( os, indentLevel+1, isOneLineOnly );
            if ( myHasInstrument )
            {
                os << std::endl;
                getInstrument()->toStream( os, indentLevel+1 );
            }
            if ( getEditorialVoiceGroup()->hasContents() )
            {
                os << std::endl;
                getEditorialVoiceGroup()->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasType )
            {
                os << std::endl;
                getType()->toStream( os, indentLevel+1 );
            }
            for ( auto x : myDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasAccidental )
            {
                os << std::endl;
                getAccidental()->toStream( os, indentLevel+1 );
            }
            if ( myHasTimeModification )
            {
                os << std::endl;
                getTimeModification()->toStream( os, indentLevel+1 );
            }
            if ( myHasStem )
            {
                os << std::endl;
                getStem()->toStream( os, indentLevel+1 );
            }
            if ( myHasNotehead )
            {
                os << std::endl;
                getNotehead()->toStream( os, indentLevel+1 );
            }
            if ( myHasNoteheadText )
            {
                os << std::endl;
                getNoteheadText()->toStream( os, indentLevel+1 );
            }
            if ( myHasStaff )
            {
                os << std::endl;
                getStaff()->toStream( os, indentLevel+1 );
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
                getPlay()->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        NoteAttributesPtr Note::getAttributes() const
        {
            MX_LOCK;
            MX_JIT_ALLOCATE_ATTRIBUTES( NoteAttributes );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( NoteChoice );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Instrument );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( EditorialVoiceGroup );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Type );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Accidental );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( TimeModification );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Stem );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Notehead );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( NoteheadText );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Staff );
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
            MX_LOCK;
            MX_JIT_ALLOCATE( Play );
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
        
        
        bool Note::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isNoteChoiceFound = false;
            bool isFirstDotAdded = false;
            bool isFirstBeamAdded = false;
            bool isFirstNotationsAdded = false;
            bool isFirstLyricAdded = false;
            
            isSuccess &= getAttributes()->fromXElement( message, xelement );
            
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
                    isSuccess &= getInstrument()->fromXElement( message, *it );
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
                    isSuccess &= getType()->fromXElement( message, *it );
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
                    isSuccess &= getAccidental()->fromXElement( message, *it );
                }
                else if( elementName == "time-modification" )
                {
                    myHasTimeModification = true;
                    isSuccess &= getTimeModification()->fromXElement( message, *it );
                }
                else if( elementName == "stem" )
                {
                    myHasStem = true;
                    isSuccess &= getStem()->fromXElement( message, *it );
                }
                else if( elementName == "notehead" )
                {
                    myHasNotehead = true;
                    isSuccess &= getNotehead()->fromXElement( message, *it );
                }
                else if( elementName == "notehead-text" )
                {
                    myHasNoteheadText = true;
                    isSuccess &= getNoteheadText()->fromXElement( message, *it );
                }
                else if( elementName == "staff" )
                {
                    myHasStaff = true;
                    isSuccess &= getStaff()->fromXElement( message, *it );
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
                    isSuccess &= getPlay()->fromXElement( message, *it );
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

        
        bool Note::parseNoteChoice( std::ostream& message, ::ezxml::XElement& noteElement, ::ezxml::XElementIterator& iter )
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
                getNoteChoice()->setChoice( NoteChoice::Choice::grace );
                isSuccess &= getNoteChoice()->getGraceNoteGroup()->getGrace()->fromXElement( message, *iter );
                fullNoteGroup = getNoteChoice()->getGraceNoteGroup()->getFullNoteGroup();
                ++iter;
            }
            else if ( elementName == "cue" )
            {
                getNoteChoice()->setChoice( NoteChoice::Choice::cue );
                isSuccess &= getNoteChoice()->getCueNoteGroup()->getCue()->fromXElement( message, *iter );
                fullNoteGroup = getNoteChoice()->getCueNoteGroup()->getFullNoteGroup();
                ++iter;
            }
            else
            {
                getNoteChoice()->setChoice( NoteChoice::Choice::normal );
                fullNoteGroup = getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup();
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
                
                if( getNoteChoice()->getChoice() == NoteChoice::Choice::normal )
                {
                    getNoteChoice()->getNormalNoteGroup()->getDuration()->fromXElement( message, *iter );
                }
                else if( getNoteChoice()->getChoice() == NoteChoice::Choice::cue )
                {
                    getNoteChoice()->getCueNoteGroup()->getDuration()->fromXElement( message, *iter );
                }
                ++iter;
            }
            
            // additional stuff is optional so we may be at the end iter
            if( iter == noteElement.end() )
            {
                MX_RETURN_IS_SUCCESS;
            }
            
            // now we may be pointing at tie elements, but only if the choice is normal or grace
            if( getNoteChoice()->getChoice() == NoteChoice::Choice::normal || getNoteChoice()->getChoice() == NoteChoice::Choice::grace  )
            {
                std::string possibleTieElementName = iter->getName();
                while( iter != noteElement.end() && iter->getName() == "tie" )
                {
                    auto tie = makeTie();
                    isSuccess &= tie->fromXElement( message, *iter );
                    if( getNoteChoice()->getChoice() == NoteChoice::Choice::normal )
                    {
                        getNoteChoice()->getNormalNoteGroup()->addTie( tie );
                    }
                    else if ( getNoteChoice()->getChoice() == NoteChoice::Choice::grace )
                    {
                        getNoteChoice()->getGraceNoteGroup()->addTie( tie );
                    }
                    ++iter;
                }
            }
            else if( getNoteChoice()->getChoice() == NoteChoice::Choice::cue )
            {
                MX_RETURN_IS_SUCCESS;
            }
            
            
            MX_RETURN_IS_SUCCESS;
        }
        
        
        bool Note::parseFullNoteGroup( std::ostream& message, ::ezxml::XElement& noteElement, ::ezxml::XElementIterator& iter, FullNoteGroupPtr& outFullNoteGroup )
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
        
        
        bool Note::parseEditorialVoiceGroup( std::ostream& message, ::ezxml::XElement& noteElement, ::ezxml::XElementIterator& iter )
        {
            bool isSuccess = true;
            bool isIterIncremented = false;
            while( iter != noteElement.end() && ( iter->getName() == "footnote" || iter->getName() == "level" || iter->getName() == "voice" ) )
            {
                if( iter->getName() == "footnote" )
                {
                    getEditorialVoiceGroup()->setHasFootnote( true );
                    isSuccess &= getEditorialVoiceGroup()->getFootnote()->fromXElement( message, *iter );
                }
                else if ( iter->getName() == "level" )
                {
                    getEditorialVoiceGroup()->setHasLevel( true );
                    isSuccess &= getEditorialVoiceGroup()->getLevel()->fromXElement( message, *iter );
                }
                else if ( iter->getName() == "voice" )
                {
                    getEditorialVoiceGroup()->setHasVoice( true );
                    isSuccess &= getEditorialVoiceGroup()->getVoice()->fromXElement( message, *iter );
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
