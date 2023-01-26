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
        class NonArpeggiate;
    }
    
    namespace impl
    {
        class NonArpeggiateFunctions
        {
        public:
            NonArpeggiateFunctions( const core::NonArpeggiate& inNonArpeggiate, impl::Cursor inCursor );
            ~NonArpeggiateFunctions() = default;
            NonArpeggiateFunctions( const NonArpeggiateFunctions& ) = default;
            NonArpeggiateFunctions( NonArpeggiateFunctions&& ) = default;
            api::MarkData parseNonArpeggiate() const;
            
        private:
            const core::NonArpeggiate& myNonArpeggiate;
            const impl::Cursor myCursor;
        };
    }
}
