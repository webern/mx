// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/impl/Cursor.h"

#include <mutex>

namespace mx
{
	namespace core
	{
		class ScorePartwise;
        using ScorePartwisePtr = std::shared_ptr<ScorePartwise>;
        class PartGroup;
        using PartGroupPtr = std::shared_ptr<PartGroup>;
        class PartGroupOrScorePart;
        using PartGroupOrScorePartPtr = std::shared_ptr<PartGroupOrScorePart>;
        class ScorePart;
        using ScorePartPtr = std::shared_ptr<ScorePart>;
        class PartwisePart;
        using PartwisePartPtr = std::shared_ptr<PartwisePart>;
	}

    namespace impl
    {
    	class ScoreWriter
    	{
    	public:
            ScoreWriter( const api::ScoreData& inScoreData );

    		core::ScorePartwisePtr getScorePartwise() const;

            bool isStartOfSystem( int measureIndex ) const;
            api::SystemData getSystemData( int measureIndex ) const;
            
    	private:
    		const api::ScoreData& myScoreData;
            mutable std::mutex myMutex;
            mutable core::ScorePartwisePtr myOutScorePartwise;
            
        private:
            void addScorePart( int partIndex, const core::ScorePartPtr& scorePart ) const;
            void addPartwisePart( int partIndex, const core::PartwisePartPtr& partwisePart ) const;
            bool partGroupStartExists( int partIndex ) const;
            bool partGroupStopExists( int partIndex ) const;
            std::vector<api::PartGroupData> findPartGroupsByStartIndex( int partIndex ) const;
            std::vector<api::PartGroupData> findPartGroupsByStopIndex( int partIndex ) const;
            core::PartGroupPtr makePartGroupStart( const api::PartGroupData& apiGrp ) const;
            core::PartGroupPtr makePartGroupStop( const api::PartGroupData& apiGrp ) const;
            core::PartGroupOrScorePartPtr makePartGroupOrScorePart( const core::PartGroupPtr& grp ) const;
            core::PartGroupOrScorePartPtr makePartGroupOrScorePart( const core::ScorePartPtr& spr ) const;
    	};
    }
}



/*
 // TODO
 if( myScoreWriter.isStartOfSystem( myCursor.measureIndex ) )
 {
 auto systemData = myScoreWriter.getSystemData( myCursor.measureIndex );
 // TODO - add it to the measure
 }
 
 */
