// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        class LyricData
        {
        public:
            LyricData() {}
            
            std::string text;
        };
        
        MXAPI_EQUALS_BEGIN( LyricData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LyricData );
    }
}
