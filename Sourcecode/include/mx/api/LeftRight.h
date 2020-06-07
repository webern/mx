// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageLayoutData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        /// Represents left and right measurements, in tenths, that are inseparable.
        class LeftRight
        {
        public:

            /// Left measurement, in tenths.
            Double left;

            /// Right measurement, in tenths.
            Double right;

            LeftRight()
                : left( 0.0 )
                , right( 0.0 )
            {

            }


        };

        MXAPI_EQUALS_BEGIN( LeftRight )
            MXAPI_EQUALS_MEMBER( left )
            MXAPI_EQUALS_MEMBER( right )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LeftRight );
    }
}
