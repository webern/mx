// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteAttachmentData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
    namespace core
    {
        class Slide;
    }
    
    namespace impl
    {
        class SlideFunctions
        {
        public:
            SlideFunctions( const core::Slide& inSlide, impl::Cursor inCursor );
            ~SlideFunctions() = default;
            SlideFunctions( const SlideFunctions& ) = default;
            SlideFunctions( SlideFunctions&& ) = default;
            void parseSlide( api::NoteAttachmentData& inNoteAttachmentData ) const;
            
        private:
            const core::Slide& mySlide;
            const impl::Cursor myCursor;
        };
    }
}
