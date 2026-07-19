// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

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

enum class EndingType
{
    none,
    start,
    stop,
    discontinue
};

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
    EndingType endingType;
    int endingNumber;
    bool repeat;
    // Number of times a backward repeat is played (the repeat's `times` attribute). 0 = not
    // specified, mirroring endingNumber above.
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

    BarlineData()
        : tickTimePosition{0}, barlineType{BarlineType::normal}, endingType{EndingType::none}, endingNumber{0},
          repeat{false}, repeatTimes{0}, repeatDirection{RepeatDirection::unspecified},
          repeatAfterJump{Bool::unspecified}, repeatWinged{RepeatWinged::unspecified},
          location{HorizontalAlignment::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(BarlineData)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(barlineType)
MXAPI_EQUALS_MEMBER(endingType)
MXAPI_EQUALS_MEMBER(endingNumber)
MXAPI_EQUALS_MEMBER(repeat)
MXAPI_EQUALS_MEMBER(repeatTimes)
MXAPI_EQUALS_MEMBER(repeatDirection)
MXAPI_EQUALS_MEMBER(repeatAfterJump)
MXAPI_EQUALS_MEMBER(repeatWinged)
MXAPI_EQUALS_MEMBER(location)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BarlineData);
} // namespace api
} // namespace mx
