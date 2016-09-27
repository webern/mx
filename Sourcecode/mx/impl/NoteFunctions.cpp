// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/NoteFunctions.h"
#include "mx/impl/MxNoteReader.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"

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
            outNoteData.userRequestedVoiceNumber = reader.getVoiceNumber(); // TODO - make sure this is -1 when not found in the musicxml
            
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
            
            return outNoteData;
        }
    }
}
