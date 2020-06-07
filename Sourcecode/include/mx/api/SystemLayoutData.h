// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageLayoutData.h"
#include "mx/api/LeftRight.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        /// Margins and spacing for staff systems.
        class SystemLayoutData
        {
        public:

            /// The left and right margins for the system, in tenths.
            std::optional<LeftRight> margins;

            /// Distance from bottom line of one system to top line of next system, in tenths.
            OptionalDouble systemDistance;

            /// Distance from the top margin of the page to the top line of the first staff on the page, in tenths.
            OptionalDouble topSystemDistance;

            /// the space between staves within the same system, in tenths.
            OptionalDouble staffDistance;

            /// Returns true if any of the members have values.
            inline bool isUsed() const { return margins || systemDistance || topSystemDistance || staffDistance; }

            explicit inline SystemLayoutData(
                    std::optional<LeftRight> inMargins = std::nullopt,
                    OptionalDouble inSystemDistance = std::nullopt,
                    OptionalDouble inTopSystemDistance = std::nullopt,
                    OptionalDouble inStaffDistance = std::nullopt
            )
                : margins( inMargins )
                , systemDistance( inSystemDistance )
                , topSystemDistance{ inTopSystemDistance }
                , staffDistance( inStaffDistance )
            {

            }
        };

        MXAPI_EQUALS_BEGIN( SystemLayoutData )
            MXAPI_EQUALS_MEMBER( margins )
            MXAPI_EQUALS_MEMBER( systemDistance )
            MXAPI_EQUALS_MEMBER( topSystemDistance )
            MXAPI_EQUALS_MEMBER( staffDistance )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SystemLayoutData );
    }
}
