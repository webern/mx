// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {

        /// Page margins are specified separately for odd and even pages. If odd and even pages have the same values,
        /// then then a type of "both" will be used in the produced MusicXML. All values are optional. A negative value
        /// indicates the absence of value. Distance values are in 'tenths' governed by the 'scaling' values
        class PageMarginsData
        {
        public:
            long double oddPageLeftMargin;     // a negative number represents the absence of a value
            long double oddPageRightMargin;    // a negative number represents the absence of a value
            long double oddPageTopMargin;      // a negative number represents the absence of a value
            long double oddPageBottomMargin;   // a negative number represents the absence of a value
            long double evenPageLeftMargin;    // a negative number represents the absence of a value
            long double evenPageRightMargin;   // a negative number represents the absence of a value
            long double evenPageTopMargin;     // a negative number represents the absence of a value
            long double evenPageBottomMargin;  // a negative number represents the absence of a value

            PageMarginsData()
                : oddPageLeftMargin{ -1.0 }
                , oddPageRightMargin{ -1.0 }
                , oddPageTopMargin{ -1.0 }
                , oddPageBottomMargin{ -1.0 }
                , evenPageLeftMargin{ -1.0 }
                , evenPageRightMargin{ -1.0 }
                , evenPageTopMargin{ -1.0 }
                , evenPageBottomMargin{ -1.0 }
            {

            }

            /// Returns true of any of the values are greater or equal to zero (indicating the presence of a value).
            inline bool isUsed() const
            {
                return oddPageLeftMargin >= 0.0 ||
                    oddPageRightMargin >= 0.0 ||
                    oddPageTopMargin >= 0.0 ||
                    oddPageBottomMargin >= 0.0 ||
                    evenPageLeftMargin >= 0.0 ||
                    evenPageRightMargin >= 0.0 ||
                    evenPageTopMargin >= 0.0 ||
                    evenPageBottomMargin >= 0.0;
            }

            inline bool areOddMarginsValid() const
            {
                return oddPageLeftMargin > 0
                       && oddPageRightMargin > 0
                       && oddPageTopMargin > 0
                       && oddPageBottomMargin > 0;
            }

            inline bool areOddMarginsSpecified() const
            {
                return oddPageLeftMargin > 0
                       || oddPageRightMargin > 0
                       || oddPageTopMargin > 0
                       || oddPageBottomMargin > 0;
            }

            inline bool areEvenMarginsValid() const
            {
                return    evenPageLeftMargin > 0
                       && evenPageRightMargin > 0
                       && evenPageTopMargin > 0
                       && evenPageBottomMargin > 0;
            }

            inline bool areEvenMarginsSpecified() const
            {
                return    evenPageLeftMargin > 0
                       || evenPageRightMargin > 0
                       || evenPageTopMargin > 0
                       || evenPageBottomMargin > 0;
            }

            inline bool areMarginsSpecified() const
            {
                return areOddMarginsSpecified() || areEvenMarginsSpecified();
            }


            inline bool areOddEvenMarginsTheSame() const
            {
                return    areSame(oddPageLeftMargin, evenPageLeftMargin)
                       && areSame(oddPageRightMargin, evenPageRightMargin)
                       && areSame(oddPageTopMargin, evenPageTopMargin)
                       && areSame(oddPageBottomMargin, evenPageBottomMargin);
            }

        };

        MXAPI_EQUALS_BEGIN( PageMarginsData )
            MXAPI_EQUALS_MEMBER( oddPageLeftMargin )
            MXAPI_EQUALS_MEMBER( oddPageRightMargin )
            MXAPI_EQUALS_MEMBER( oddPageTopMargin )
            MXAPI_EQUALS_MEMBER( oddPageBottomMargin )
            MXAPI_EQUALS_MEMBER( evenPageLeftMargin )
            MXAPI_EQUALS_MEMBER( evenPageRightMargin )
            MXAPI_EQUALS_MEMBER( evenPageTopMargin )
            MXAPI_EQUALS_MEMBER( evenPageBottomMargin )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageMarginsData );
    }
}
