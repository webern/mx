// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageMarginsData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api {

        /// A separate class is used for page size (and other sizes) because both of its members, height and width are
        /// required when either is present. i.e. they are inseparable.
        class SizeData {
        public:

            /// Page height in tenths.
            Double height;

            /// Page width in tenths.
            Double width;

            inline explicit SizeData( Double inHeight, Double inWidth )
            : height{ inHeight }
            , width{ inWidth }
            {

            }

            inline SizeData()
                    : SizeData{0.0, 0.0 }
            {

            }
        };

        MXAPI_EQUALS_BEGIN( SizeData )
            MXAPI_EQUALS_MEMBER( height )
            MXAPI_EQUALS_MEMBER( width )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SizeData );
    }
}
