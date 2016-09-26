// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/Enums.h"

#include <vector>

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
            inline const std::vector<core::BeamValue>& getBeams() const { return myBeams; }
            inline int getTimeModificationActualNotes() const { return myTimeModificationActualNotes; }
            inline int getTimeModificationNormalNotes() const { return myTimeModificationNormalNotes; }
            inline core::NoteTypeValue getTimeModificationNormalType() const { return myTimeModificationNormalType; }
            inline int getTimeModificationNormalTypeDots() const { return myTimeModificationNormalTypeDots; }
            inline bool getHasAccidental() const { return myHasAccidental; }
            inline core::AccidentalValue getAccidental() const { return myAccidental; }
            inline bool getIsAccidentalParenthetical() const { return myIsAccidentalParenthetical; }
            inline bool getIsAccidentalCautionary() const { return myIsAccidentalCautionary; }
            inline bool getIsAccidentalEditorial() const { return myIsAccidentalEditorial; }
            inline bool getIsAccidentalBracketed() const { return myIsAccidentalBracketed; }
            
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
            std::vector<core::BeamValue> myBeams;
            int myTimeModificationActualNotes;
            int myTimeModificationNormalNotes;
            core::NoteTypeValue myTimeModificationNormalType;
            int myTimeModificationNormalTypeDots;
            bool myHasAccidental;
            core::AccidentalValue myAccidental;
            bool myIsAccidentalParenthetical;
            bool myIsAccidentalCautionary;
            bool myIsAccidentalEditorial;
            bool myIsAccidentalBracketed;

        private:
            const core::FullNoteGroup& findFullNoteGroup( const core::NoteChoice& noteChoice ) const;
            void setNormalGraceCueItems();
            void setRestPitchUnpitchedItems();
            void setStaffNumber();
            void setVoiceNumber();
            void setDurationType();
            void setNumDots();
            void setBeams();
            void setTimeModification();
            void setAccidental();
        };
    }
}
