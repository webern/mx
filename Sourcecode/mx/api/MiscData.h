// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
    namespace api
    {
        class MiscData
        {
        public:
            std::string name;
            std::string data;
        };

        MXAPI_EQUALS_BEGIN( MiscData )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( data )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MiscData );
    }
}
