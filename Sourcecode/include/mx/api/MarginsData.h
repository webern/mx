// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {

        /// top, bottom, left, right. Measurements are in tenths.
        class MarginsData
        {
        public:
            Double left;
            Double right;
            Double top;
            Double bottom;

            inline MarginsData()
            : MarginsData{ 0.0, 0.0, 0.0, 0.0 }
            {

            }

            inline explicit MarginsData( Double inLeft, Double inRight, Double inTop, Double inBottom )
                : left{ inLeft }
                , right{ inRight }
                , top{ inTop }
                , bottom{ inBottom }
            {

            }
        };

        MXAPI_EQUALS_BEGIN( MarginsData )
            MXAPI_EQUALS_MEMBER( left )
            MXAPI_EQUALS_MEMBER( right )
            MXAPI_EQUALS_MEMBER( top )
            MXAPI_EQUALS_MEMBER( bottom )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MarginsData );
    }
}
