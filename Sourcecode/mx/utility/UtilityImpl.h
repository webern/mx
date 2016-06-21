// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mx/core/DocumentPartwise.h"
#include "mx/utility/Utility.h"

namespace mx
{
	namespace utility
	{
        core::PartGroupPtr makePartGroupStartTag( const PartGroupParams& params );
        
        bool isPartListInitialized( const MxDoc& doc );
        
        core::ScorePartPtr makePartDeclaration( const PartParams& params );

        void addScoreInstrumentToPart(
            core::ScorePartPtr& part,
            const PartParams& params );
        
        void addVirtualInstrumentToScoreInstrument(
            core::ScorePartPtr& part,
            const PartParams& params );
        
        void addMidiInstrumentToPart(
            core::ScorePartPtr& part,
            const PartParams& params );

        core::PartwisePartPtr makePartForMusicData( const PartParams& params );
        
        core::PartwisePartPtr addInitialPartToDocument( const MxDoc& doc, const PartParams& params );
        core::PartwisePartPtr addSubsequentPartToDocument( const MxDoc& doc, const PartParams& params );
        core::MusicDataChoicePtr createOrRetrieveMeasureProperties( const core::PartwiseMeasurePtr& measure );
        
        core::MusicDataChoicePtr createEmptyNote( core::NoteChoice::Choice choice );
        core::FullNoteGroupPtr findFullNoteGroup( const core::NotePtr& noteElement );
        void setNoteStep( const core::MusicDataChoicePtr& mdc, int step ); // c = 0, d = 1, etc.
        void setNoteAlter( const core::MusicDataChoicePtr& mdc, int alter );
        void setNoteOctave( const core::MusicDataChoicePtr& mdc, int octave );
        void setNoteStaffNumber( const core::MusicDataChoicePtr& mdc, int staffNumber );
        void setNoteVoiceNumber( const core::MusicDataChoicePtr& mdc, int voiceNumber );
        void setStemDirection( const core::MusicDataChoicePtr& mdc, mx::utility::UpDown stemDirection );
        void setNoteDurationType( const core::MusicDataChoicePtr& mdc, core::NoteTypeValue durationType );
        void setNoteDurationInt( const core::MusicDataChoicePtr& mdc, int noteDuration );
        void setNoteDurationDots( const core::MusicDataChoicePtr& mdc, int dots );
        void setNoteBeams( const core::MusicDataChoicePtr& mdc, const std::vector<core::BeamValue>& beams );
        void setNoteAccidental( const core::MusicDataChoicePtr& mdc, int alter );
        void setNoteIsRest( const core::MusicDataChoicePtr& mdc );
        void setNoteIsChord( const core::MusicDataChoicePtr& mdc );
        void setNoteIsGrace( const core::MusicDataChoicePtr& mdc );
        void setNoteIsCue( const core::MusicDataChoicePtr& mdc );
        

        /// if the Measure all ready has an <attributes> (aka properties) element,
        /// returns a pointer to that element.  Otherwise adds an <attributes>
        /// element to the measure and returns a pointer to it.
        core::MusicDataChoicePtr getOrAddMeasureProperties( const core::PartwiseMeasurePtr& measure );

    }
}
