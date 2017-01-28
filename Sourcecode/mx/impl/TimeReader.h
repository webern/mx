// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TimeSignatureData.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/impl/Cursor.h"

namespace mx
{
    namespace core
    {
        class Duration;
        class MusicDataChoice;
        using MusicDataChoicePtr = std::shared_ptr<MusicDataChoice>;
        using MusicDataChoiceSet = std::vector<MusicDataChoicePtr>;
        class Time;
        using TimePtr = std::shared_ptr<Time>;
        class TimeSignatureGroup;
    }
    
    namespace impl
    {
    	class TimeReader
    	{
        public:
            
            // The constructor will do all of the parsing
            // after that you can query the discovered time
            // element properties using accessors on the
            // cached data
            TimeReader( const core::MusicDataChoiceSet& inMusicDataChoices );
            bool getIsTimeFound() const;
            mx::api::TimeSignatureData getTimeSignatureData() const;

        private:
            const core::MusicDataChoiceSet& myMusicDataChoiceSet;
            core::TimePtr myTime;
            bool myIsTimeFound;
            mx::api::TimeSignatureData myTimeSignatureData;
            
        private:
            bool initialize();
            bool parseTime();
            bool parseTimeSignatureGroup( const core::TimeSignatureGroup& timeSig );
            
	        //bool findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData ) const;
	        //bool isTimeSignatureImplicit( const api::TimeSignatureData& previousTimeSignature, const api::TimeSignatureData& currentTimeSignature, const bool isFirstMeasureInPart ) const;
    	    int findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise ) const;
//            int convertDurationToGlobalTickScale( const impl::Cursor& cursor, const core::Duration& duration ) const;
//            int convertDurationToGlobalTickScale( const impl::Cursor& cursor, long double durationValue ) const;
        };
    }
}
