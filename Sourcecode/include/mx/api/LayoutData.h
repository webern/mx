// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageLayoutData.h"
#include "mx/api/SystemLayoutData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        
        // distance values are in 'tenths' governed by the 'scaling' values

        class LayoutData
        {
        public:

            long double scalingMillimeters;
            long double scalingTenths;

            inline long double tenthsPerMillimeter() const
            {
                return scalingTenths / scalingMillimeters;
            }

            inline long double tenthsPerInch() const
            {
                return tenthsPerMillimeter() * 25.4;
            }

            /// Page layout. Note all members of PageLayoutData are optional.
            PageLayoutData pageLayout;

            /// System layout. Note all members of SystemLayoutData are optional.
            SystemLayoutData systemLayout;

            std::vector<AppearanceData> appearance;
            MeasureNumbering measureNumbering; // measure numbering setting at the global level, will be stated in first measure's <print> tag.  can be overridden by a value in the Measure
            
            LayoutData()
            : scalingMillimeters{ -1.0 }
            , scalingTenths{ -1.0 }
            , pageLayout{}
            , systemLayout{}
            , appearance{}
            , measureNumbering{ MeasureNumbering::unspecified }
            {

            }
        };
        
        MXAPI_EQUALS_BEGIN( LayoutData )
        MXAPI_EQUALS_MEMBER( pageLayout )
        MXAPI_EQUALS_MEMBER( systemLayout )
        MXAPI_EQUALS_MEMBER( appearance )
        MXAPI_EQUALS_MEMBER( measureNumbering )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LayoutData );
    }
}
