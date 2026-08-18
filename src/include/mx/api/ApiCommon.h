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

// MusicXML lets most elements carry an optional id attribute, a name that identifies that one
// element within the document. Software uses it to point at a particular note, measure, or
// marking -- to line playback up with the score, to hang an annotation on a note, or to link one
// file to another. Every mx::api type that models such an element has an `id` member. Leave it
// empty and no id attribute is written.
//
// An id must be unique within the document and must follow the XML name rules: a letter or an
// underscore first, then letters, digits, dots, hyphens, or underscores. mx repairs an id that
// breaks the name rules when it writes the file, but it does not check uniqueness, so an id you
// invent must not collide with one already in the score.

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

// Whether an item is associated with the system rather than only with the part it appears in --
// the system attribute of <measure-numbering> (MeasureData) and of <direction> and <harmony>
// (DirectionData). only... means the item is drawn on the top or bottom part of the system instead
// of this part; also... means it is drawn on both. unspecified means the attribute is absent.
// onlyBottom and alsoBottom are measure-numbering only; MusicXML has no bottom-of-system direction.
enum class SystemRelation
{
    unspecified,
    none,
    onlyTop,
    onlyBottom,
    alsoTop,
    alsoBottom
};

// The shape drawn around a piece of text or a symbol -- MusicXML's enclosure attribute, carried by
// RehearsalData, WordsData, SymbolData, PercussionData and PageTextData. unspecified means the
// attribute is absent, which draws no enclosure; none states explicitly that there is none. A
// bracket is a rectangle with the bottom line missing, as is common in jazz notation, and an
// invertedBracket is one with the top line missing.
enum class Enclosure
{
    unspecified,
    rectangle,
    square,
    oval,
    circle,
    bracket,
    invertedBracket,
    triangle,
    diamond,
    pentagon,
    hexagon,
    heptagon,
    octagon,
    nonagon,
    decagon,
    none
};
} // namespace api
} // namespace mx

// this defines inline equality comparison macros and functions
// which are mainly used for test code to assert equality
#include "mx/api/ApiEquality.h"
