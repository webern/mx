// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/impl/Cursor.h"

namespace mx
{
    namespace core
    {
        class Tuplet;
        class Note;
    }
    
    namespace impl
    {
        class TupletReader
        {
        public:
            TupletReader( const core::Tuplet& inMxTuplet, impl::Cursor inCursor, const core::Note& inNote );
            void parseTuplet( std::vector<api::TupletStart>& outTupletStarts, std::vector<api::TupletStop>& outTupletStops );
            
        private:
            const core::Tuplet& myTuplet;
            const core::Note& myNote;
            const impl::Cursor myCursor;

        private:
            void guessNormalFromNote( api::TupletStart& outTupletStart );
            void guessActualFromNote( api::TupletStart& outTupletStart );
        };
    }
}
