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
#include "mx/core/elements/NotationsChoice.h"
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
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/XsString.h"
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
            bool isPreviousNoteAChordMember,
            const std::vector<mx::api::NoteData>& inSiblingNotes,
            int inNoteIndex )
        : myNoteData{ inNoteData }
        , myCursor{ inCursor }
        , myScoreWriter{ inScoreWriter }
        , myConverter{}
        , myIsPreviousNoteAChordMember{ isPreviousNoteAChordMember }
        , mySiblingNotes{ inSiblingNotes }
        , myNoteIndex{ inNoteIndex }
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

                // find the tuplet start note and TupletStart object
                bool isTupletStartFound = false;
                int tupletStartIndex = myNoteIndex;
                mx::api::NoteData tupletStartNote{};
                mx::api::TupletStart tupletStart{};

                for( ; tupletStartIndex >= 0 && mySiblingNotes.size() > 0; --tupletStartIndex )
                {
                    const auto& siblingNote = mySiblingNotes.at( static_cast<size_t>( tupletStartIndex ) );
                    if( siblingNote.noteAttachmentData.tupletStarts.size() == 1 )
                    {
                        isTupletStartFound = true;
                        tupletStartNote = siblingNote;
                        tupletStart = siblingNote.noteAttachmentData.tupletStarts.at( 0 );
                        break;
                    }
                }

                if( !isTupletStartFound )
                {
                    MX_DEBUG_THROW( "tupletStart was not found" );
                }

                // find the tuplet stop note and TupletStop object
                bool isTupletStopFound = false;
                int tupletStopIndex = tupletStartIndex;
                mx::api::NoteData tupletStopNote{};
                mx::api::TupletStop tupletStop{};

                if( isTupletStartFound )
                {
                    for( ; tupletStopIndex < static_cast<int>( mySiblingNotes.size() ); ++tupletStopIndex )
                    {
                        const auto& siblingNote = mySiblingNotes.at( static_cast<size_t>( tupletStopIndex ) );
                        if( siblingNote.noteAttachmentData.tupletStops.size() == 1 )
                        {
                            isTupletStopFound = true;
                            tupletStopNote = siblingNote;
                            tupletStop = siblingNote.noteAttachmentData.tupletStops.at( 0 );
                            break;
                        }
                    }
                }

                if( !isTupletStopFound )
                {
                    MX_DEBUG_THROW( "tupletStop was not found" );
                }

                // calculate the distance between the two
                if( isTupletStartFound && isTupletStopFound )
                {
                    const auto tickTimeDistance = ( tupletStopNote.tickTimePosition + tupletStopNote.durationData.durationTimeTicks ) - tupletStartNote.tickTimePosition;

                    if( tickTimeDistance > 0 && tupletStart.normalNumber != 0 && tupletStart.actualNumber != 0 )
                    {
                        // calculate the tuplet normal type and dots based on the distance between start and stop and the ratio
                        const long double normalLength = static_cast<long double>( tickTimeDistance ) / static_cast<long double>( tupletStart.normalNumber );

                        mx::api::DurationName normalName = mx::api::DurationName::unspecified;
                        int normalDots = 0;
                        const bool isNormalNameAndDotsFound = this->findNormalNameAndDots( normalName, normalDots, normalLength );
                        if( isNormalNameAndDotsFound )
                        {
                            timeMod->setHasNormalTypeNormalDotGroup(true);
                            timeMod->getNormalTypeNormalDotGroup()->getNormalType()->setValue( myConverter.convert( normalName ) );

                            for( int i = 0; i < normalDots; ++i )
                            {
                                timeMod->getNormalTypeNormalDotGroup()->addNormalDot( mx::core::makeNormalDot() );
                            }
                        }
                        else
                        {
                           MX_DEBUG_THROW( "this->findNormalNameAndDots could not find what it was looking for. This probably means that the file has a badly specified tuplet." );
                        }
                    }
                    else
                    {
                        MX_DEBUG_THROW( "one of these things was not true ( tickTimeDistance > 0 && tupletStart.normalNumber != 0 && tupletStart.actualNumber != 0 )" );
                    }
                }
                else
                {
                    MX_DEBUG_THROW( "one of these things was not true ( isTupletStartFound && isTupletStopFound )" );
                }

                // TODO - decide what happens if the user entered specific tuplet type in the
                // duration data, possibly remove those fields from duration data.
            }

            return myOutNote;
        }
        
        template<typename CHOICE_OBJ_TYPE>
        static inline void addTie( bool isStart, CHOICE_OBJ_TYPE choiceObj, core::NotePtr outNote )
        {
            auto tie = core::makeTie();
            
            if( isStart )
            {
                tie->getAttributes()->type = core::StartStop::start;
            }
            else
            {
                tie->getAttributes()->type = core::StartStop::stop;
            }
            
            choiceObj->addTie( tie );
            
            core::NotationsPtr notations = nullptr;
            core::NotationsChoicePtr notationsChoice = nullptr;
            
            if( !outNote->getNotationsSet().empty() )
            {
                notations = outNote->getNotationsSet().front();
                notationsChoice = core::makeNotationsChoice();
                notations->addNotationsChoice( notationsChoice );
                outNote->addNotations( notations );
            }
            else
            {
                notations = core::makeNotations();
                
                if( notations->getNotationsChoiceSet().empty() )
                {
                    notationsChoice = core::makeNotationsChoice();
                    notations->addNotationsChoice( notationsChoice );
                }
                else
                {
                    notationsChoice = notations->getNotationsChoiceSet().front();
                }
                
                outNote->addNotations( notations );
            }
            
            notationsChoice->setChoice( core::NotationsChoice::Choice::tied );
            
            if( isStart )
            {
                notationsChoice->getTied()->getAttributes()->type = core::StartStopContinue::start;
            }
            else
            {
                notationsChoice->getTied()->getAttributes()->type = core::StartStopContinue::stop;
            }
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

                    if( myNoteData.isTieStop )
                    {
                        addTie( false, choiceObj, myOutNote );
                    }
                    
                    if( myNoteData.isTieStart )
                    {
                        addTie( true, choiceObj, myOutNote );
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
                    
                    if( myNoteData.isTieStop )
                    {
                        addTie( false, choiceObj, myOutNote );
                    }
                    
                    if( myNoteData.isTieStart )
                    {
                        addTie( true, choiceObj, myOutNote );
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
                if( myNoteData.pitchData.alter != 0 || myNoteData.pitchData.cents != 0.0 )
                {
                    const auto alter = Converter::convertToAlter( myNoteData.pitchData.alter, myNoteData.pitchData.cents );
                    pitch->setHasAlter( true );
                    pitch->getAlter()->setValue( core::Semitones{ alter } );
                }
                pitch->getOctave()->setValue( core::OctaveValue{ myNoteData.pitchData.octave } );
            }
        }
        
        
        void NoteWriter::setStaffAndVoice() const
        {
            if( myCursor.getNumStaves() > 1 && myCursor.staffIndex >= 0 )
            {
                myOutNote->setHasStaff( true );
                myOutNote->getStaff()->setValue( core::PositiveInteger{ myCursor.staffIndex + 1 } );
            }
            
            if( myCursor.voiceIndex >= 0 )
            {
                myOutNote->getEditorialVoiceGroup()->setHasVoice( true );
                myOutNote->getEditorialVoiceGroup()->getVoice()->setValue( core::XsString{ std::to_string( myCursor.voiceIndex + 1 ) } );
            }
            // TODO - only show voice number if it is needed i.e. only show if != 0 or voiceCount > 1
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
            if( myNoteData.notehead != mx::api::Notehead::normal )
            {
                myOutNote->setHasNotehead( true );
                myOutNote->getNotehead()->setValue( myConverter.convert( myNoteData.notehead ) );
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

        bool
        NoteWriter::findNormalNameAndDots( mx::api::DurationName& ioName, int& ioDots, long double inTickLength ) const
        {
            const auto equals = [&]( long double a, long double b )
            {
                return std::abs( a - b ) < 0.0001;
            };

            const auto isMatch = [&]( long double durQuarters, int numDots, mx::api::DurationName name )
            {
                if( equals( mx::api::applyDots( durQuarters * static_cast<long double>( myCursor.ticksPerQuarter ), numDots ), inTickLength ) )
                {
                    ioName = name;
                    ioDots = numDots;
                    return true;
                }

                return false;
            };

            for( int dots = 0; dots < 4; ++dots )
            {

                if( isMatch( mx::api::DUR_QUARTERS_VALUE_QUARTER, dots, mx::api::DurationName::quarter ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_EIGHTH, dots, mx::api::DurationName::eighth ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_16TH, dots, mx::api::DurationName::dur16th ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_MAXIMA, dots, mx::api::DurationName::maxima ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_LONGA, dots, mx::api::DurationName::longa ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_BREVE, dots, mx::api::DurationName::breve ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_WHOLE, dots, mx::api::DurationName::whole ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_HALF, dots, mx::api::DurationName::half ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_32ND, dots, mx::api::DurationName::dur32nd ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_64TH, dots, mx::api::DurationName::dur64th ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_128TH, dots, mx::api::DurationName::dur128th ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_256TH, dots, mx::api::DurationName::dur256th ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_512TH, dots, mx::api::DurationName::dur512th ) )
                {
                    return true;
                }
                else if( isMatch( mx::api::DUR_QUARTERS_VALUE_1024TH, dots, mx::api::DurationName::dur1024th ) )
                {
                    return true;
                }
            }

            return false;
        }
    }
}
