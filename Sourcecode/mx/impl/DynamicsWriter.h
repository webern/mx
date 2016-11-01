// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/Converter.h"

namespace mx
{
    namespace core
    {
        class Dynamics;
        using DynamicsPtr = std::shared_ptr<Dynamics>;
        using DynamicsSet = std::vector<DynamicsPtr>;
    }
    
    namespace impl
    {
        class DynamicsWriter
        {
        public:
            DynamicsWriter( const api::MarkData& inMark, impl::Cursor inCursor );            
            DynamicsWriter( const api::MarkData& inMark, impl::Cursor inCursor, api::Placement directionPlacement );
            core::DynamicsPtr getDynamics() const;
            
        private:
            const api::MarkData& myMarkData;
            const impl::Cursor myCursor;
            const api::Placement myDirectionPlacement;
            const Converter myConverter;
            
        private:
            
        };
    }
}
