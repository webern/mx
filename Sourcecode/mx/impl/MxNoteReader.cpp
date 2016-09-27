// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/MxNoteReader.h"
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
#include "mx/utility/StringToInt.h"

#include <map>

namespace mx
{
    namespace impl
    {
        MxNoteReader::MxNoteReader( const core::Note& mxNote )
        : myNote( mxNote )
        , myNoteChoice( *myNote.getNoteChoice() )
        , myFullNoteGroup( findFullNoteGroup( myNoteChoice ) )
        , myIsNormal( false )
        , myIsGrace( false )
        , myIsCue( false )
        , myIsRest( false )
        , myIsChord( false )
        , myIsMeasureRest( false )
        , myIsUnpitched( false )
        , myIsPitch( false )
        , myIsDisplayStepOctaveSpecified( false )
        , myDurationValue( 0.0L )
        , myTieCount( 0 )
        , myStep( core::StepEnum::c )
        , myAlter( 0 )
        , myOctave( 4 )
        , myStaffNumber( 0 )
        , myVoiceNumber( 0 )
        , myDurationType( core::NoteTypeValue::maxima )
        , myIsDurationTypeSpecified( false )
        , myNumDots( 0 )
        , myBeams()
        , myTimeModificationActualNotes( -1 )
        , myTimeModificationNormalNotes( -1 )
        , myTimeModificationNormalType( core::NoteTypeValue::maxima )
        , myTimeModificationNormalTypeDots( 0 )
        , myHasAccidental( false )
        , myAccidental( core::AccidentalValue::natural )
        , myIsAccidentalParenthetical( false )
        , myIsAccidentalCautionary{ false }
        , myIsAccidentalEditorial{ false }
        , myIsAccidentalBracketed{ false }
        {
            setNormalGraceCueItems();
            setRestPitchUnpitchedItems();
            setChord();
            setStaffNumber();
            setVoiceNumber();
            setDurationType();
            setNumDots();
            setBeams();
            setTimeModification();
            setAccidental();
        }

        const core::FullNoteGroup& MxNoteReader::findFullNoteGroup( const core::NoteChoice& noteChoice ) const
        {
            switch ( noteChoice.getChoice() )
            {
                case core::NoteChoice::Choice::normal:
                {
                    const auto& noteGuts = *noteChoice.getNormalNoteGroup();
                    return *noteGuts.getFullNoteGroup();
                }
                case core::NoteChoice::Choice::grace:
                {
                    const auto& noteGuts = *noteChoice.getGraceNoteGroup();
                    return *noteGuts.getFullNoteGroup();
                }
                case core::NoteChoice::Choice::cue:
                {
                    const auto& noteGuts = *noteChoice.getCueNoteGroup();
                    return *noteGuts.getFullNoteGroup();
                }
                default:
                    break;
            }

            MX_BUG;
        }


        void MxNoteReader::setNormalGraceCueItems()
        {
            switch ( myNoteChoice.getChoice() )
            {
                case core::NoteChoice::Choice::normal:
                {
                    myIsNormal = true;
                    const auto& noteGuts = *myNoteChoice.getNormalNoteGroup();
                    myTieCount = static_cast<int>( noteGuts.getTieSet().size() );
                    myDurationValue = noteGuts.getDuration()->getValue().getValue();
                    break;
                }
                case core::NoteChoice::Choice::grace:
                {
                    myIsGrace = true;
                    const auto& noteGuts = *myNoteChoice.getGraceNoteGroup();
                    myTieCount = static_cast<int>( noteGuts.getTieSet().size() );
                    myDurationValue = 0;
                    break;
                }
                case core::NoteChoice::Choice::cue:
                {
                    myIsCue = true;
                    const auto& noteGuts = *myNoteChoice.getCueNoteGroup();
                    myDurationValue = noteGuts.getDuration()->getValue().getValue();
                    myTieCount = 0;
                    break;
                }
                default:
                    MX_THROW( "invalid NoteChoice::Choice value" );
            }
        }


        void MxNoteReader::setRestPitchUnpitchedItems()
        {
            const auto& fullNoteTypeChoice = *myFullNoteGroup.getFullNoteTypeChoice();
            
            switch ( fullNoteTypeChoice.getChoice() )
            {
                case core::FullNoteTypeChoice::Choice::rest:
                {
                    myIsRest = true;
                    const auto& rest = *fullNoteTypeChoice.getRest();
                    
                    if( rest.getAttributes()->hasMeasure && ( rest.getAttributes()->measure == core::YesNo::yes ) )
                    {
                        myIsMeasureRest = true;
                        
                    }
                    
                    const auto& stepOctave = *rest.getDisplayStepOctaveGroup();
                    
                    if( rest.getHasDisplayStepOctaveGroup() )
                    {
                        myIsDisplayStepOctaveSpecified = true;
                        myStep = stepOctave.getDisplayStep()->getValue();
                        myOctave = stepOctave.getDisplayOctave()->getValue().getValue();
                    }

                    myAlter = 0;
                    break;
                }
                case core::FullNoteTypeChoice::Choice::unpitched:
                {
                    myIsUnpitched = true;
                    const auto& unpitched = *fullNoteTypeChoice.getUnpitched();
                    const auto& stepOctave = *unpitched.getDisplayStepOctaveGroup();
                    
                    if( unpitched.getHasDisplayStepOctaveGroup() )
                    {
                        myIsDisplayStepOctaveSpecified = true;
                        myStep = stepOctave.getDisplayStep()->getValue();
                        myOctave = stepOctave.getDisplayOctave()->getValue().getValue();
                    }
                    
                    myAlter = 0;
                    break;
                }
                case core::FullNoteTypeChoice::Choice::pitch:
                {
                    myIsPitch = true;
                    const auto& pitch = *fullNoteTypeChoice.getPitch();
                    myStep = pitch.getStep()->getValue();
                    myOctave = pitch.getOctave()->getValue().getValue();
                    myAlter = static_cast<int>( std::ceil( pitch.getAlter()->getValue().getValue() - 0.5 ) );
                    break;
                }
                    
                default:
                    MX_THROW( "invalid FullNoteTypeChoice::Choice value" );
            }
        }


        void MxNoteReader::setChord()
        {
            myIsChord = myFullNoteGroup.getHasChord();
        }
        
        
        void MxNoteReader::setStaffNumber()
        {
            if( myNote.getHasStaff() )
            {
                const auto& staffObject = *myNote.getStaff();
                myStaffNumber = static_cast<int>( staffObject.getValue().getValue() );
            }
        }


        void MxNoteReader::setVoiceNumber()
        {
            myVoiceNumber = -1;
            
            if( !myNote.getEditorialVoiceGroup()->getHasVoice() )
            {
                return;
            }
            
            utility::stringToInt( myNote.getEditorialVoiceGroup()->getVoice()->getValue().getValue().c_str(), myVoiceNumber );
        }


        void MxNoteReader::setDurationType()
        {
            if( myNote.getHasType() )
            {
                myIsDurationTypeSpecified = true;
                myDurationType = myNote.getType()->getValue();
            }
        }


        void MxNoteReader::setNumDots()
        {
            myNumDots = static_cast<int>( myNote.getDotSet().size() );
        }
        
        
        void MxNoteReader::setBeams()
        {
            const auto& mxBeamSet = myNote.getBeamSet();
            std::map<int,core::BeamValue> mapOfBeams;
            int calculatedBeamIndex = 0;
            
            for( const auto& mxBeam : mxBeamSet )
            {
                const int userBeamIndex = ( mxBeam->getAttributes()->number.getValue() );
                int useThisBeamIndex = calculatedBeamIndex;
                
                // if the userBeamIndex seems valid we will use it
                // by doing this we support the possibility that
                // the musicxml file could have beams listed in the
                // 'wrong' order, i.e. 3,4,1,2
                if( mxBeam->getAttributes()->hasNumber )
                {
                    if( mapOfBeams.find( userBeamIndex ) == mapOfBeams.cend() )
                    {
                        useThisBeamIndex = userBeamIndex;
                    }
                }
                
                mapOfBeams[useThisBeamIndex] = mxBeam->getValue();
                ++calculatedBeamIndex;
            }
            
            // if the musicxml document has not provided all of the needed
            // beams then this will not be using the docs illogical numbers
            for( const auto& beamVal : mapOfBeams )
            {
                myBeams.push_back( beamVal.second );
            }
        }
        
        
        void MxNoteReader::setTimeModification()
        {
            if( ! myNote.getHasTimeModification() )
            {
                myTimeModificationActualNotes = 1;
                myTimeModificationNormalNotes = 1;
            }
            
            const auto& mxTimeMod = *myNote.getTimeModification();
            myTimeModificationActualNotes = mxTimeMod.getActualNotes()->getValue().getValue();
            myTimeModificationNormalNotes = mxTimeMod.getNormalNotes()->getValue().getValue();
            
            if( mxTimeMod.getHasNormalTypeNormalDotGroup() )
            {
                const auto& grp = *mxTimeMod.getNormalTypeNormalDotGroup();
                myTimeModificationNormalType = grp.getNormalType()->getValue();
                myTimeModificationNormalTypeDots = static_cast<int>( grp.getNormalDotSet().size() );
            }
            else
            {
                myTimeModificationNormalType = myDurationType;
                myTimeModificationNormalTypeDots = 0;
            }
        }
        
        
        void MxNoteReader::setAccidental()
        {
            myIsAccidentalParenthetical = false;
            myHasAccidental = myNote.getHasAccidental();

            if( myHasAccidental )
            {
                myAccidental = myNote.getAccidental()->getValue();
                
                if(    myNote.getAccidental()->getAttributes()->hasParentheses
                    && myNote.getAccidental()->getAttributes()->parentheses == core::YesNo::yes )
                {
                    myIsAccidentalParenthetical = true;
                }
                
                if(    myNote.getAccidental()->getAttributes()->hasCautionary
                    && myNote.getAccidental()->getAttributes()->cautionary == core::YesNo::yes )
                {
                    myIsAccidentalCautionary = true;
                }
                
                if(    myNote.getAccidental()->getAttributes()->hasEditorial
                    && myNote.getAccidental()->getAttributes()->editorial == core::YesNo::yes )
                {
                    myIsAccidentalEditorial = true;
                }
                
                if(    myNote.getAccidental()->getAttributes()->hasBracket
                    && myNote.getAccidental()->getAttributes()->bracket == core::YesNo::yes )
                {
                    myIsAccidentalBracketed = true;
                }
            }
        }
    }
}
