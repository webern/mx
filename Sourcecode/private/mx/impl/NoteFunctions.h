// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteData.h"
#include "mx/core/Enums.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

#include <mutex>

namespace mx
{
    namespace core
    {
        class Note;
        //using NotePtr = std::shared_ptr<Note>;
        class Notations;
        using NotationsPtr = std::shared_ptr<Notations>;
        using NotationsSet = std::vector<NotationsPtr>;
        class ArticulationsChoice;
        using ArticulationsChoicePtr = std::shared_ptr<ArticulationsChoice>;
        using ArticulationsChoiceSet = std::vector<ArticulationsChoicePtr>;
        class Dynamics;
        class Fermata;
        class Arpeggiate;
        class NonArpeggiate;
        class AccidentalMark;
        class OtherNotation;
    }
    
    namespace impl
    {
        class NoteReader;
        
        class NoteFunctions
        {
        public:
            NoteFunctions( const core::Note& inMxNote, impl::Cursor cursor );
            ~NoteFunctions() = default;
            NoteFunctions( const NoteFunctions& ) = default;
            NoteFunctions( NoteFunctions&& ) = default;
            NoteFunctions& operator=( const NoteFunctions& ) = default;
            NoteFunctions& operator=( NoteFunctions&& ) = default;
            
            api::NoteData parseNote() const;
            
        private:
            const core::Note& myNote;
            const impl::Cursor myCursor;
            mutable api::NoteData myOutNoteData;
            mutable std::mutex myMutex;
            const Converter myConverter;
            
        private:
            api::DurationName deriveNoteTypeFromDurationValue( const NoteReader& reader ) const;
            void parseNotations() const;
            void parseMiscData() const;
        };
    }
}
