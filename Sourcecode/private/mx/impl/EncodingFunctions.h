// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/EncodingData.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/ScoreHeaderGroup.h"

namespace mx
{
    namespace impl
    {
        void createEncoding( const api::EncodingData& inEncoding, core::ScoreHeaderGroup& header );
        api::EncodingData createEncoding( const core::Encoding& inEncoding );
    }
}
