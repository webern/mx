// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
    namespace core
    {
        class Arpeggiate;
    }
    
    namespace impl
    {
        class ArpeggiateFunctions
        {
        public:
            ArpeggiateFunctions( const core::Arpeggiate& inArpeggiate, impl::Cursor inCursor );
            ~ArpeggiateFunctions() = default;
            ArpeggiateFunctions( const ArpeggiateFunctions& ) = default;
            ArpeggiateFunctions( ArpeggiateFunctions&& ) = default;
            api::MarkData parseArpeggiate() const;
            
        private:
            const core::Arpeggiate& myArpeggiate;
            const impl::Cursor myCursor;
        };
    }
}
