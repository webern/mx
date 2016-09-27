// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/NoteData.h"
#include "mx/core/Enums.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/Converter.h"

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
        };
    }
}
