// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/ScoreData.h"

namespace mx
{
	namespace core
	{
        class ScorePartwise;
	}

    namespace impl
    {
        void createMxFromPartData( const api::ScoreData& inScoreData, core::ScorePartwise& outMx );
        void createPartDataFromMx( const core::ScorePartwise& inMx, api::ScoreData& outScoreData );
    }
}
