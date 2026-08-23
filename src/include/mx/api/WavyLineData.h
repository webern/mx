// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/PositionData.h"
#include "mx/api/SpannerNumber.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// Which of the two notes flanking a trill or vibrato the wavy line's playback starts on.
enum class WavyLineStartNote
{
    unspecified,
    upper,
    main,
    below
};

// The written pitch step of the auxiliary (upper) note the trill alternates with.
enum class WavyLineTrillStep
{
    unspecified,
    whole,
    half,
    unison
};

// The interval of a trill's closing turn, or that it has none.
enum class WavyLineTwoNoteTurn
{
    unspecified,
    whole,
    half,
    none
};

// A wavy line indicates a trill extension or a vibrato line. Unlike glissando and slide it can
// have a continue point (for a line that crosses a system or page break), so it is a
// start/continue/stop spanner rather than a start/stop one. It lives inside a note's <ornaments>,
// not directly under <notations>.
struct WavyLineStart
{
    SpannerNumber number;
    PositionData positionData; // placement rides in positionData.placement
    bool isColorSpecified;
    ColorData colorData;

    // A specific SMuFL glyph from the Multi-segment lines range (e.g. "wiggleTrill"), overriding
    // the default trill/vibrato glyph.
    std::optional<std::string> smufl;

    // Playback (MusicXML's trill-sound attributes).
    WavyLineStartNote startNote;
    WavyLineTrillStep trillStep;
    WavyLineTwoNoteTurn twoNoteTurn;
    Bool accelerate;
    std::optional<double> beats;
    std::optional<double> secondBeat;
    std::optional<double> lastBeat;

    WavyLineStart()
        : number{}, positionData{}, isColorSpecified{false}, colorData{}, smufl{std::nullopt},
          startNote{WavyLineStartNote::unspecified}, trillStep{WavyLineTrillStep::unspecified},
          twoNoteTurn{WavyLineTwoNoteTurn::unspecified}, accelerate{Bool::unspecified}, beats{std::nullopt},
          secondBeat{std::nullopt}, lastBeat{std::nullopt}
    {
    }
};

struct WavyLineContinue
{
    SpannerNumber number;
    PositionData positionData;
    bool isColorSpecified;
    ColorData colorData;

    WavyLineContinue() : number{}, positionData{}, isColorSpecified{false}, colorData{}
    {
    }
};

struct WavyLineStop
{
    SpannerNumber number;
    PositionData positionData;
    bool isColorSpecified;
    ColorData colorData;

    WavyLineStop() : number{}, positionData{}, isColorSpecified{false}, colorData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(WavyLineStart)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(startNote)
MXAPI_EQUALS_MEMBER(trillStep)
MXAPI_EQUALS_MEMBER(twoNoteTurn)
MXAPI_EQUALS_MEMBER(accelerate)
MXAPI_EQUALS_MEMBER(beats)
MXAPI_EQUALS_MEMBER(secondBeat)
MXAPI_EQUALS_MEMBER(lastBeat)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WavyLineStart);

MXAPI_EQUALS_BEGIN(WavyLineContinue)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WavyLineContinue);

MXAPI_EQUALS_BEGIN(WavyLineStop)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(isColorSpecified)
MXAPI_EQUALS_MEMBER(colorData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(WavyLineStop);
} // namespace api
} // namespace mx
