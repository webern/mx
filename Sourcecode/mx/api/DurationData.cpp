// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/DurationData.h"

namespace mx
{
    namespace api
    {
        DurationData::DurationData()
        : durationName{ DurationName::unspecified }
        , durationDots{ 0 }
        , durationTimeTicks{ 0 }
        , isTied{ false }
        , timeModificationActualNotes{ 1 }
        , timeModificationNormalNotes{ 1 }
        , timeModificationNormalType{ api::DurationName::unspecified }
        , timeModificationNormalTypeDots{ 0 }
        {
            
        }
    }
}
