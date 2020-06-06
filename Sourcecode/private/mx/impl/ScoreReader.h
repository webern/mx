// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"

#include <mutex>
#include <list>
#include <map>
#include <memory>

namespace mx
{
	namespace core
	{
		class ScorePartwise;
        using ScorePartwisePtr = std::shared_ptr<ScorePartwise>;
        class ScorePart;
        using ScorePartPtr = std::shared_ptr<ScorePart>;
        class ScoreHeaderGroup;
        class PartwisePart;
        using PartwisePartPtr = std::shared_ptr<PartwisePart>;
        using PartwisePartSet = std::vector<PartwisePartPtr>;
        class PartGroup;
        using PartGroupPtr = std::shared_ptr<PartGroup>;
	}

    namespace impl
    {
    	class ScoreReader
    	{
    	public:
            ScoreReader( const core::ScorePartwise& inScorePartwise );

    		api::ScoreData getScoreData() const;

    	private:
    		const core::ScorePartwise& myScorePartwise;
            const core::PartwisePartSet& myPartSet;
            const core::ScoreHeaderGroup& myHeaderGroup;
            
        private:
            mutable std::mutex myMutex;
            mutable api::ScoreData myOutScoreData;
            mutable std::list<api::PartGroupData> myPartGroupStack;
            
        private:
            using ReconciledPart = std::pair<core::ScorePartPtr, core::PartwisePartPtr>;
            using ReconciledParts = std::vector<ReconciledPart>;
            
            const core::PartwisePartPtr findPartwisePart( const core::ScorePartPtr& scorePartPtr, const core::PartwisePartSet& partwiseParts ) const;
            ReconciledParts reconcileParts( const core::ScorePartwise& inScorePartwise ) const;
            void handlePartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const;
            void startPartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const;
            void stopPartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const;
            bool groupNumberExistsOnStack( int groupNumber ) const;
            api::PartGroupData popGroupFromStack( int groupNumber ) const;
            api::PartGroupData popMostRecentGroupFromStack() const;
            int parsePartGroupNumber( const core::PartGroupPtr& inPartGroup ) const;
            void scanForSystemInfo() const;
            void scanForPageInfo() const;
            int findMaxDivisionsPerQuarter() const;
    	};
    }
}
