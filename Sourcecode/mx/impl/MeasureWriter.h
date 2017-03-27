// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/impl/MeasureCursor.h"
#include "mx/api/SystemData.h"
#include "mx/api/BarlineData.h"
#include "mx/impl/PropertiesWriter.h"
#include "mx/impl/Converter.h"

#include <list>
#include <sstream>

namespace mx
{
	namespace core
	{
		class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Properties;
        using PropertiesPtr = std::shared_ptr<Properties>;
        class Direction;
        using DirectionPtr = std::shared_ptr<Direction>;
	}

    namespace impl
    {
        class ScoreWriter;
        
        class MeasureWriter
        {
		public:
            MeasureWriter( const api::MeasureData& inMeasureData, const MeasureCursor& inCursor, const ScoreWriter& inScoreWriter );
        	core::PartwiseMeasurePtr getPartwiseMeasure();

        public:
            class HistoryRecord
            {
            public:
                std::string reason;
                int amount;
                int timeBefore;
                int timeAfter;
                MeasureCursor cursorBefore;
                MeasureCursor cursorAfter;

            };

            struct History
            {
                History( const MeasureCursor& inCursor )
                : myCursor{ inCursor }
                , history{}
                {
                    HistoryRecord record;
                    record.reason = "initialize history";
                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;
                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                std::vector<HistoryRecord> history;
                void advanceTickTimePosition( int amount, std::string reason );
                const MeasureCursor& getCursor() const { return myCursor; }

                void resetCursorFofStaffIterations()
                {
                    HistoryRecord record;
                    record.reason = "resetCursorFofStaffIterations";
                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;

                    myCursor.tickTimePosition = 0;
                    myCursor.staffIndex = 0;
                    myCursor.voiceIndex = 0;

                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                void nextStaff()
                {
                    HistoryRecord record;
                    record.reason = "nextStaff";
                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;

                    ++myCursor.staffIndex;

                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                void setVoiceIndex( int inVoiceIndex )
                {
                    HistoryRecord record;
                    record.reason = "setVoiceIndex to " + std::to_string(inVoiceIndex);
                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;

                    myCursor.voiceIndex = inVoiceIndex;

                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                void setChord( bool isChordActive )
                {
                    if( isChordActive == myCursor.isChordActive )
                    {
                        return;
                    }

                    HistoryRecord record;

                    if( isChordActive )
                    {
                        record.reason = "set isChordActive to true";
                    }
                    else
                    {
                        record.reason = "set isChordActive to false";
                    }

                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;

                    myCursor.isChordActive = isChordActive;

                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                void setTime( int inTickTimePosition, std::string inReason )
                {
                    HistoryRecord record;
                    record.reason = inReason;
                    record.amount = inTickTimePosition - myCursor.tickTimePosition;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;

                    myCursor.tickTimePosition = inTickTimePosition;

                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                void log( std::string message )
                {
                    HistoryRecord record;
                    record.reason = message;
                    record.amount = 0;
                    record.timeBefore = myCursor.tickTimePosition;
                    record.cursorBefore = myCursor;
                    record.cursorAfter = myCursor;
                    record.timeAfter = myCursor.tickTimePosition;
                    history.push_back( record );
                }

                std::string toString() const
                {
                    std::stringstream ss;
                    for( const auto& record : history )
                    {
                        ss << record.timeBefore << " " << record.reason << std::endl;
                    }
                    return ss.str();
                }

            private:
                MeasureCursor myCursor;
            };

        private:
            using NoteIter = std::vector<api::NoteData>::const_iterator;
        	const api::MeasureData& myMeasureData;
        	core::PartwiseMeasurePtr myOutMeasure;
            MeasureCursor myPreviousCursor;
            const ScoreWriter& myScoreWriter;
            std::unique_ptr<PropertiesWriter> myPropertiesWriter;
            const Converter myConverter;
            std::vector<api::BarlineData>::const_iterator myBarlinesIter;
            std::vector<api::BarlineData>::const_iterator myBarlinesEnd;
            std::vector<api::KeyData>::const_iterator myMeasureKeysIter;
            const std::vector<api::KeyData>::const_iterator myMeasureKeysEnd;
            History myHistory;

        private:
            void writeMeasureGlobals();
            void writeSystemInfo();
            void writeStaves();
            void writeVoices( const api::StaffData& inStaff );
            void writeForwardOrBackupIfNeeded( const api::NoteData& currentNote );
            void backup( const int ticks );
            void forward( const int ticks );
            void advanceCursorIfNeeded( const api::NoteData& currentNote, NoteIter inNoteIter, const NoteIter inEndIter, bool isStartOfChord );
            void writeBarlines( int tickTimePosition );
            using dirs = std::vector<api::DirectionData>;
            using dIter = dirs::const_iterator;
            void writeDirections( dIter& directionIter, const dIter& directionEnd, int minTickTimePosition, int maxTickTimePosition );

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
