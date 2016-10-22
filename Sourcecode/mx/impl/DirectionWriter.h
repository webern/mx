// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
	namespace core
	{
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Direction;
        using DirectionPtr = std::shared_ptr<Direction>;
        class DirectionType;
        using DirectionTypePtr = std::shared_ptr<DirectionType>;
	}

    namespace impl
    {
        class DirectionWriter
        {
        public:
            DirectionWriter( const api::DirectionData& inDirectionData, const Cursor& inCursor );
            core::DirectionPtr getDirection();

        private:
            void addDirectionType( const core::DirectionTypePtr& directionType );

        private:
            const api::DirectionData& myDirectionData;
            const Cursor myCursor;
            core::DirectionPtr myOutDirectionPtr;
            const Converter myConverter;
            bool myIsFirstDirectionTypeAdded;
        };
    }
}
