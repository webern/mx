// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <cmath>
#include <limits>

namespace mx
{
    namespace api
    {
        constexpr const long double MX_API_EQUALITY_EPSILON = 0.00000001;

        inline bool areSame( long double left, long double right )
        {
            return ( std::abs( left - right ) < MX_API_EQUALITY_EPSILON );
        }

        constexpr int DEFAULT_TICKS_PER_QUARTER = 3 * 4 * 5 * 7;
        constexpr int TICK_TIME_INFINITY = std::numeric_limits<int>::max();

        enum class Bool
        {
            unspecified,
            yes,
            no
        };
        
        enum class Placement
        {
            unspecified,
            above,
            below
        };
        
        enum class MeasureNumbering
        {
            unspecified,
            none,
            measure,
            system
        };
    }
}

// this defines inline equality comparison macros and functions
// which are mainly used for test code to assert equality
#include "mx/api/ApiEquality.h"
