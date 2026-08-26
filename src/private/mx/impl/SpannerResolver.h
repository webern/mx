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
// Resolves the writer-side facts that a spanner endpoint cannot know on its
// own, in one pass over a part before the part is written: the MusicXML
// 'number' attribute for every spanner start/continue/stop, and the size an
// octave-shift stop inherits from the start it closes.
//
// == Numbers ==
//
// A SpannerNumber self-describes as unspecified, explicit, or identity (see
// mx/api/ApiCommon.h). Explicit levels are emitted verbatim and unspecified
// spanners emit no number, so only identity spanners need writer-side
// assignment -- but explicit spanners still participate here, because an
// explicit level must be treated as reserved while it is open so a
// concurrently-open identity spanner is never handed the same number.
//
// Numbers come from a pool of 1..16 per part and per spanner class (slur,
// tied, wedge, octave-shift, bracket, dashes, glissando, slide, and wavy-line
// each have their own pool; a slur numbered 1 and a wedge numbered 1 do not
// conflict). The number-level documentation scopes concurrency to the part,
// never the staff: two spanners conflict exactly when they overlap in the
// order a streaming reader encounters them, even when they sit on different
// staves of the part. So resolvePart walks the part in the exact order
// MeasureWriter serializes it: measures in order, staves in order, voices
// ascending, notes in vector order (curve stops/continues/starts, then
// glissando/slide stops then starts, then wavy-line stops/continues/starts,
// per note -- mirroring NotationsWriter), and each staff's directions in
// vector order (mirroring DirectionWriter's per-direction emission order). An
// identity spanner takes the lowest number that is free across its whole
// serialized extent -- from its first event to its last, whichever of
// start/stop comes first in the stream -- and releases it afterward. The one
// place the writer departs from this order is a same-note span (see below),
// emitted start-first; its two events stay next to each other, so a number
// chosen from this walk still holds.
//
// Identity ids are scoped per part and per spanner class: events in the same
// part sharing a class and id are one logical spanner, even across staves.
// Pedal starts/stops carry SpannerNumber, but mx::api does not model the
// <pedal> number attribute (added in MusicXML 3.1), so pedals are ignored
// here and no number is ever emitted for one.
//
// If more than 16 spanners of one class are open at once in a part (which no
// real score approaches), resolution fails loudly with an exception rather
// than emitting an illegal number.
//
// == Same-note spans ==
//
// A spanner can begin and end on one note. Finale produces this shape
// routinely: it can only anchor a glissando or slide line to a notehead, so a
// line drawn toward a rest starts and stops on the note it leaves from.
// NotationsWriter must emit such a pair start-first, while a note that merely
// chains two spanners (one ends where the next begins) keeps its
// stop-before-start order (#139). The two shapes can carry identical numbers,
// so a note cannot tell them apart on its own. The walk here can: a stop that
// closes a spanner opened on an earlier note belongs to a chain, and a stop
// that closes nothing pairs with a same-numbered start on its own note. Both
// endpoints of each detected pair are recorded so the writer can ask about
// either one (see sameNoteSpanPartner). Only the classes the writer reorders
// are tracked: glissando, slide, and wavy-line (#429).
//
// == Octave-shift stop sizes ==
//
// An octave-shift's size is stated by api::OttavaStart::ottavaType alone;
// api::OttavaStop has no size of its own. The same walk therefore pairs each
// ottava stop with the start it closes and records that start's size, so
// DirectionWriter can write the stop's size attribute without the author
// restating it (see api/OttavaData.h).
//
// Pairing is by SpannerNumber within the part, in serialized order: identity
// endpoints pair by id, explicit endpoints by level, and unspecified
// endpoints with each other. Within one of those groups a stop closes the
// most recently opened start that is still open, so overlapping ottavas that
// carry distinct numbers or identities never cross, and pairing spans
// measures. A stop with no open start is left unpaired and falls back to
// MusicXML's default size of 8.
class SpannerResolver
{
  public:
    SpannerResolver() = default;

    // Walks inPart in serialization order, assigning a number to every
    // identity spanner event and a size to every ottava stop. May be called
    // once per part of a score; results accumulate (object addresses are
    // unique across parts).
    void resolvePart(const api::PartData &inPart);

    // The number the writer should emit for the given spanner object, or
    // nullopt to omit the attribute. inObject must be the address of the same
    // start/continue/stop object (within the ScoreData being written) that
    // resolvePart visited. Throws if inNumber is an identity that resolvePart
    // never saw -- that is a wiring bug, and omitting the number could emit a
    // colliding spanner pair.
    std::optional<int> emittedNumber(const api::SpannerNumber &inNumber, const void *inObject) const;

    // The size (8, 15, or 22) the writer should emit for the given ottava
    // stop, taken from the start it closes. inObject must be the address of
    // the same object resolvePart visited. A stop this resolver never paired
    // -- one with no matching start, or one written outside a resolved part --
    // gets MusicXML's default size of 8.
    int ottavaStopSize(const void *inObject) const;

    // The other end of a spanner that starts and stops on the same note, or
    // nullptr when the spanner extends beyond its note. inObject must be the
    // address of the same start/stop object that resolvePart visited. Tracked
    // for glissando, slide, and wavy-line only -- the writer does not reorder
    // the other spanner classes.
    const void *sameNoteSpanPartner(const void *inObject) const;

  private:
    std::unordered_map<const void *, int> myResolved;
    std::unordered_map<const void *, int> myOttavaStopSizes;
    std::unordered_map<const void *, const void *> mySameNoteSpanPartners;
};
} // namespace impl
} // namespace mx
