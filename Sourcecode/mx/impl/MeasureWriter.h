// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/api/SystemData.h"

namespace mx
{
	namespace core
	{
		class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Properties;
        using PropertiesPtr = std::shared_ptr<Properties>;
	}

    namespace impl
    {
        class ScoreWriter;
        
        class MeasureWriter
        {
		public:
            MeasureWriter( const api::MeasureData& inMeasureData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter );
        	core::PartwiseMeasurePtr getPartwiseMeasure() const;

        private:
        	const api::MeasureData& myMeasureData;
        	mutable core::PartwiseMeasurePtr myOutMeasure;
            mutable MeasureCursor myCursor;
            mutable MeasureCursor myPreviousCursor;
            const ScoreWriter& myScoreWriter;

        private:
            void writeSystemInfo() const;
            void writeFirstMeasureProperties() const;
            core::PropertiesPtr createSubsequentMeasureStartingPoperties() const;
            bool isMeasurePropertiesRequired( const core::Properties& props ) const;
            core::Properties& createAndInsertMeasureProperties() const;
            void writeDivisions( core::Properties& outProperties ) const;
            void writeKey(int staffIndex, const api::KeyData& inKeyData, core::Properties& outProperties ) const;
            void writeTime( core::Properties& outProperties ) const;
            void writeNumStaves( core::Properties& outProperties ) const;
            void writeClef( int staffIndex, const api::ClefData& inClefData, core::Properties& outProperties ) const;
            void writeInitialClefs( core::Properties& outProperties ) const;
            void writeInitialKeys( core::Properties& outProperties ) const;
            void writeStaffData() const;
            void writeVoiceData( const api::StaffData& inStaff ) const;
            
            template<typename T>
            std::vector<T> findItemsAtTimePosition( const std::vector<T>& inItems, int inTickTimePosition ) const
            {
                std::vector<T> outVector;
                for( const auto& item : inItems )
                {
                    if( item.tickTimePosition == inTickTimePosition )
                    {
                        outVector.push_back( item );
                    }
                }
                return outVector;
            }
        };
    }
}
