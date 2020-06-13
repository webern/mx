// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/MarginsData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {

        /// Page margins are specified separately for odd and even pages. If odd and even pages have the same values,
        /// then then a type of "both" will be used in the produced MusicXML. All values are optional.
        class PageMarginsData
        {
        public:
            /// Margins for odd pages.
            std::optional<MarginsData> odd;

            /// Margins for even pages.
            std::optional<MarginsData> even;

            PageMarginsData()
                : odd{ std::nullopt }
                , even{ std::nullopt }
            {

            }

            /// Returns true if any values have been specified. If false, then PageMarginsData can be ignored.
            inline bool isUsed() const
            {
                return odd || even;
            }

            /// Returns true if the odd page margins are equal to the even page margins (i.e. if a 'both' attribute
            /// type can be used in MusicXML).
            inline bool same() const
            {
                return odd == even;
            }
        };

        MXAPI_EQUALS_BEGIN( PageMarginsData )
            MXAPI_EQUALS_MEMBER( odd )
            MXAPI_EQUALS_MEMBER( even )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageMarginsData );
    }
}
