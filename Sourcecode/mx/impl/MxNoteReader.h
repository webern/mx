// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/Enums.h"

namespace mx
{
    namespace core
    {
        class Note;
        class NoteChoice;
        class FullNoteGroup;
    }
    
    namespace impl
    {

        class MxNoteReader
        {
        public:
            MxNoteReader( const core::Note& mxNote );
            ~MxNoteReader() = default;
            MxNoteReader( const MxNoteReader& ) = delete;
            MxNoteReader( MxNoteReader&& ) = delete;
            MxNoteReader& operator=( const MxNoteReader& ) = delete;
            MxNoteReader& operator=( MxNoteReader&& ) = delete;

            inline const core::Note& getMxNote() const { return myNote; }
            inline const core::NoteChoice& getMxNoteChoice() const { return myNoteChoice; }
            inline const core::FullNoteGroup& getMxFullNoteGroup() const { return myFullNoteGroup; }
            inline bool getIsNormal() const { return myIsNormal; }
            inline bool getIsGrace() const { return myIsGrace; }
            inline bool getIsCue() const { return myIsCue; }
            inline bool getIsRest() const { return myIsRest; }
            inline bool getIsMeasureRest() const { return myIsMeasureRest; }
            inline bool getIsUnpitched() const { return myIsUnpitched; }
            inline bool getIsPitch() const { return myIsPitch; }
            inline bool getIsDisplayStepOctaveSpecified() const { return myIsDisplayStepOctaveSpecified; }
            inline long double getDurationValue() const { return myDurationValue; }
            inline int getTieCount() const { return myTieCount; }
            inline core::StepEnum getStep() const { return myStep; }
            inline int getAlter() const { return myAlter; }
            inline int getOctave() const { return myOctave; }
            inline int getStaffNumber() const { return myStaffNumber; }
            inline int getVoiceNumber() const { return myVoiceNumber; }
            inline core::NoteTypeValue getDurationType() const { return myDurationType; }
            inline bool getIsDurationTypeSpecified() const { return myIsDurationTypeSpecified; }
            inline int getNumDots() const { return myNumDots; }

		private:
        	const core::Note& myNote;
            const core::NoteChoice& myNoteChoice;
            const core::FullNoteGroup& myFullNoteGroup;
            bool myIsNormal;
            bool myIsGrace;
            bool myIsCue;
            bool myIsRest;
            bool myIsMeasureRest;
            bool myIsUnpitched;
            bool myIsPitch;
            bool myIsDisplayStepOctaveSpecified;
            long double myDurationValue;
            int myTieCount;
            core::StepEnum myStep;
            int myAlter;
            int myOctave;
            int myStaffNumber;
            int myVoiceNumber;
            core::NoteTypeValue myDurationType;
            bool myIsDurationTypeSpecified;
            int myNumDots;

        private:
            const core::FullNoteGroup& findFullNoteGroup( const core::NoteChoice& noteChoice ) const;
            void setNormalGraceCueItems();
            void setRestPitchUnpitchedItems();
            void setStaffNumber();
            void setVoiceNumber();
            void setDurationType();
            void setNumDots();
        };
    }
}
