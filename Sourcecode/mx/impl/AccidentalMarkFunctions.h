// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
    namespace core
    {
        class AccidentalMark;
    }
    
    namespace impl
    {
        class AccidentalMarkFunctions
        {
        public:
            AccidentalMarkFunctions( const core::AccidentalMark& inAccidentalMark, impl::Cursor inCursor );
            ~AccidentalMarkFunctions() = default;
            AccidentalMarkFunctions( const AccidentalMarkFunctions& ) = default;
            AccidentalMarkFunctions( AccidentalMarkFunctions&& ) = default;
            AccidentalMarkFunctions& operator=( const AccidentalMarkFunctions& ) = default;
            AccidentalMarkFunctions& operator=( AccidentalMarkFunctions&& ) = default;
            
            api::MarkData parseAccidentalMark() const;
            
        private:
            const core::AccidentalMark& myAccidentalMark;
            const impl::Cursor myCursor;
        };
    }
}
