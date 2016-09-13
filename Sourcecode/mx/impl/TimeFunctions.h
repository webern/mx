// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/TimeSignatureData.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/ScorePartwise.h"

namespace mx
{
    namespace impl
    {
        bool findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData );
        int findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise );
    }
}
