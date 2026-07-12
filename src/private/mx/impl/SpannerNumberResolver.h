// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"

#include <optional>
#include <unordered_map>

namespace mx
{
namespace impl
{
// Resolves the MusicXML 'number' attribute for every spanner start/continue/stop
// in a part before the part is written.
//
// A SpannerNumber self-describes as unspecified, explicit, or identity (see
// mx/api/ApiCommon.h). Explicit levels are emitted verbatim and unspecified
// spanners emit no number, so only identity spanners need writer-side
// assignment -- but explicit spanners still participate here, because an
// explicit level must be treated as reserved while it is open so a
// concurrently-open identity spanner is never handed the same number.
//
// Numbers come from a pool of 1..16 per staff and per spanner class (slur,
// tied, wedge, octave-shift, bracket, and dashes each have their own pool; a
// slur numbered 1 and a wedge numbered 1 do not conflict). Two spanners
// conflict only when they overlap in the order a streaming reader encounters
// them, so resolvePart walks the part in the exact order MeasureWriter
// serializes it: measures in order, staves in order, voices ascending, notes
// in vector order (curve stops, then continues, then starts per note --
// mirroring NotationsWriter), and each staff's directions in vector order
// (mirroring DirectionWriter's per-direction emission order). An identity
// spanner takes the lowest number that is free across its whole serialized
// extent -- from its first event to its last, whichever of start/stop comes
// first in the stream -- and releases it afterward.
//
// Identity ids are scoped per part and per spanner class: events in the same
// part sharing a class and id are one logical spanner, even across staves. A
// cross-staff identity spanner reserves its number in every staff pool it
// touches. Pedal starts/stops carry SpannerNumber but the <pedal> element has
// no number attribute, so pedals are ignored here.
//
// If more than 16 spanners of one class are open at once in a staff (which no
// real score approaches), resolution fails loudly with an exception rather
// than emitting an illegal number.
class SpannerNumberResolver
{
  public:
    SpannerNumberResolver() = default;

    // Walks inPart in serialization order and assigns a number to every
    // identity spanner event. May be called once per part of a score; results
    // accumulate (object addresses are unique across parts).
    void resolvePart(const api::PartData &inPart);

    // The number the writer should emit for the given spanner object, or
    // nullopt to omit the attribute. inObject must be the address of the same
    // start/continue/stop object (within the ScoreData being written) that
    // resolvePart visited. Throws if inNumber is an identity that resolvePart
    // never saw -- that is a wiring bug, and omitting the number could emit a
    // colliding spanner pair.
    std::optional<int> emittedNumber(const api::SpannerNumber &inNumber, const void *inObject) const;

  private:
    std::unordered_map<const void *, int> myResolved;
};
} // namespace impl
} // namespace mx
