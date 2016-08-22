// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        // all distance values are in 'tenths'

        enum class Placement
        {
            unspecified,
            above,
            below
        };

        enum class VerticalAlignment
        {
            unspecified,
            top,
            middle,
            bottom,
            baseline
        };

        enum class HorizontalAlignment
        {
            unspecified,
            left,
            center,
            right
        };

        class PositionData
        {
        public:

            // decimal values are in 'tenths'

            long double defaultX;
            bool hasDefaultX;
            long double defaultY;
            bool hasDefaultY;
            long double relativeX;
            bool hasRelativeX;
            long double relativeY;
            bool hasRelativeY;
            Placement placement;
            VerticalAlignment verticalAlignment;
            HorizontalAlignment horizontalAlignmnet;

            PositionData()
            : defaultX( 0.0 )
            , hasDefaultX( false )
            , defaultY( 0.0 )
            , hasDefaultY( false )
            , relativeX( 0.0 )
            , hasRelativeX( false )
            , relativeY( 0.0 )
            , hasRelativeY( false )
            , placement( Placement::unspecified )
            , verticalAlignment( VerticalAlignment::unspecified )
            , horizontalAlignmnet( HorizontalAlignment::unspecified )
            {

            }
        };
    }
}
