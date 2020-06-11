// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageMarginsData.h"
#include "mx/api/SizeData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        // TODO - documentations
        class PageLayoutData
        {
        public:

            /// Page size, in tenths.
            std::optional<SizeData> size;

            /// Page margin data (note all members of PageMarginsData are optional).
            PageMarginsData margins;

            PageLayoutData()
                : size( std::nullopt )
                , margins{}
            {

            }

            inline bool isUsed() const
            {
                return size || margins.isUsed();
            }
        };

        MXAPI_EQUALS_BEGIN( PageLayoutData )
            MXAPI_EQUALS_MEMBER( size )
            MXAPI_EQUALS_MEMBER( margins )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageLayoutData );
    }
}
