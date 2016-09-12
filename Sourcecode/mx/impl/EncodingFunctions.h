// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Encoding.h"
#include "mx/api/EncodingData.h"
#include "mx/core/elements/ScoreHeaderGroup.h"

namespace mx
{
    namespace impl
    {
        void createEncoding( const api::EncodingData& inEncoding, core::ScoreHeaderGroup& header );
        api::EncodingData createEncoding( const core::Encoding& inEncoding );
    }
}
