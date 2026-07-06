// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cmath>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>

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

// A spanner's number identity, shared by curves (slur/tied), wedges, and the
// generic spanner start/stop structs. Exactly one of three states; a
// contradictory state (both an explicit level and an identity) is
// unrepresentable by construction.
//
//   - unspecified   : no number information. The writer applies its default
//                     (omit the attribute for a lone spanner).
//   - explicit(n)   : a literal MusicXML number, 1..16. Emitted verbatim. Set by
//                     the reader to preserve an imported number, or by a caller
//                     who wants to control the number themselves.
//   - identity(id)  : an author-only, NEVER-serialized label. Start/continue/stop
//                     that share the same id are the same logical spanner; the
//                     writer assigns the MusicXML number from serialization
//                     order. The reader never produces this state.
//
// Misuse (an out-of-range level, an empty id, or calling an accessor for the
// wrong state) is a programming error and throws std::logic_error. Consumers
// should branch on kind() before touching a payload accessor.
class SpannerNumber
{
  public:
    enum class Kind
    {
        unspecified,
        explicitLevel,
        identity
    };

    // Default is unspecified.
    SpannerNumber() = default;

    // Factory: an explicit MusicXML number. `level` must be in [1, 16]; passing a
    // value outside that range is a programming error.
    static SpannerNumber makeLevel(int level)
    {
        if (level < 1 || level > 16)
        {
            throw std::logic_error{"mx::api::SpannerNumber::makeLevel: level must be in the range [1, 16]"};
        }
        SpannerNumber result;
        result.myKind = Kind::explicitLevel;
        result.myLevel = level;
        return result;
    }

    // Factory: an author identity label. `id` must be non-empty; an empty string is
    // a programming error (use the default-constructed unspecified state instead).
    // Because unspecified is its own first-class state, empty-string-as-sentinel
    // does not exist here.
    static SpannerNumber makeIdentity(std::string id)
    {
        if (id.empty())
        {
            throw std::logic_error{"mx::api::SpannerNumber::makeIdentity: id must be non-empty"};
        }
        SpannerNumber result;
        result.myKind = Kind::identity;
        result.myIdentity = std::move(id);
        return result;
    }

    inline Kind kind() const
    {
        return myKind;
    }

    inline bool isUnspecified() const
    {
        return myKind == Kind::unspecified;
    }

    inline bool isExplicit() const
    {
        return myKind == Kind::explicitLevel;
    }

    inline bool isIdentity() const
    {
        return myKind == Kind::identity;
    }

    // Precondition: isExplicit(). Returns the literal number 1..16.
    inline int level() const
    {
        if (myKind != Kind::explicitLevel)
        {
            throw std::logic_error{"mx::api::SpannerNumber::level: not an explicit level"};
        }
        return myLevel;
    }

    // Precondition: isIdentity(). Returns the (non-empty) id.
    inline const std::string &identity() const
    {
        if (myKind != Kind::identity)
        {
            throw std::logic_error{"mx::api::SpannerNumber::identity: not an identity"};
        }
        return myIdentity;
    }

    inline bool operator==(const SpannerNumber &other) const
    {
        if (myKind != other.myKind)
        {
            return false;
        }
        switch (myKind)
        {
        case Kind::explicitLevel:
            return myLevel == other.myLevel;
        case Kind::identity:
            return myIdentity == other.myIdentity;
        case Kind::unspecified:
        default:
            return true;
        }
    }

    inline bool operator!=(const SpannerNumber &other) const
    {
        return !(*this == other);
    }

  private:
    Kind myKind = Kind::unspecified;
    int myLevel = 0;
    std::string myIdentity;
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
} // namespace api
} // namespace mx

// this defines inline equality comparison macros and functions
// which are mainly used for test code to assert equality
#include "mx/api/ApiEquality.h"
