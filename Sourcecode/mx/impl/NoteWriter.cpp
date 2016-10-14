// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NoteWriter.h"
#include "mx/core/elements/Note.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/ActualNotes.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/DisplayStep.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/NormalType.h"
#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Stem.h"

namespace mx
{
    namespace impl
    {
        NoteWriter::NoteWriter( const api::NoteData& inNoteData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter )
        : myNoteData{ inNoteData }
        , myCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myOutNote{ nullptr }
        , myOutNoteChoice( nullptr )
        , myOutFullNoteGroup( nullptr )
        , myConverter{}
        {
            
        }
        
        core::NotePtr NoteWriter::getNote() const
        {
            myOutNote = core::makeNote();
            setNoteChoiceAndFullNoteGroup();
            setFullNoteTypeChoice();
            setStaffAndVoice();
            setDurationNameAndDots();
            setStemDirection();
            return myOutNote;
        }
        
        
        void NoteWriter::setNoteChoiceAndFullNoteGroup() const
        {
            MX_ASSERT( myOutNote != nullptr );
            myOutNoteChoice = myOutNote->getNoteChoice();
            switch( myNoteData.noteType )
            {
                case api::NoteType::cue:
                {
                    myOutNoteChoice->setChoice( core::NoteChoice::Choice::cue );
                    auto choiceObj = myOutNoteChoice->getCueNoteGroup();
                    choiceObj->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>(myNoteData.durationData.durationTimeTicks) } );
                    myOutFullNoteGroup = choiceObj->getFullNoteGroup();
                    break;
                }
                case api::NoteType::grace:
                {
                    myOutNoteChoice->setChoice( core::NoteChoice::Choice::grace );
                    auto choiceObj = myOutNoteChoice->getGraceNoteGroup();
                    myOutFullNoteGroup = choiceObj->getFullNoteGroup();
                    if( myNoteData.isTied )
                    {
                        choiceObj->addTie( core::makeTie() );
                    }
                    break;
                }
                case api::NoteType::normal:
                {
                    myOutNoteChoice->setChoice( core::NoteChoice::Choice::normal );
                    auto choiceObj = myOutNoteChoice->getNormalNoteGroup();
                    myOutFullNoteGroup = choiceObj->getFullNoteGroup();
                    choiceObj->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>(myNoteData.durationData.durationTimeTicks) } );
                    if( myNoteData.isTied )
                    {
                        choiceObj->addTie( core::makeTie() );
                    }
                    break;
                }
                default:
                    break;
            }
            MX_ASSERT( myOutFullNoteGroup != nullptr );
        }
        
        
        void NoteWriter::setFullNoteTypeChoice() const
        {
            MX_ASSERT( myOutFullNoteGroup != nullptr );
            myOutFullNoteTypeChoice = myOutFullNoteGroup->getFullNoteTypeChoice();
            
            if( myNoteData.isRest )
            {
                myOutFullNoteTypeChoice->setChoice( core::FullNoteTypeChoice::Choice::rest );
                if( myNoteData.isDisplayStepOctaveSpecified )
                {
                    auto pitch = myOutFullNoteTypeChoice->getUnpitched()->getDisplayStepOctaveGroup();
                    pitch->getDisplayStep()->setValue( myConverter.convert( myNoteData.pitchData.step ) );
                    pitch->getDisplayOctave()->setValue( core::OctaveValue{ myNoteData.pitchData.octave } );
                }
            }
            else if( myNoteData.isUnpitched )
            {
                myOutFullNoteTypeChoice->setChoice( core::FullNoteTypeChoice::Choice::unpitched );
                if( myNoteData.isDisplayStepOctaveSpecified )
                {
                    auto pitch = myOutFullNoteTypeChoice->getUnpitched()->getDisplayStepOctaveGroup();
                    pitch->getDisplayStep()->setValue( myConverter.convert( myNoteData.pitchData.step ) );
                    pitch->getDisplayOctave()->setValue( core::OctaveValue{ myNoteData.pitchData.octave } );
                }
            }
            else
            {
                myOutFullNoteTypeChoice->setChoice( core::FullNoteTypeChoice::Choice::pitch );
                auto pitch = myOutFullNoteTypeChoice->getPitch();
                pitch->getStep()->setValue( myConverter.convert( myNoteData.pitchData.step ) );
                if( myNoteData.pitchData.alter != 0 )
                {
                    pitch->setHasAlter( true );
                    pitch->getAlter()->setValue( core::Semitones{ static_cast<core::DecimalType>( myNoteData.pitchData.alter ) } );
                }
                pitch->getOctave()->setValue( core::OctaveValue{ myNoteData.pitchData.octave } );
            }
        }
        
        
        void NoteWriter::setStaffAndVoice() const
        {
            if( myCursor.getNumStaves() > 1 && myCursor.staffIndex >= 0 )
            {
                myOutNote->setHasStaff( true );
                myOutNote->getStaff()->setValue( core::StaffNumber{ myCursor.staffIndex + 1 } );
            }
            
            if( myCursor.voiceIndex >= 0 )
            {
                myOutNote->getEditorialVoiceGroup()->setHasVoice( true );
                myOutNote->getEditorialVoiceGroup()->getVoice()->setValue( core::XsString{ std::to_string( myCursor.voiceIndex + 1 ) } );
            }
            // TODO - only show voice number if it is needed
            // i.e. only show if != 0 or voiceCount > 1
        }
        
        
        void NoteWriter::setDurationNameAndDots() const
        {
            myOutNote->setHasType( true );
            myOutNote->getType()->setValue( myConverter.convert( myNoteData.durationData.durationName ) );
            
            for( int d = 0; d < static_cast<int>( myNoteData.durationData.durationDots ); ++d )
            {
                myOutNote->addDot( core::makeDot() );
            }
        }
        
        
        void NoteWriter::setStemDirection() const
        {
            if( myNoteData.stem == api::Stem::unspecified )
            {
                return;
            }
            
            myOutNote->setHasStem( true );
            myOutNote->getStem()->setValue( myConverter.convert( myNoteData.stem ) );
        }
    }
}
