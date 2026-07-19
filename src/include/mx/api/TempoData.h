// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/DurationData.h"
#include "mx/api/FontData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>

namespace mx
{
namespace api
{
// Which kind of metronome mark a TempoData carries.
enum class TempoType
{
    unspecified,
    beatsPerMinute,
    metricModulation
};

// A metronome mark of the form "note = number", e.g. a quarter note at 120. durationName and
// dots give the beat-unit -- the note picture to the left of the equals sign. beatsPerMinute is
// the value to the right of it. MusicXML allows free text there ("120", "ca. 76", a range like
// "126-138"), so it is a string; an empty string is a valid "unset" value. A player that needs a
// numeric playback tempo should read SoundData::tempo, which is always numeric and expressed in
// quarter notes per minute.
struct BeatsPerMinute
{
    DurationName durationName;
    int dots;
    std::string beatsPerMinute;

    BeatsPerMinute() : durationName{DurationName::unspecified}, dots{VALUE_UNSPECIFIED}, beatsPerMinute{}
    {
    }
};

// A metronome mark of the form "note = note", e.g. a dotted quarter equal to a half note -- the
// two-beat-unit spelling of a metric modulation. The left and right beat-units are each a note
// picture (durationName + dots). playbackBeatsPerMinute is unused for this form.
struct MetricModulation
{
    DurationName leftDurationName;
    int leftDots;
    DurationName rightDurationName;
    int rightDots;
    BeatsPerMinute playbackBeatsPerMinute;

    MetricModulation()
        : leftDurationName{DurationName::unspecified}, leftDots{VALUE_UNSPECIFIED},
          rightDurationName{DurationName::unspecified}, rightDots{VALUE_UNSPECIFIED}, playbackBeatsPerMinute{}
    {
    }
};

// A metronome (tempo) mark carried by a <direction>. tempoType selects the form: a
// note-equals-number mark (beatsPerMinute) or a note-equals-note metric modulation
// (metricModulation). positionData, fontData, color, and id give the mark's placement and
// appearance; justify aligns the mark's content within its box; printObject set to 'no' keeps the
// mark in the file without drawing it; isParenthetical draws the mark in parentheses.
class TempoData
{
  public:
    int tickTime;
    Bool isParenthetical;
    PrintData printData;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<std::string> id;
    HorizontalAlignment justify;
    Bool printObject;
    TempoType tempoType;

    // only used when tempoType is 'beatsPerMinute'
    BeatsPerMinute beatsPerMinute;

    // only used when tempoType is 'metricModulation'
    MetricModulation metricModulation;

    TempoData()
        : tickTime{0}, isParenthetical{Bool::unspecified}, printData{}, positionData{}, fontData{}, color{}, id{},
          justify{HorizontalAlignment::unspecified}, printObject{Bool::unspecified}, tempoType{TempoType::unspecified},
          beatsPerMinute{}, metricModulation{}
    {
    }
};

MXAPI_EQUALS_BEGIN(BeatsPerMinute)
MXAPI_EQUALS_MEMBER(durationName)
MXAPI_EQUALS_MEMBER(dots)
MXAPI_EQUALS_MEMBER(beatsPerMinute)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BeatsPerMinute);

MXAPI_EQUALS_BEGIN(MetricModulation)
MXAPI_EQUALS_MEMBER(leftDurationName)
MXAPI_EQUALS_MEMBER(leftDots)
MXAPI_EQUALS_MEMBER(rightDurationName)
MXAPI_EQUALS_MEMBER(rightDots)
MXAPI_EQUALS_MEMBER(playbackBeatsPerMinute)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetricModulation);

MXAPI_EQUALS_BEGIN(TempoData)
MXAPI_EQUALS_MEMBER(tickTime)
MXAPI_EQUALS_MEMBER(isParenthetical)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(fontData)
MXAPI_EQUALS_MEMBER(color)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_MEMBER(justify)
MXAPI_EQUALS_MEMBER(printObject)
MXAPI_EQUALS_MEMBER(tempoType)
MXAPI_EQUALS_MEMBER(beatsPerMinute)
MXAPI_EQUALS_MEMBER(metricModulation)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TempoData);
} // namespace api
} // namespace mx
