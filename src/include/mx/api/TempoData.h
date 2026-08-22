// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
#include "mx/api/ColorData.h"
#include "mx/api/DurationData.h"
#include "mx/api/FontData.h"
#include "mx/api/Id.h"
#include "mx/api/NoteRelationData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace mx
{
namespace api
{
// A single note picture in a beat-unit metronome mark: a note value (type) with augmentation
// dots. Used for the tied continuations of a beat-unit, e.g. the eighth in "quarter + eighth".
struct BeatUnit
{
    DurationName type;
    int dots;

    BeatUnit() : type{DurationName::unspecified}, dots{0}
    {
    }
};

// A metronome mark of the form "note = number", e.g. a quarter note at 120. durationName and dots
// give the beat-unit -- the note picture to the left of the equals sign. tiedBeatUnits holds any
// further note pictures tied to it (a mark like "quarter + eighth = 120" ties an eighth to the
// quarter). beatsPerMinute is the value to the right of the equals sign; MusicXML allows free text
// there ("120", "ca. 76", a range like "126-138"), so it is a string, and an empty string is a
// valid "unset" value. A player that needs a numeric playback tempo should read SoundData::tempo,
// which is always numeric and expressed in quarter notes per minute.
struct BeatsPerMinute
{
    DurationName durationName;
    int dots;
    std::vector<BeatUnit> tiedBeatUnits;
    std::string beatsPerMinute;

    BeatsPerMinute()
        : durationName{DurationName::unspecified}, dots{VALUE_UNSPECIFIED}, tiedBeatUnits{}, beatsPerMinute{}
    {
    }
};

// A metronome mark of the form "note = note", e.g. a dotted quarter equal to a half note -- the
// two-beat-unit spelling of a metric modulation. The left and right beat-units are each a note
// picture (durationName + dots), and each may carry tied continuations. playbackBeatsPerMinute is
// unused for this form.
struct MetricModulation
{
    DurationName leftDurationName;
    int leftDots;
    std::vector<BeatUnit> leftTiedBeatUnits;
    DurationName rightDurationName;
    int rightDots;
    std::vector<BeatUnit> rightTiedBeatUnits;
    BeatsPerMinute playbackBeatsPerMinute;

    MetricModulation()
        : leftDurationName{DurationName::unspecified}, leftDots{VALUE_UNSPECIFIED}, leftTiedBeatUnits{},
          rightDurationName{DurationName::unspecified}, rightDots{VALUE_UNSPECIFIED}, rightTiedBeatUnits{},
          playbackBeatsPerMinute{}
    {
    }
};

// The body of a metronome mark: exactly one of the three forms a <metronome> can take. Construct
// it from whichever form you mean; query kind() (or the is...() helpers) and read the matching
// accessor. Reading the wrong accessor returns a default-constructed value rather than throwing.
class TempoChoice
{
  public:
    enum class Kind
    {
        beatsPerMinute,
        metricModulation,
        noteRelation
    };

    // Defaults to an (empty) beats-per-minute mark, the first alternative.
    TempoChoice();

    TempoChoice(BeatsPerMinute value);
    TempoChoice(MetricModulation value);
    TempoChoice(NoteRelation value);

    Kind kind() const;
    bool isBeatsPerMinute() const;
    bool isMetricModulation() const;
    bool isNoteRelation() const;

    // Each accessor returns a copy of the held alternative. Check the matching is...() first; a
    // wrong-kind access returns a default-constructed value.
    BeatsPerMinute beatsPerMinute() const;
    MetricModulation metricModulation() const;
    NoteRelation noteRelation() const;

    bool operator==(const TempoChoice &other) const;

  private:
    std::variant<BeatsPerMinute, MetricModulation, NoteRelation> myValue;
};

// A metronome (tempo) mark carried by a <direction>. choice holds the mark's body -- a
// note-equals-number mark, a note-equals-note metric modulation, or a note-relationship mark
// drawn with note figures. positionData, fontData, color, and id give the mark's placement and
// appearance; justify aligns the mark's content within its box; printObject set to 'no' keeps the
// mark in the file without drawing it (common for relationship marks); isParenthetical draws the
// mark in parentheses.
class TempoData
{
  public:
    int tickTime;
    Bool isParenthetical;
    PrintData printData;
    PositionData positionData;
    FontData fontData;
    std::optional<ColorData> color;
    std::optional<Id> id;
    HorizontalAlignment justify;
    Bool printObject;
    TempoChoice choice;

    TempoData()
        : tickTime{0}, isParenthetical{Bool::unspecified}, printData{}, positionData{}, fontData{}, color{}, id{},
          justify{HorizontalAlignment::unspecified}, printObject{Bool::unspecified}, choice{}
    {
    }
};

MXAPI_EQUALS_BEGIN(BeatUnit)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(dots)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BeatUnit);

MXAPI_EQUALS_BEGIN(BeatsPerMinute)
MXAPI_EQUALS_MEMBER(durationName)
MXAPI_EQUALS_MEMBER(dots)
MXAPI_EQUALS_MEMBER(tiedBeatUnits)
MXAPI_EQUALS_MEMBER(beatsPerMinute)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(BeatsPerMinute);

MXAPI_EQUALS_BEGIN(MetricModulation)
MXAPI_EQUALS_MEMBER(leftDurationName)
MXAPI_EQUALS_MEMBER(leftDots)
MXAPI_EQUALS_MEMBER(leftTiedBeatUnits)
MXAPI_EQUALS_MEMBER(rightDurationName)
MXAPI_EQUALS_MEMBER(rightDots)
MXAPI_EQUALS_MEMBER(rightTiedBeatUnits)
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
MXAPI_EQUALS_MEMBER(choice)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TempoData);
} // namespace api
} // namespace mx
