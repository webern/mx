// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Enums.h"
#include "mx/core/elements/Tie.h"
#include "mx/impl/LyricType.h"
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

        class NoteReader
        {
        public:
            NoteReader( const core::Note& mxNote );
            ~NoteReader() = default;
            NoteReader( const NoteReader& ) = delete;
            NoteReader( NoteReader&& ) = delete;
            NoteReader& operator=( const NoteReader& ) = delete;
            NoteReader& operator=( NoteReader&& ) = delete;

            inline const core::Note& getMxNote() const { return myNote; }
            inline const core::NoteChoice& getMxNoteChoice() const { return myNoteChoice; }
            inline const core::FullNoteGroup& getMxFullNoteGroup() const { return myFullNoteGroup; }
            inline bool getIsNormal() const { return myIsNormal; }
            inline bool getIsGrace() const { return myIsGrace; }
            inline bool getIsCue() const { return myIsCue; }
            inline bool getIsRest() const { return myIsRest; }
            
            // careful - this only means that the MusicXML <note> has a <chord> tag
            // in fact a note is part of a chord if the subsequent note has a <chord> tag
            inline bool getIsChord() const { return myIsChord; }
            
            inline bool getIsMeasureRest() const { return myIsMeasureRest; }
            inline bool getIsUnpitched() const { return myIsUnpitched; }
            inline bool getIsPitch() const { return myIsPitch; }
            inline bool getIsDisplayStepOctaveSpecified() const { return myIsDisplayStepOctaveSpecified; }
            inline long double getDurationValue() const { return myDurationValue; }
            inline core::StepEnum getStep() const { return myStep; }
            inline int getAlter() const { return myAlter; }
            inline int getCents() const { return myCents; }
            inline int getOctave() const { return myOctave; }
            inline int getStaffNumber() const { return myStaffNumber; }
            inline int getVoiceNumber() const { return myVoiceNumber; }
            inline core::NoteheadValue getNoteheadValue() const { return myNoteheadValue; }
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
            inline bool getIsStemSpecified() const { return myIsStemSpecified; }
            inline core::StemValue getStem() const { return myStem; }
            bool getIsTieStart() const { return myIsTieStart; }
            bool getIsTieStop() const { return myIsTieStop; }
            
		private:
        	const core::Note& myNote;
            const core::NoteChoice& myNoteChoice;
            const core::FullNoteGroup& myFullNoteGroup;
            bool myIsNormal;
            bool myIsGrace;
            bool myIsCue;
            bool myIsRest;
            bool myIsChord;
            bool myIsMeasureRest;
            bool myIsUnpitched;
            bool myIsPitch;
            bool myIsDisplayStepOctaveSpecified;
            long double myDurationValue;
            core::StepEnum myStep;
            int myAlter;
            double myCents;
            int myOctave;
            int myStaffNumber;
            int myVoiceNumber;
            core::NoteheadValue myNoteheadValue;
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
            bool myIsStemSpecified;
            core::StemValue myStem;
            bool myIsTieStart;
            bool myIsTieStop;
            bool myHasLyric;
            std::vector<LyricType> myLyrics;

        private:
            const core::FullNoteGroup& findFullNoteGroup( const core::NoteChoice& noteChoice ) const;
            void setNormalGraceCueItems();
            void setRestPitchUnpitchedItems();
            void setChord();
            void setStaffNumber();
            void setVoiceNumber();
            void setNoteheadValue();
            void setDurationType();
            void setNumDots();
            void setBeams();
            void setTimeModification();
            void setAccidental();
            void setStem();
            void setTie( const core::TieSet& tieSet );
            void setLyric();

        };
    }
}
