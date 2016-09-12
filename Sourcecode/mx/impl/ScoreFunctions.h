// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Document.h"
#include "mx/api/ScoreData.h"

namespace mx
{
    namespace impl
    {
    	core::DocumentPtr createDocument( const api::ScoreData& score );
        api::ScoreData createScore( const core::Document& document );
    }
}
