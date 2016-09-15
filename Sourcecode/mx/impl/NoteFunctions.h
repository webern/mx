// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/NoteData.h"
#include "mx/core/Enums.h"
#include "mx/impl/Cursor.h"

#include <map>

namespace mx
{
    namespace core
    {
        class Note;
    }
    
    namespace impl
    {
        class NoteFunctions
        {
        public:
            NoteFunctions();
            ~NoteFunctions() = default;
            NoteFunctions( const NoteFunctions& ) = default;
            NoteFunctions( NoteFunctions&& ) = default;
            NoteFunctions& operator=( const NoteFunctions& ) = default;
            NoteFunctions& operator=( NoteFunctions&& ) = default;
            
            api::NoteData parseNote( const core::Note& inMxNote, const impl::Cursor& cursor ) const;
            
            api::Step convert( core::StepEnum value ) const;
            core::StepEnum convert( api::Step value ) const;
            api::DurationName convert( core::NoteTypeValue value ) const;
            core::NoteTypeValue convert( api::DurationName value ) const;
            core::BeamValue convert( api::Beam value ) const;
            api::Beam convert( core::BeamValue value ) const;
            
            static std::map<core::StepEnum, api::Step> stepMap;
            static std::map<core::NoteTypeValue, api::DurationName> durationMap;
            static std::map<core::BeamValue, api::Beam> beamMap;
        };
    }
}
