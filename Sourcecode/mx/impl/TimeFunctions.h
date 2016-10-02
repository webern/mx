// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
    }
    
    namespace impl
    {
    	class TimeFunctions
    	{
        public:
	        bool findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData ) const;
	        bool isTimeSignatureImplicit( const api::TimeSignatureData& previousTimeSignature, const api::TimeSignatureData& currentTimeSignature, const bool isFirstMeasureInPart ) const;
    	    int findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise ) const;
            int convertDurationToGlobalTickScale( const impl::Cursor& cursor, const core::Duration& duration ) const;
            int convertDurationToGlobalTickScale( const impl::Cursor& cursor, long double durationValue ) const;
        };
    }
}
