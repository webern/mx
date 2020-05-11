// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PageLayoutData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {

        /// PageData allows the specification of a new page, or the prevention of a new page, along with the ability
        /// to change the page layout settings. `firstSystemIndex` is the index of the first system (in score.systems)
        /// on the page.
        class PageData
        {
        public:

            /// This is the index of the first system, in ScoreData.systems, that is affected by the PageData.
            /// For example, if `firstSystemIndex` is 10, and newPage is `true`, then system index 10 will be the first
            /// system on a new page.
            int firstSystemIndex;

            /// `true` will cause a page break before firstSystem. `false` will tell the application that a page break
            /// should be avoided here.
            bool newPage;

            // TODO - make this optional with `std::optional` if we go to >= C++17, https://github.com/webern/mx/pull/93
            PageLayoutData pageLayoutChange;
        };

        inline bool operator<( const PageData& lhs, const PageData& rhs ) { return lhs.firstSystemIndex < rhs.firstSystemIndex; }

        MXAPI_EQUALS_BEGIN( PageData )
            MXAPI_EQUALS_MEMBER( firstSystemIndex )
            MXAPI_EQUALS_MEMBER( newPage )
            MXAPI_EQUALS_MEMBER( pageLayoutChange )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageData );
    }
}
