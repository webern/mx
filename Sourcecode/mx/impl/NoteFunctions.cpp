// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NoteFunctions.h"
#include "mx/impl/MxNoteReader.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/api/MarkData.h"
#include "mx/utility/Throw.h"


// remove?
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/elements/Arpeggiate.h"
#include "mx/core/elements/Articulations.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Glissando.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/NonArpeggiate.h"
#include "mx/core/elements/Ornaments.h"
#include "mx/core/elements/OtherNotation.h"
#include "mx/core/elements/Slide.h"
#include "mx/core/elements/Slur.h"
#include "mx/core/elements/Technical.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/Tuplet.h"
#include "mx/core/elements/ArticulationsChoice.h"
#include "mx/impl/PositionFunctions.h"

#include <algorithm>

namespace mx
{
    namespace impl
    {
        NoteFunctions::NoteFunctions() {}
        
        
        api::NoteData NoteFunctions::parseNote( const core::Note& inMxNote, const impl::Cursor& cursor ) const
        {
            impl::MxNoteReader reader{ inMxNote };
            api::NoteData outNoteData{};
            
            if( reader.getIsRest() )
            {
                outNoteData.isRest = true;
                outNoteData.isMeasureRest = reader.getIsMeasureRest();
            }
            
            outNoteData.isUnpitched = reader.getIsUnpitched();
            outNoteData.isDisplayStepOctaveSpecified = reader.getIsDisplayStepOctaveSpecified();
            outNoteData.isChord = reader.getIsChord();
            
            if( reader.getIsCue() )
            {
                outNoteData.noteType = api::NoteType::cue;
            }
            
            if( reader.getIsGrace() )
            {
                outNoteData.noteType = api::NoteType::grace;
            }
            
            if( reader.getIsNormal() )
            {
                outNoteData.noteType = api::NoteType::normal;
            }
            auto converter = Converter{};
            outNoteData.step = converter.convert( reader.getStep() );
            outNoteData.alter = reader.getAlter();
            
            outNoteData.accidental = api::Accidental::none;
            
            if( reader.getHasAccidental() )
            {
                outNoteData.accidental = converter.convert( reader.getAccidental() );
                outNoteData.isAccidentalParenthetical = reader.getIsAccidentalParenthetical();
                outNoteData.isAccidentalCautionary = reader.getIsAccidentalCautionary();
                outNoteData.isAccidentalEditorial = reader.getIsAccidentalEditorial();
                outNoteData.isAccidentalBracketed = reader.getIsAccidentalBracketed();
            }
            
            outNoteData.octave = reader.getOctave();
            outNoteData.staffIndex = reader.getStaffNumber() - 1;
            outNoteData.userRequestedVoiceNumber = reader.getVoiceNumber();
            
            if( reader.getIsDurationTypeSpecified() )
            {
                outNoteData.durationName = converter.convert( reader.getDurationType() );
            }
            else
            {
                outNoteData.durationName = api::DurationName::unspecified;
            }
            
            outNoteData.durationDots = reader.getNumDots();
            impl::TimeFunctions timeFunc;
            outNoteData.durationTimeTicks = timeFunc.convertDurationToGlobalTickScale( cursor, reader.getDurationValue() );
            outNoteData.startTimeTicks = cursor.position;
            
            for( const auto& coreBeamVal : reader.getBeams() )
            {
                outNoteData.beams.push_back( converter.convert( coreBeamVal ) );
            }
            
            outNoteData.timeModificationActualNotes = reader.getTimeModificationActualNotes();
            outNoteData.timeModificationNormalNotes = reader.getTimeModificationNormalNotes();
            
            const core::NoteTypeValue timeModType = reader.getTimeModificationNormalType();
            const int timeModTypeDots = reader.getTimeModificationNormalTypeDots();
            bool isTimeModTypeSpecified = ( timeModTypeDots > 0 ) && ( timeModType != reader.getDurationType() );
            
            if( isTimeModTypeSpecified )
            {
                outNoteData.timeModificationNormalType = converter.convert( timeModType );
                outNoteData.timeModificationNormalTypeDots = timeModTypeDots;
            }
            else
            {
                outNoteData.timeModificationNormalType = api::DurationName::unspecified;
                outNoteData.timeModificationNormalTypeDots = 0;
            }
            thing( inMxNote, cursor, outNoteData );
            outNoteData.positionData = impl::createPositionData( *inMxNote.getAttributes() );
            return outNoteData;
        }
        
        void NoteFunctions::thing( const core::Note& inMxNote, const impl::Cursor& cursor, api::NoteData& outNoteData ) const
        {
            MX_UNUSED(inMxNote);
            MX_UNUSED(cursor);
            for( const auto& notationsPtr : inMxNote.getNotationsSet() )
            {
                for( const auto& notationsChoicePtr : notationsPtr->getNotationsChoiceSet() )
                {
                    const auto& notationsChoice = *notationsChoicePtr;
                    const auto choice = notationsChoice.getChoice();
                    
                    switch (choice)
                    {
                        // these are spanners, handle elsewhere
                        case core::NotationsChoice::Choice::tied:
                        case core::NotationsChoice::Choice::slur:
                        case core::NotationsChoice::Choice::tuplet:
                        case core::NotationsChoice::Choice::glissando:
                        case core::NotationsChoice::Choice::slide:
                            break;
                        
                        // TODO - these
                        case core::NotationsChoice::Choice::ornaments:
                        case core::NotationsChoice::Choice::technical:
                            break;
                            
                        case core::NotationsChoice::Choice::articulations:
                        {
                            const auto& item = *notationsChoice.getArticulations();
                            for( const auto& articulationsChoicePtr : item.getArticulationsChoiceSet() )
                            {
                                const auto& articulationsChoice = *articulationsChoicePtr;
                                const auto articulationType = articulationsChoice.getChoice();
                                
                                switch ( articulationType )
                                {
                                    case core::ArticulationsChoice::Choice::accent:
                                    {
                                        api::MarkData mark;
                                        mark.absoluteMarkId = -1;
                                        mark.markType = api::MarkType::accent;
                                        mark.name = "accent";
                                        mark.smuflName = "articAccentAbove";
                                        mark.smuflCodepoint = api::Smufl::findCodepoint( mark.smuflName );
                                        mark.tickPosition = cursor.position;
                                        outNoteData.marks.emplace_back( std::move( mark ) );
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::strongAccent:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::staccato:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::tenuto:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::detachedLegato:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::staccatissimo:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::spiccato:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::scoop:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::plop:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::doit:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::falloff:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::breathMark:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::caesura:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::stress:
                                    {
                                        break;
                                    }
                                    
                                    case core::ArticulationsChoice::Choice::unstress:
                                    {
                                        break;
                                    }

                                    case core::ArticulationsChoice::Choice::otherArticulation:
                                    {
                                        break;
                                    }
                                    default:
                                        break;
                                }
                                
                            }

                        }
                            
                        // TODO - these
                        case core::NotationsChoice::Choice::dynamics:
                        case core::NotationsChoice::Choice::fermata:
                        case core::NotationsChoice::Choice::arpeggiate:
                        case core::NotationsChoice::Choice::nonArpeggiate:
                        case core::NotationsChoice::Choice::accidentalMark:
                        case core::NotationsChoice::Choice::otherNotation:
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }
    }
}
