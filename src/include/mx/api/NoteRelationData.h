// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DurationData.h"
#include "mx/api/NoteData.h"

#include <optional>
#include <string>
#include <vector>

namespace mx
{
namespace api
{
// Whether a metronome-tied marks the start or the stop of a tie between two note figures in a
// metric-relationship metronome mark. As with ordinary ties, both ends should be present, each on
// its own note figure.
enum class MetronomeTieType
{
    start,
    stop
};

// Whether a metronome-tuplet marks the start or the stop of the tuplet bracket.
enum class MetronomeTupletType
{
    start,
    stop
};

// Which numbers a metronome-tuplet displays: the actual count, both the actual and normal counts,
// or neither. unspecified means the source omitted show-number and it is not written back.
enum class MetronomeShowNumber
{
    unspecified,
    actual,
    both,
    none
};

// A beam drawn on a note figure inside a metric-relationship metronome mark. value is the beam
// action (begin, continue = extend, end, or a forward/backward hook). number is the beam level,
// counting from 1 for the eighth-note beam upward; an absent number means MusicXML's default of
// level 1 and is written without the attribute.
struct MetronomeBeam
{
    Beam value;
    std::optional<int> number;

    MetronomeBeam() : value{Beam::unspecified}, number{}
    {
    }
};

// A tuplet drawn on a note figure inside a metric-relationship metronome mark: the
// time-modification ratio -- actualNotes in the time of normalNotes, where a normal note is a
// normalType note carrying normalDots dots -- plus how the bracket and numbers are shown.
// normalType unspecified means the ratio uses the figure's own note type.
struct MetronomeTuplet
{
    int actualNotes;
    int normalNotes;
    DurationName normalType;
    int normalDots;
    MetronomeTupletType type;
    Bool bracket;
    MetronomeShowNumber showNumber;

    MetronomeTuplet()
        : actualNotes{0}, normalNotes{0}, normalType{DurationName::unspecified}, normalDots{0},
          type{MetronomeTupletType::start}, bracket{Bool::unspecified}, showNumber{MetronomeShowNumber::unspecified}
    {
    }
};

// One note figure inside a metric-relationship metronome mark. metronomeType is the note value
// drawn (quarter, eighth, ...) and dots its augmentation dots; beams, tie, and tuplet decorate the
// figure the way beams, ties, and tuplets decorate a real note, but using the reduced
// metronome-specific vocabulary.
struct MetronomeNoteData
{
    DurationName metronomeType;
    int dots;
    std::vector<MetronomeBeam> beams;
    std::optional<MetronomeTieType> tie;
    std::optional<MetronomeTuplet> tuplet;

    MetronomeNoteData() : metronomeType{DurationName::unspecified}, dots{0}, beams{}, tie{}, tuplet{}
    {
    }
};

// The right-hand side of a metric relationship: the relation symbol drawn between the two note
// groups (MusicXML currently allows only "equals"; an empty symbol also means "equals") followed
// by the note figures to its right (at least one).
struct MetronomeRelation
{
    std::string symbol;
    std::vector<MetronomeNoteData> notes;

    MetronomeRelation() : symbol{}, notes{}
    {
    }
};

// A metric-relationship metronome mark -- the general form of a metric modulation, drawn with note
// pictures rather than the bare beat-units of the simpler MetricModulation form. A typical mark
// equates two note groups, e.g. "two beamed eighths = a quarter-note triplet". notes is the
// left-hand group (at least one figure). relation, when present, holds the relation symbol and the
// right-hand group; when absent the mark shows a single group with no equation. arrows draws
// metric-modulation arrows on both sides of the mark.
struct NoteRelation
{
    bool arrows;
    std::vector<MetronomeNoteData> notes;
    std::optional<MetronomeRelation> relation;

    NoteRelation() : arrows{false}, notes{}, relation{}
    {
    }
};

MXAPI_EQUALS_BEGIN(MetronomeBeam)
MXAPI_EQUALS_MEMBER(value)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetronomeBeam);

MXAPI_EQUALS_BEGIN(MetronomeTuplet)
MXAPI_EQUALS_MEMBER(actualNotes)
MXAPI_EQUALS_MEMBER(normalNotes)
MXAPI_EQUALS_MEMBER(normalType)
MXAPI_EQUALS_MEMBER(normalDots)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(showNumber)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetronomeTuplet);

MXAPI_EQUALS_BEGIN(MetronomeNoteData)
MXAPI_EQUALS_MEMBER(metronomeType)
MXAPI_EQUALS_MEMBER(dots)
MXAPI_EQUALS_MEMBER(beams)
MXAPI_EQUALS_MEMBER(tie)
MXAPI_EQUALS_MEMBER(tuplet)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetronomeNoteData);

MXAPI_EQUALS_BEGIN(MetronomeRelation)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(notes)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(MetronomeRelation);

MXAPI_EQUALS_BEGIN(NoteRelation)
MXAPI_EQUALS_MEMBER(arrows)
MXAPI_EQUALS_MEMBER(notes)
MXAPI_EQUALS_MEMBER(relation)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NoteRelation);
} // namespace api
} // namespace mx
