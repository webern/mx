// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NoteReader.h"
#include "mx/core/elements/Stem.h"
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
#include "mx/core/elements/Lyric.h"
#include "mx/core/elements/LyricTextChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/NormalType.h"
#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/Notehead.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/Syllabic.h"
#include "mx/core/elements/SyllabicTextGroup.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Type.h"
#include "mx/core/elements/Text.h"
#include "mx/core/elements/Tie.h"
#include "mx/core/elements/TimeModification.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/Voice.h"
#include "mx/utility/StringToInt.h"

#include <map>
#include "mx/api/PitchData.h"
#include "Converter.h"

namespace mx
{
    namespace impl
    {
        NoteReader::NoteReader( const core::Note& mxNote )
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
        , myStep( core::StepEnum::c )
        , myAlter( 0 )
        , myCents( 0.0 )
        , myOctave( 4 )
        , myStaffNumber( 0 )
        , myVoiceNumber( 0 )
        , myNoteheadValue( core::NoteheadValue::normal)
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
        , myIsTieStart{ false }
        , myIsTieStop{ false }
        , myHasLyric{ false }
        {
            setNormalGraceCueItems();
            setRestPitchUnpitchedItems();
            setChord();
            setStaffNumber();
            setVoiceNumber();
            setNoteheadValue();
            setDurationType();
            setNumDots();
            setBeams();
            setTimeModification();
            setAccidental();
            setStem();
            setLyric();
        }

        const core::FullNoteGroup& NoteReader::findFullNoteGroup( const core::NoteChoice& noteChoice ) const
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


        void NoteReader::setNormalGraceCueItems()
        {
            switch ( myNoteChoice.getChoice() )
            {
                case core::NoteChoice::Choice::normal:
                {
                    myIsNormal = true;
                    const auto& noteGuts = *myNoteChoice.getNormalNoteGroup();
                    myDurationValue = noteGuts.getDuration()->getValue().getValue();
                    setTie( noteGuts.getTieSet() );
                    break;
                }
                case core::NoteChoice::Choice::grace:
                {
                    myIsGrace = true;
                    const auto& noteGuts = *myNoteChoice.getGraceNoteGroup();
                    myDurationValue = 0;
                    setTie( noteGuts.getTieSet() );
                    break;
                }
                case core::NoteChoice::Choice::cue:
                {
                    myIsCue = true;
                    const auto& noteGuts = *myNoteChoice.getCueNoteGroup();
                    myDurationValue = noteGuts.getDuration()->getValue().getValue();
                    break;
                }
                default:
                    MX_THROW( "invalid NoteChoice::Choice value" );
            }
        }


        void NoteReader::setRestPitchUnpitchedItems()
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
                    if( pitch.getHasAlter() )
                    {
                        const auto semitonesAndCents = mx::impl::Converter::convertToSemitonesAndCents( pitch.getAlter()->getValue().getValue() );
                        myAlter = semitonesAndCents.first;
                        myCents = semitonesAndCents.second;
                    }
                    break;
                }
                    
                default:
                    MX_THROW( "invalid FullNoteTypeChoice::Choice value" );
            }
        }


        void NoteReader::setChord()
        {
            myIsChord = myFullNoteGroup.getHasChord();
        }
        
        
        void NoteReader::setStaffNumber()
        {
            if( myNote.getHasStaff() )
            {
                const auto& staffObject = *myNote.getStaff();
                myStaffNumber = static_cast<int>( staffObject.getValue().getValue() );
            }
        }


        void NoteReader::setVoiceNumber()
        {
            myVoiceNumber = -1;
            
            if( !myNote.getEditorialVoiceGroup()->getHasVoice() )
            {
                return;
            }
            
            utility::stringToInt( myNote.getEditorialVoiceGroup()->getVoice()->getValue().getValue().c_str(), myVoiceNumber );
        }

        
        void NoteReader::setNoteheadValue()
        {
            myNoteheadValue = myNote.getNotehead()->getValue();
        }

        
        void NoteReader::setDurationType()
        {
            if( myNote.getHasType() )
            {
                myIsDurationTypeSpecified = true;
                myDurationType = myNote.getType()->getValue();
            }
        }


        void NoteReader::setNumDots()
        {
            myNumDots = static_cast<int>( myNote.getDotSet().size() );
        }
        
        
        void NoteReader::setBeams()
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
        
        
        void NoteReader::setTimeModification()
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
        
        
        void NoteReader::setAccidental()
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
        
        
        void NoteReader::setStem()
        {
            if( myNote.getHasStem() )
            {
                myIsStemSpecified = true;
                myStem = myNote.getStem()->getValue();
            }
            else
            {
                myIsStemSpecified = false;
            }
        }
        
        
        void NoteReader::setTie( const core::TieSet& tieSet )
        {
            for( const auto& tie : tieSet )
            {
                if( tie->getAttributes()->type == core::StartStop::start )
                {
                    myIsTieStart = true;
                }
                else if( tie->getAttributes()->type == core::StartStop::stop )
                {
                    myIsTieStop = true;
                }
            }
        }
        
        void NoteReader::setLyric()
        {
            const auto& lyricSet = myNote.getLyricSet();
            auto iter = lyricSet.begin();
            const auto iterEnd = lyricSet.end();
            for (; iter != iterEnd; ++iter) {
                const auto& lyric = *iter;
                
                const auto& textChoice = lyric->getLyricTextChoice();
                if( textChoice )
                {
                    const auto choice = textChoice->getChoice();
                    switch ( choice )
                    {
                        case core::LyricTextChoice::Choice::syllabicTextGroup:
                        {
                            const auto textGroup = textChoice->getSyllabicTextGroup();
                            if ( textGroup )
                            {
                                core::SyllabicEnum syllabic = core::SyllabicEnum::single;
                                if( textGroup->getHasSyllabic() )
                                {
                                    syllabic = textGroup->getSyllabic()->getValue();
                                }

                                const auto& textPtr = textGroup->getText();
                                if ( textPtr )
                                {
                                    const auto text = textPtr->getValue();
                                    const LyricType lyricType( text.getValue(), syllabic );
                                    myLyrics.emplace_back( lyricType );
                                    myHasLyric = true;
                                }
                            }
                            break;
                        }

                        case core::LyricTextChoice::Choice::extend:
                        case core::LyricTextChoice::Choice::laughing:
                        case core::LyricTextChoice::Choice::humming:
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}
