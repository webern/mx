// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cmath>
#include <limits>
#include <optional>

namespace mx
{
    namespace api
    {
        using Double = long double;
        using OptionalDouble = std::optional<Double>;
        constexpr const Double MX_API_EQUALITY_EPSILON = 0.00000001;

        inline bool areSame( Double left, Double right )
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

        inline bool toBool( Bool b ) { return b == Bool::yes; }
        inline Bool fromBool( bool b ) { return b ? Bool::yes : Bool::no; }
        inline bool isSpecified( Bool b ) { return b != Bool::unspecified; }

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
