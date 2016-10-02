// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/Cursor.h"

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
        class DynamicsFunctions
        {
        public:
            DynamicsFunctions( const core::Dynamics& inDynamic, impl::Cursor inCursor );
            ~DynamicsFunctions() = default;
            DynamicsFunctions( const DynamicsFunctions& ) = delete;
            DynamicsFunctions( DynamicsFunctions&& ) = delete;
            DynamicsFunctions& operator=( const DynamicsFunctions& ) = delete;
            DynamicsFunctions& operator=( DynamicsFunctions&& ) = delete;
            
            void parseDynamics( std::vector<api::MarkData>& outMarks ) const;
            
        private:
            const core::Dynamics& myDynamic;
            const impl::Cursor myCursor;
            
        private:
            void parseDynamic( const core::Dynamics& inDynamic, api::MarkData& outMark ) const;
        };
    }
}
