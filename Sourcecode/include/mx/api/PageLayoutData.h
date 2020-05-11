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
    namespace api
    {

        class PageLayoutData
        {
        public:
            long double pageWidth;   // a negative number represents the absence of a value
            long double pageHeight;  // a negative number represents the absence of a value

            PageMarginsData pageMargins;

            PageLayoutData()
                :   pageWidth( -1.0 )
                  , pageHeight( -1.0 )
                  , pageMargins{}
            {

            }

            inline bool isUsed() const
            {
                return pageWidth >= 0.0 ||
                    pageHeight >= 0.0 ||
                    pageMargins.isUsed();
            }
        };

        MXAPI_EQUALS_BEGIN( PageLayoutData )
            MXAPI_EQUALS_MEMBER( pageWidth )
            MXAPI_EQUALS_MEMBER( pageHeight )
            MXAPI_EQUALS_MEMBER( pageMargins )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PageLayoutData );
    }
}
