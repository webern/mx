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
            const ScoreWriter& myScoreWriter;

        private:
            void writeSystemInfo() const;
            void writeFirstMeasureProperties() const;
            core::Properties& createAndInsertMeasureProperties() const;
            void writeDivisions( core::Properties& outProperties ) const;
            void writeKey(int staffIndex, const api::KeyData& inKeyData, core::Properties& outProperties ) const;
            void writeTime( core::Properties& outProperties ) const;
            void writeNumStaves( core::Properties& outProperties ) const;
            void writeClef( int staffIndex, const api::ClefData& inClefData, core::Properties& outProperties ) const;
            void writeInitialClefs( core::Properties& outProperties ) const;
            void writeInitialKeys( core::Properties& outProperties ) const;
        };
    }
}
