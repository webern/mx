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
using Double = double;
using OptionalDouble = std::optional<Double>;
constexpr const Double MX_API_EQUALITY_EPSILON = 0.00000001;

inline bool areSame(Double left, Double right)
{
    return (std::abs(left - right) < MX_API_EQUALITY_EPSILON);
}

constexpr int DEFAULT_TICKS_PER_QUARTER = 3 * 4 * 5 * 7;
constexpr int TICK_TIME_INFINITY = std::numeric_limits<int>::max();

// Named sentinels for legacy fields that predate std::optional, where -1 (or -1.0) means
// "unspecified" or "absent". Do not use these for new fields; new absent-able fields use
// std::optional instead (see "mx::api conventions" in AGENTS.md and issue #249).
constexpr int INDEX_UNSPECIFIED = -1;        // staff/part indices and numbers, e.g. KeyData::staffIndex
constexpr int NUMBER_LEVEL_UNSPECIFIED = -1; // MusicXML 'number' attributes, e.g. TupletStart::numberLevel
constexpr int VALUE_UNSPECIFIED = -1;        // other absent-able ints, e.g. DirectionData::voice
constexpr Double DOUBLE_UNSPECIFIED = -1.0;  // absent-able doubles, e.g. StaffData::staffSize

// Intentional ternary: absent-able bools use Bool::unspecified, not std::optional<bool>.
// See "mx::api conventions" in AGENTS.md.
enum class Bool
{
    unspecified,
    yes,
    no
};

inline bool toBool(Bool b)
{
    return b == Bool::yes;
}

inline Bool fromBool(bool b)
{
    return b ? Bool::yes : Bool::no;
}

inline bool isSpecified(Bool b)
{
    return b != Bool::unspecified;
}

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
} // namespace api
} // namespace mx

// this defines inline equality comparison macros and functions
// which are mainly used for test code to assert equality
#include "mx/api/ApiEquality.h"
