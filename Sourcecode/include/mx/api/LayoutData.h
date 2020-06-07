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

            PageLayoutData pageLayout;

            long double systemLeftMargin;      // negative number represent the absence of a value
            long double systemRightMargin;     // negative number represent the absence of a value
            long double systemDistance;        // distance from bollom line of one system to top line of next system, negative number represent the absence of a value
            long double topSystemDistance;     // first system distance from top margin to top line of first staff, negative number represent the absence of a value
            long double staffDistance;         // the space between staves within the same system, negative number represent the absence of a value

            std::vector<AppearanceData> appearance;
            MeasureNumbering measureNumbering; // measure numbering setting at the global level, will be stated in first measure's <print> tag.  can be overridden by a value in the Measure
            
            LayoutData()
            : scalingMillimeters( -1.0 )
            , scalingTenths( -1.0 )
            , pageLayout{}
            , systemLeftMargin( -1.0 )
            , systemRightMargin( -1.0 )
            , systemDistance( -1.0 )
            , topSystemDistance( -1.0 )
            , staffDistance( -1.0 )
            , appearance()
            , measureNumbering{ MeasureNumbering::unspecified }
            {

            }


        };
        
        MXAPI_EQUALS_BEGIN( LayoutData )
        MXAPI_EQUALS_MEMBER( pageLayout )
        MXAPI_EQUALS_MEMBER( systemLeftMargin )
        MXAPI_EQUALS_MEMBER( systemRightMargin )
        MXAPI_EQUALS_MEMBER( systemDistance )
        MXAPI_EQUALS_MEMBER( topSystemDistance )
        MXAPI_EQUALS_MEMBER( staffDistance )
        MXAPI_EQUALS_MEMBER( appearance )
        MXAPI_EQUALS_MEMBER( measureNumbering )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( LayoutData );
    }
}
