// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/Id.h"
#include "mx/api/PositionData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{

enum class BarlineType
{
    unspecified,
    unsupported,
    none,
    normal,
    lightLight,
    lightHeavy,
    heavyLight,
    dotted,
    dashed,
    heavy,
    heavyHeavy,
    tick,
    short_
};

// Where a barline sits in an ending (volta) bracket. start opens the bracket on the left barline
// of the ending's first measure; stop closes it with a downward jog, as a first ending usually
// does; discontinue closes it without a jog, as a final ending usually does.
enum class EndingType
{
    start,
    stop,
    discontinue
};

// An ending (volta) bracket: the numbered bracket drawn over the measures played on a particular
// pass through a repeat.
class EndingData
{
  public:
    EndingType type;

    // The passes on which this ending is played, in the order they are written: an ending marked
    // "1, 2, 3" is played on passes 1, 2, and 3. Numbering starts at 1; a value below 1 is written
    // as 1. Leave this empty for a blank ending -- the way MusicXML says that a bracket is here
    // but which passes it covers is unknown.
    std::vector<int> numbers;

    // The label drawn under the bracket. Leave it empty to have the numbers themselves displayed;
    // set it only when the label differs from them, as when passes 1, 2, and 3 are labeled "1-3".
    std::string text;

    EndingData() : type{EndingType::start}, numbers{}, text{}
    {
    }
};

MXAPI_EQUALS_BEGIN(EndingData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(numbers)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(EndingData);

// Whether a repeat mark faces forward (the start of a repeated section, drawn |:) or backward
// (the end, drawn :|).
enum class RepeatDirection
{
    unspecified,
    forward,
    backward
};

// The winged extensions that can be drawn above and below a repeat barline. `none` is a real
// value (no wings, the MusicXML default); `unspecified` means the attribute is absent.
enum class RepeatWinged
{
    unspecified,
    none,
    straight,
    curved,
    doubleStraight,
    doubleCurved
};

class BarlineData
{
  public:
    int tickTimePosition;
    BarlineType barlineType;
    // The ending (volta) bracket this barline starts or ends, if any.
    std::optional<EndingData> ending;
    bool repeat;
    // Number of times a backward repeat is played (the repeat's `times` attribute). 0 = not
    // specified.
    int repeatTimes;
    // Whether the repeat faces forward (start of a repeated section) or backward (end). Leave
    // unspecified to let mx infer it from the barline's position -- a repeat on a left or
    // downbeat barline is forward, otherwise backward -- or set it to control the direction.
    RepeatDirection repeatDirection;
    // The repeat's `after-jump` attribute: when yes, the repeat is only taken after a da capo or
    // dal segno jump reaches it, not on the first pass. Leave unspecified to omit the attribute.
    Bool repeatAfterJump;
    // The winged extensions drawn above and below the repeat barline. RepeatWinged::none is an
    // explicit `winged="none"`; leave unspecified to omit the attribute.
    RepeatWinged repeatWinged;
    HorizontalAlignment location;

    // The <barline> element's id attribute (see Id.h).
    std::optional<Id> id;

    BarlineData()
        : tickTimePosition{0}, barlineType{BarlineType::normal}, ending{}, repeat{false}, repeatTimes{0},
          repeatDirection{RepeatDirection::unspecified}, repeatAfterJump{Bool::unspecified},
          repeatWinged{RepeatWinged::unspecified}, location{HorizontalAlignment::unspecified}, id{}
    {
    }
};

MXAPI_EQUALS_BEGIN(BarlineData)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(barlineType)
MXAPI_EQUALS_MEMBER(ending)
MXAPI_EQUALS_MEMBER(repeat)
MXAPI_EQUALS_MEMBER(repeatTimes)
MXAPI_EQUALS_MEMBER(repeatDirection)
MXAPI_EQUALS_MEMBER(repeatAfterJump)
MXAPI_EQUALS_MEMBER(repeatWinged)
MXAPI_EQUALS_MEMBER(location)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BarlineData);
} // namespace api
} // namespace mx
