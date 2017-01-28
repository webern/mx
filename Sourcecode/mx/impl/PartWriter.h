// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"
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
            void writeMeasures() const;
    	};
    }
}
