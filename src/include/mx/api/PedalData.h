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
// The kind of piano-pedal event, MusicXML's pedal/@type. A damper or sostenuto pedal line is
// written as a run of these events on one staff, ordered by tickTimePosition. The three
// "opening" kinds begin a line, the two "closing" kinds end one, and change/continueLine occur
// mid-line:
//
//   start        Begin a damper-pedal line with the usual downstroke.
//   sostenuto    Begin a sostenuto-pedal line (the middle piano pedal).
//   resume       Begin a line with no downstroke -- used to pick a line back up after a
//                discontinue, or to start one that a text or symbol already introduced.
//   change       A pedal lift and immediate retake, drawn as an inverted "V" in the line.
//   continueLine A formatting-only continuation point, used to break the line neatly across a
//                system break or to shape a complex pedaling line. It changes nothing musically.
//   stop         End the line with an explicit lift (the upstroke at the end of the line).
//   discontinue  End the line with no explicit lift -- the counterpart to resume.
//
// mx::api models only pedal lines here (MusicXML pedal/@line="yes"). The alternative sign form
// (Ped. and * marks, pedal/@line="no") is expressed through MarkData as MarkType::pedal and
// MarkType::damp instead.
enum class PedalKind
{
    unspecified,
    start,
    stop,
    sostenuto,
    change,
    continueLine,
    discontinue,
    resume
};

// One event in a piano-pedal line (see PedalKind). Put a sequence of these in
// DirectionData::pedals to describe an entire pedaling; each event stands on its own and is
// placed at its own tickTimePosition.
struct PedalData
{
    PedalKind kind;
    int tickTimePosition;
    PositionData positionData;

    PedalData() : kind{PedalKind::unspecified}, tickTimePosition{0}, positionData{}
    {
    }

    PedalData(PedalKind inKind) : kind{inKind}, tickTimePosition{0}, positionData{}
    {
    }
};

MXAPI_EQUALS_BEGIN(PedalData)
MXAPI_EQUALS_MEMBER(kind)
MXAPI_EQUALS_MEMBER(tickTimePosition)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(PedalData);
} // namespace api
} // namespace mx
