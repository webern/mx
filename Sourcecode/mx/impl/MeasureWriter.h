// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/api/SystemData.h"
#include "mx/impl/PropertiesWriter.h"
#include "mx/impl/Converter.h"

#include <list>

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
        	core::PartwiseMeasurePtr getPartwiseMeasure();

        private:
        	const api::MeasureData& myMeasureData;
        	core::PartwiseMeasurePtr myOutMeasure;
            MeasureCursor myCursor;
            MeasureCursor myPreviousCursor;
            const ScoreWriter& myScoreWriter;
            std::unique_ptr<PropertiesWriter> myPropertiesWriter;
            std::list<api::KeyData> myMeasureKeys;
            const Converter myConverter;

        private:
            void writeMeasureGlobals();
            void writeSystemInfo();
            void writeStaves();
            void writeVoices( const api::StaffData& inStaff );            void writeForwardOrBackupIfNeeded( const api::NoteData& currentNote );
            void backup( const int ticks );
            void forward( const int ticks );
            void advanceCursorIfNeeded( const api::NoteData& currentNote );

            
            template<typename T>
            std::vector<T> findItemsAtTimePosition( const std::vector<T>& inItems, int inTickTimePosition )
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
