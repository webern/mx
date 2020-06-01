// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PageLayoutData.h"

#include <string>
#include <vector>
#include <optional>

namespace mx
{
    namespace api
    {
        /// The location of a page break or page layout change can be specified either by giving the measure index (i.e.
        /// in the case of a page break, the index of the first measure that should appear on the new page) or by giving
        /// a system index (i.e. in the case of a page break, the index of the first SystemData that should appear on
        /// the new page).
        enum StartSpecifier
        {
            measureIndex,
            systemIndex,
        };

        /// PageData allows the specification of a new page, or the prevention of a new page, along with the ability
        /// to change the page layout settings. `firstSystemIndex` is the index of the first system (in score.systems)
        /// on the page.
        class PageData
        {
        public:
            inline explicit PageData(
                    StartSpecifier inStartSpecifier,
                    int inMeasureOrSystemIndex,
                    bool inNewPage = true,
                    std::optional<PageLayoutData> inPageLayoutData = std::nullopt
             )
             : startSpecifier{ inStartSpecifier }
             , measureOrSystemIndex{ inMeasureOrSystemIndex }
             , newPage{ inNewPage }
             , pageLayoutData{ inPageLayoutData }
            {

            }

            inline PageData()
            : PageData{ StartSpecifier::measureIndex, 0 }
            {

            }

            /// Indicate whether the `startIndex` is a measureIndex or a systemIndex.
            StartSpecifier startSpecifier;

            /// This is the index of either the measure or the system at which this `PageData` takes effect. Indicate
            /// whether this number is a measure index or a system index using the startSpecifier field.
            int measureOrSystemIndex;

            /// `true` will cause a page break at the specified measure index or system index.
            bool newPage;

            /// Optionally change the page layout settings starting with this page (and continuing thereafter).
            std::optional<PageLayoutData> pageLayoutData;
        };

        inline bool operator<( const PageData& lhs, const PageData& rhs )
        {
            if( lhs.startSpecifier == rhs.startSpecifier )
            {
                return lhs.measureOrSystemIndex < rhs.measureOrSystemIndex;
            }

            return static_cast<int>( lhs.startSpecifier ) < static_cast<int>( rhs.startSpecifier );
        }

        MXAPI_EQUALS_BEGIN( PageData )
            MXAPI_EQUALS_MEMBER( startSpecifier )
            MXAPI_EQUALS_MEMBER( measureOrSystemIndex )
            MXAPI_EQUALS_MEMBER( newPage )
            MXAPI_EQUALS_MEMBER( pageLayoutData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageData );
    }
}
