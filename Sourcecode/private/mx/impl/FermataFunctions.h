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
        class Fermata;
    }
    
    namespace impl
    {
        class FermataFunctions
        {
        public:
            FermataFunctions( const core::Fermata& inFermata, impl::Cursor inCursor );
            ~FermataFunctions() = default;
            FermataFunctions( const FermataFunctions& ) = default;
            FermataFunctions( FermataFunctions&& ) = default;
            api::MarkData parseFermata() const;
            
        private:
            const core::Fermata& myFermata;
            const impl::Cursor myCursor;
        };
    }
}
