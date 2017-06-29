// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

#include <set>

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
        class MusicDataChoice;
        using MusicDataChoicePtr = std::shared_ptr<MusicDataChoice>;
        using MusicDataChoiceSet = std::vector<MusicDataChoicePtr>;
	}

    namespace impl
    {
        class DirectionWriter
        {
        public:
            DirectionWriter( const api::DirectionData& inDirectionData, const Cursor& inCursor );
            core::MusicDataChoiceSet getDirectionLikeThings();

        private:
            void addDirectionType( const core::DirectionTypePtr& directionType, const core::DirectionPtr& ioDirection );
            core::MusicDataChoiceSet createHarmonyElements();
            void addMusicDataChoices( const core::MusicDataChoiceSet& inMdcs, core::MusicDataChoiceSet& ioOutputSet );

        private:
            const api::DirectionData& myDirectionData;
            const Cursor myCursor;
            const Converter myConverter;
            bool myIsFirstDirectionTypeAdded;
            std::set<api::Placement> myPlacements;
        };
    }
}
