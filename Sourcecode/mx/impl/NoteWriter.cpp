// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteWriter.h"
#include "mx/core/elements/Accidental.h"
#include "mx/core/elements/ActualNotes.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/Beam.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/DisplayStep.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/elements/Dot.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/NormalType.h"
#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Pitch.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Stem.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/Strings.h"
#include "mx/impl/NotationsWriter.h"
#include "mx/impl/ScoreWriter.h"

namespace mx
{
    namespace impl
    {
        NoteWriter::NoteWriter(
            const api::NoteData& inNoteData,
            const MeasureCursor& inCursor,
            const ScoreWriter& inScoreWriter,
            bool isPreviousNoteAChordMember )
        : myNoteData{ inNoteData }
        , myCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myConverter{}
        , myIsPreviousNoteAChordMember{ isPreviousNoteAChordMember }
        , myOutNote{ nullptr }
        , myOutNoteChoice( nullptr )
        , myOutFullNoteGroup( nullptr )
        , myOutFullNoteTypeChoice{ nullptr }
        {
            
        }

        
        core::NotePtr NoteWriter::getNote( bool isStartOfChord ) const
        {
            myOutNote = core::makeNote();
            setNoteChoiceAndFullNoteGroup( isStartOfChord );
            setFullNoteTypeChoice();
            setStaffAndVoice();
            setDurationNameAndDots();
            setNotehead();
            setStemDirection();
            setMiscData();
            NotationsWriter notationsWriter{ myNoteData, myCursor, myScoreWriter };
            auto& noteAttr = *myOutNote->getAttributes();
            impl::setAttributesFromPositionData(myNoteData.positionData, noteAttr);

            auto notations = notationsWriter.getNotations();
            if( notations->getNotationsChoiceSet().size() > 0 )
            {
                myOutNote->addNotations( notations );
            }
            
            if( myNoteData.pitchData.accidental != api::Accidental::none )
            {
                myOutNote->setHasAccidental( true );
                myOutNote->getAccidental()->setValue( myConverter.convert( myNoteData.pitchData.accidental ) );
            }
            
            auto beamIndex = 0;
            for( const auto& beam : myNoteData.beams )
            {
                auto mxBeam = core::makeBeam();
                auto attr = mxBeam->getAttributes();
                attr->hasNumber = true;
                attr->number = core::BeamLevel{ beamIndex + 1 };
                mxBeam->setValue( myConverter.convert( beam ) );
                myOutNote->addBeam( mxBeam );
                ++beamIndex;
            }
            
            if(    myNoteData.durationData.timeModificationNormalNotes > 0
                && myNoteData.durationData.timeModificationActualNotes > 0
                && (    myNoteData.durationData.timeModificationNormalNotes > 1
                     || myNoteData.durationData.timeModificationActualNotes > 1 ) )
            {
                myOutNote->setHasTimeModification( true );
                auto timeMod = myOutNote->getTimeModification();
                timeMod->getActualNotes()->setValue( core::NonNegativeInteger{ myNoteData.durationData.timeModificationActualNotes } );
                timeMod->getNormalNotes()->setValue( core::NonNegativeInteger{ myNoteData.durationData.timeModificationNormalNotes } );

                if (myNoteData.durationData.timeModificationNormalType != api::DurationName::unspecified)
                {
                    timeMod->setHasNormalTypeNormalDotGroup(true);
                    timeMod->getNormalTypeNormalDotGroup()->getNormalType()->setValue( myConverter.convert( myNoteData.durationData.timeModificationNormalType ) );

                    for( int i = 0; i < myNoteData.durationData.timeModificationNormalTypeDots; ++i )
                    {
                        timeMod->getNormalTypeNormalDotGroup()->addNormalDot( core::makeNormalDot() );
                    }
                }
            }

            return myOutNote;
        }
        
        
        void NoteWriter::setNoteChoiceAndFullNoteGroup( bool isStartOfChord ) const
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
                    myOutFullNoteGroup->setHasChord( myCursor.isChordActive && myIsPreviousNoteAChordMember && !isStartOfChord );
                    break;
                }
                case api::NoteType::grace:
                {
                    myOutNoteChoice->setChoice( core::NoteChoice::Choice::grace );
                    auto choiceObj = myOutNoteChoice->getGraceNoteGroup();
                    myOutFullNoteGroup = choiceObj->getFullNoteGroup();
                    myOutFullNoteGroup->setHasChord( myCursor.isChordActive && myIsPreviousNoteAChordMember && !isStartOfChord );
                    if( myNoteData.isTieStart )
                    {
                        auto tie = core::makeTie();
                        tie->getAttributes()->type = core::StartStop::start;
                        choiceObj->addTie( tie );
                    }
                    if( myNoteData.isTieStop )
                    {
                        auto tie = core::makeTie();
                        tie->getAttributes()->type = core::StartStop::stop;
                        choiceObj->addTie( tie );
                    }
                    break;
                }
                case api::NoteType::normal:
                {
                    myOutNoteChoice->setChoice( core::NoteChoice::Choice::normal );
                    auto choiceObj = myOutNoteChoice->getNormalNoteGroup();
                    myOutFullNoteGroup = choiceObj->getFullNoteGroup();
                    myOutFullNoteGroup->setHasChord( myCursor.isChordActive && myIsPreviousNoteAChordMember && !isStartOfChord );
                    choiceObj->getDuration()->setValue( core::PositiveDivisionsValue{ static_cast<core::DecimalType>(myNoteData.durationData.durationTimeTicks) } );
                    if( myNoteData.isTieStart )
                    {
                        auto tie = core::makeTie();
                        tie->getAttributes()->type = core::StartStop::start;
                        choiceObj->addTie( tie );
                    }
                    if( myNoteData.isTieStop )
                    {
                        auto tie = core::makeTie();
                        tie->getAttributes()->type = core::StartStop::stop;
                        choiceObj->addTie( tie );
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

                if( myNoteData.isMeasureRest )
                {
                    myOutFullNoteTypeChoice->getRest()->getAttributes()->hasMeasure = true;
                    myOutFullNoteTypeChoice->getRest()->getAttributes()->measure = core::YesNo::yes;
                    myOutNote->setHasType( false );
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
            if( !myNoteData.isRest || !myNoteData.isMeasureRest )
            {
                myOutNote->setHasType( true );
                myOutNote->getType()->setValue( myConverter.convert( myNoteData.durationData.durationName ) );
            }

            for( int d = 0; d < static_cast<int>( myNoteData.durationData.durationDots ); ++d )
            {
                myOutNote->addDot( core::makeDot() );
            }
        }
        
        void NoteWriter::setNotehead() const
        {
            myOutNote->setHasNotehead( true );
            myOutNote->getNotehead()->setValue( myConverter.convert( myNoteData.notehead ) );
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


        void NoteWriter::setMiscData() const
        {
            if( myNoteData.miscData.size() == 0 )
            {
                return;
            }

            const std::string comma = ",";
            const std::string underscore = "_";

            bool isFirst = true;
            for( auto s : myNoteData.miscData )
            {

                myOutNote->getEditorialVoiceGroup()->setHasFootnote( true );
                auto footnote = myOutNote->getEditorialVoiceGroup()->getFootnote();
                footnote->getAttributes()->hasFontFamily = true;
                auto& miscField = footnote->getAttributes()->fontFamily;

                std::string::size_type position = 0;
                while ( ( position = s.find( comma, position ) ) != std::string::npos )
                {
                    s.replace( position, comma.size(), underscore );
                    position++;
                }

                if (isFirst)
                {
                    isFirst = false;
                    miscField.addValue( core::XsToken{ std::string{"##misc-data##"} + s } );
                }
                else
                {
                    miscField.addValue( core::XsToken{ s } );
                }
            }
        }
    }
}
