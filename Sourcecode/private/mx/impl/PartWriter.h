// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"

#include <memory>
#include <mutex>

namespace mx
{
    namespace core
    {
        class PartwisePart;
        using PartwisePartPtr = std::shared_ptr<PartwisePart>;
        class ScorePart;
        using ScorePartPtr = std::shared_ptr<ScorePart>;
    }

    namespace impl
    {
        class ScoreWriter;
        
        class PartWriter
        {
        public:
            PartWriter( const api::PartData& inPartData, int inPartIndex, int inTicksPerQuarter, const ScoreWriter& inScoreWriter );
            core::ScorePartPtr getScorePart() const;
            core::PartwisePartPtr getPartwisePart() const;

        private:
            const api::PartData& myPartData;
            const int myPartIndex;
            const int myTicksPerQuarter;
            mutable std::mutex myMutex;
            mutable core::ScorePartPtr myOutScorePart;
            mutable core::PartwisePartPtr myOutPartwisePart;
            const ScoreWriter& myScoreWriter;
            
        private:
            /// Writes all the measures from myPartData to myOutScorePart
            void writeMeasures() const;

            /// Writes all the measures from inPartData to myOutScorePart. We added this function,
            /// in which the part data is passed in, to handle a case when there are zero measures
            /// in myPartData, but we need to force a single measure to exist in the MusicXML to
            /// preserve something about the part.
            void writeMeasures( const mx::api::PartData& inPartData ) const;
        };
    }
}
