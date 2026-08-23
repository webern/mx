---
ai_instructions: >
    This document is split between human-authored content and ai-authored content. Everything before
    `## AI Section` is human-authored. Only edit the human section when asked specifically by the
    user and your edits to the human section should be minimally invasive and surgical. The rest of
    the document after `## AI Section` is yours to author and edit.
ai_section_instructions:
  - design_overview: limited to 350 words or less. If you must continually compress it to fit.
  - implementation_details: as long and as wordy as you want it to be.
---
# Spanner Design

## Definitions

**Split-spanner** (`s_span`): this is the way MusicXML models spanners (and many things). There is a
node in the MusicXML at the point where the spanner starts, and another one where the spanner stops.
To understand the extent of the spanner one must parse past the start point all the way to the stop
point, then associate the stop point with an unfinished, in-memory spanner. The association is
managed by being in the same part and using a numbering system such as `number`. We may further
define:
- `s_span:start` as the start of an `s_span` and
- `s_span:end` as the end of an `s_span`

**Unified-spanner** (`u_span`): this is a conceptually different way of specifying a spanner in
which the start and end of the spanner are specified together in a single data point. This is how
`mnx` does it, by using IDs as pointers.

## Problem

When an application is walking through its music data-model and writing it out to MusicXML, it is
difficult to manage the state required for authoring MusicXML's `s_span`s. It requires a sort of
spanner state machine so that the `s_span:end` can be written and properly associated with its
`s_span:start`. Most applications model a spanner as a single object and this start and stop
mechanism is unnatural to implement.

### Request

Make it possible for an application to author the start and end of a spanner at the same time and in
one place, i.e. with a `u_span` object (potentially located where the `s_span:start` normally goes).
Implicit in the request is the idea that the same `u_span` mechanism would be nice to have when
*reading* parsed files from `mx::api`; however, this may be a bit harder to implement.

## Definitions

- `sp_start`: an `mx::api` spanner object that only indicates startpoint without specifying endpoint

- `sp_stop`: an `mx::api` spanner object that indicates the endpoint of an `sp_start`

- `u_span`: an `mx::api` spanner object, inserted at spanner start location that indicates where the
  spanner should end

- `xml_start`: the MusicXML node that starts a spanner, i.e. read/written by `mx`.

- `xml_stop`: the MusicXML node that stops a spanner, i.e. read/written by `mx`.

## Requirements

- R1(rpatters1): Ability to use `u_spans` when authoring documents with `mx::api`. A spanner's stop
  point should be specified with a note ID when creating the `u_span`. Stated more generally, an
  application should be able to author a spanner's start and end note in one operation, at the time
  program state is focused on the spanner start.

- R2(rpatters1): R1 should be possible even if the end note has not yet been added to the score.

- R3(rpatters1): R1 and R2 should be accomplished through the use of note IDs, inspired by MNX.

- R4(webern): Use of `u_spans` when authoring a score should be opt-in. If a user prefers to author
  `s_spans` directly, they should be able to do so.

- R5(webern): Use of `u_spans` when reading a score should be possible, and opt-in. This may
  necessitate supplying a setting prior to parsing the score.

- R6(webern): `u_spans` require the client to be using MusicXML's ID feature (`<note id="x">`).
  These IDs belong to the client and are serialized like any other attribute. `mx::api` must never
  generate or require mx-specific IDs; it is agnostic about ID content except that it must see the
  IDs for the purpose of spanner endpoint detection.

- R7(webern): An application authoring `u_spans` MAY choose to put its own IDs in the `<note@id>`
  attributes and use those same IDs for `u_span` specification. i.e., an application MUST NOT be
  required to place `mx::api`-specific IDs into the written MusicXML.

- R8(webern): The `u_span` interface should be a common interface that works with compatible spanner
  types. The following tables list the spanner and spanner-like objects in MusicXML along with the
  elements that contain them. A spanner is in scope when its `s_span:end` can be placed from an end
  note ID alone -- either into that note, or into the measure at that note's tick position.

  The schema of the following tables is as follows (null|empty means yes, no means no):
  - spanner: the element which has start, stop (and maybe continue) attributes
  - container: the element inside which the spanner element is housed
  - descendant_of: the `music-data` child element that ultimately contains the spanner
  - blocker: a GitHub issue that would unblock this spanner from being `u_span` eligible
  - phase: `poc` (first trial implementation), `1` follow up from the poc. `2`: doable after phase
    `1`. `TBD`: not sure if we want to support it.

**In scope.** `descendant_of` splits the write mechanism into exactly two cases:

- `note`: the `s_span:end` goes into the end note itself, which already exists in the client's data.
- `direction`: the `s_span:end` goes into a new `DirectionData` placed at the end note's
  `tickTimePosition`. The end note supplies time only; a wedge is one graphical object on one staff,
  so staff, voice, and placement are copied from the start, not from the end note.

Both cases are fully determined by an end note ID plus data copied from the start.

| spanner           | container        | descendant_of | blocker | phase |
|-------------------|------------------|---------------|---------|-------|
| `bracket`         | `direction-type` | `direction`   |         |       |
| `dashes`          | `direction-type` | `direction`   |         |       |
| `octave-shift`    | `direction-type` | `direction`   |         |       |
| `pedal`           | `direction-type` | `direction`   |         |       |
| `principal-voice` | `direction-type` | `direction`   |         |       |
| `wedge`           | `direction-type` | `direction`   |         |       |
| `extend`          | `lyric`          | `note`        |         |       |
| `glissando`       | `notations`      | `note`        |         |       |
| `other-notation`  | `notations`      | `note`        |         |       |
| `slide`           | `notations`      | `note`        |         |       |
| `slur`            | `notations`      | `note`        |         |       |
| `tied`            | `notations`      | `note`        |         |       |
| `tuplet`          | `notations`      | `note`        |         |       |
| `tie`             | `note`           | `note`        |         |       |
| `tremolo`         | `ornaments`      | `note`        |         |       |
| `wavy-line`       | `ornaments`      | `note`        |         |       |
| `hammer-on`       | `technical`      | `note`        |         |       |
| `pull-off`        | `technical`      | `note`        |         |       |

**Out of scope.** These are neither note-attached nor tick-attached, so an end note ID cannot place
their `s_span:end`.

| spanner            | container        | descendant_of  | why                                          |
|--------------------|------------------|----------------|----------------------------------------------|
| `grouping`         | `measure`        | (self)         | measure-level; no tick position              |
| `beat-repeat`      | `measure-style`  | `attributes`   | measure-scoped                               |
| `measure-repeat`   | `measure-style`  | `attributes`   | measure-scoped                               |
| `slash`            | `measure-style`  | `attributes`   | measure-scoped                               |
| `ending`           | `barline`        | `barline`      | measure-scoped; end lands in a barline       |
| `wavy-line`        | `barline`        | `barline`      | measure-scoped; the barline instance         |
| `metronome-tied`   | `metronome-note` | `direction`    | both ends sit in one `metronome`             |
| `metronome-tuplet` | `metronome-note` | `direction`    | both ends sit in one `metronome`             |
| `extend`           | `figure`         | `figured-bass` | figured-bass instance of `extend`            |
| `barre`            | `frame-note`     | `harmony`      | spans frets in a chord diagram; no time axis |
| `level`            | `many`           | `various`      | too many containers to address; see below    |

The measure-scoped rows (`ending`, `wavy-line` in `barline`, `beat-repeat`, `measure-repeat`,
`slash`) are deferred rather than impossible. They need a different anchor concept -- a measure ID,
which `mx::api` already exposes as `MeasureData::id` -- plus a rule for whether the end lands on the
named measure or before it.

The `<level>` element is hard to understand and can be contained in such a large variety of
containing elements (attributes, backup, barline, direction, figure, figured-bass, forward, harmony,
lyric, notations, note) that `u_span` support is not planned for `<level>` at this time.

My answers to Claude's questions:

> 1. **What is an id?** `NoteData` has no id field today. Say where the id lives, what strings are
>    allowed, and how widely it must be unique (score or part). Say what happens when two notes
>    share an id.

Decided: An ID is created and provided by the user in the schema-specified ID attribute fields
(which `mx::api` must expose). `mx::api` does not treat these as special; they are no different from
any other MusicXML attribute. They are part of `mx` equality checks and MusicXML round-trips like
any other attribute or element. `mx::api` does not care what they contain. `mx::core` MAY enforce
the syntax (`NCName`) and uniqueness, but `mx::api` will not.

> 2. **What happens to a bad reference?** R2 lets a `u_span` point at a note that does not exist
>    yet, so a dangling id can only be caught at write time. The api never throws. Say what the
>    writer does then: fail with a `Result`, drop the spanner, or write something safe. The api
>    doctrine warns against ids that can dangle, so the design must face this squarely.

`mx::api` cannot detect a bad reference when it is added to the score. The feature of automatically
placing the correct `s_span:stop` (and/or `xml_stop`) into the score happens when writing to
`mx::core` and is a best effort. If, at the end of writing to `mx::core`, spanners are left over or
`u_spans` were improperly specified, an error should be provided.

Note, we should provide a better error reporting mechanism than we currently have. We should be able
to distinguish informational messages, warning messages, and error messages. This requires a
separate design which may or may not be considered a prerequisite to the current design and
implementation.

> 3. **Non-note spanners.** A wedge does not sit on a note. What does its end point at: a note id
>    meaning "at this note's time position", or a measure and beat? This choice shapes the whole
>    interface (R9).

Based on the answer to number 1, above, we can initially offer `u_span` mechanics for spanners that
attach to elements that carry an ID attribute in the spec. We should distinguish our list of
spanners into those that do and do not attach to identifiable elements.

> 4. **Both ends carry data.** In MusicXML each end node has its own attributes (a slur stop has
>    bezier fields, a wedge stop has `spread`). One `u_span` object must hold data both ends. Also
>    say who assigns `number` when spanners overlap: mx or the user.

Any non-automatic, overridable, or specifiable data needed for the `s_span:stop` will be needed in
the `u_span`.

> 5. **Reading.** Parsed notes have no ids. Does mx invent them? Real files also contain stops with
>    no start, and starts with no stop. Say what the reader gives the user then (R5).

Parsed notes might have note IDs. Providing `u_span` resolution when reading scores should be
considered an extension of the MVP. The MVP works for writing only; the full feature works for
reading. There are a few ways that a score could work with this feature upon reading:
- It already has the requisite IDs.
- We add a feature that instructs `mx::api` to generate and fill in missing IDs.
- We have a function for converting `s_spans` to `u_spans` which fails if needed IDs are missing.

If the IDs are there, then it is not impossible for us to convert `s_span` pairs to `u_span` pairs
from a completed document.

> 6. **Opt-in shape.** Is opt-in a `DocumentManager` setting or a flag on the score? Can one score
>    mix u_spans and s_spans?

This project may require adding an options structure to the `DocumentManager` (at least for
resolution when reading scores). We may want to fix #95 (Use RAII for the `DocumentManager`) while
we're at this.

> 7. **Existing types.** The api already has `SpannerStart`/`SpannerStop`, `CurveData`,
>    `TupletData`, `WedgeData`, and friends. Say whether the new design may replace them (a breaking
>    change) or must sit beside them. Also, the api merges `tie`/`tied` into one fact today, while
>    R8 lists them as two types.

We still need support for `s_span:stop` objects. If possible, we should maintain the existing types
for `s_span:stop` objects and compose them in `u_span` objects.

### Recent, Important Design Decisions from GitHub

The following of my messages, from discussion #349, describe my core design decision:

> thinking about this has brought me around to a direction like this:
> 
> - ids belong to the client, they are the MusicXML-specified attributes
> - mx::api doesn't care what's in them or what they look like etc
> - for a u_span you could specify an end-id
> - mx::api will look for that end-id in the notes in the appropriate context
> - when it sees it it drops the spanner end
> - if it has spanners left over, you screwed up. Maybe it warns or whatever. Worst case your
>   document has ill-specified spanners

And:

> It's a hands off, IDs-are-your-business-not-mine approach.
> 
> mx agnostically says
> 
> - you want to write ids in your MusicXML nodes, cool I don't care about that
> - you want me to write a spanner element when I see one of your IDs, ok I accept the mission but
>   you're on your own to make sure you actually add that ID into the score sensibly
> - It will be more complicated than that, but it offloads all of my ID problems and fits the
>   library's design, I think.

### Design Direction

This covers the core tenets of the design, generally:
- IDs are specified by MusicXML.
- Like all values in MusicXML, `mx::api` is agnostic about IDs.
- `u_span` may refer to a client-provided ID, but does not require `mx::api` IDs in the output.

The design is as follows:

A class (ideally a single, templated class if possible) will be created that can carry both the
start and stop `s_spans` at the same time. This object is the `u_span` object. In places where an
`s_span` can be added to the data model, it must be possible (again by templating, choice, or other
union/variant sorts of tricks) to add *either* an `s_span` or a `u_span` into the same spot (i.e.
into the same vector).

When writing the score, a non-user-facing `Context` object will exist and always be accessible
during the `mx::impl` writing (and later, reading) process to represent a global "state of the
document write" (later: read). This will carry error-reporting machinery and spanner state. It may
build on the currently existing `SpannerNumberResolver` object.

> TBD: The public `SpannerNumber` object might need to change, or maybe it's not actually involved?
> TODO: understand how this new design interacts with the design created in PR #320.

**Write-side**: The writer state will be checked when adding eligible objects to the score to see if
it has an open `u_span` with an endpoint ID matching the ID of the object it is about to write. If
so, it will add the appropriate `s_span:end` object to the `ScoreData` in the correct place.

**Read-side**: A post-processing step will iterate over the score, find eligible `s_span:start`
objects and collect them, search for their matching end-points, and if the end-point has an ID,
remove it from the score and replace the `s_span:start` object with a `u_span` containing both the
start and stop information.

**Error reporting**: When errors in spanner handling can be reasonably detected, they will be added
to the `Context` error reporting mechanism.

## AI Section

### Design Overview

**Data model.** A `u_span` composes the existing `s_span:start` and `s_span:stop` structs (e.g.
`CurveStart` + `CurveStop`) plus the endpoint ID — ideally one templated class. Wherever an
`s_span:start` can be added, the vector's element type becomes a choice type (the existing `mx::api`
choice pattern, Kind enum) holding either the `s_span:start` or the `u_span`. Client breakage is
accepted. The POD design holds: no validation at authoring time (R11), and forward references are
allowed — the end note need not exist yet (R2).

**Write-side.** A non-user-facing `Context` object exists throughout the `mx::impl` write
(precedent: `ScoreWriter`'s `SpannerNumberResolver`), carrying open-u_span state and error
reporting. When the writer is about to emit a u_span-eligible element, it checks that element's ID
against pending u_spans. On a match it validates that the current context is appropriate for
completing that spanner type (part/voice/etc.; rules are per element type and defined during
implementation), then emits the `s_span:stop` there. Resolution is best effort: u_spans still
pending at end of write are reported as errors through the `Context`. Error reporting should
distinguish info/warning/error; that mechanism may be a separate prerequisite design.

**Read-side.** MVP is write-only. The full feature adds an opt-in post-processing pass that converts
`s_span` pairs to `u_spans` where the endpoint carries an ID, configured via a `DocumentManager`
options structure (R5).

**Scope.** Eligibility is limited to element types carrying schema ID attributes. MVP covers
note-attached types (R8); R9/R10 types follow.

**SpannerNumber.** Expected unchanged: the composed start/stop structs already carry it, and the
resolver walks serialization order, which includes materialized stops.

### Decisions Needed

Harvested from a throwaway implementation spike (curves only: `NoteData::id`, a templated
`USpan<StartT, StopT>` + `SpanChoice` in `curveStarts`, and a lowering pass in `ScoreWriter`). All
spike code was discarded. The spike compiled cleanly; impl blast radius for the `curveStarts` type
change was 4 sites (`NotationsWriter.cpp`, `NoteWriter.cpp`, `SpannerNumberResolver.cpp`,
`CurveFunctions.h`) plus ~30 usages in 6 test files.

1. **Sound-level ties cannot be u_spans in the current model.** `<tie>` is modeled as
   `NoteData::isTieStart` / `isTieStop` bools (`src/include/mx/api/NoteData.h:120`), not as vector
   entries, so there is no place to put a `USpan` for it. `CurveType::tie` covers `<tied>` notation
   only. Decide: drop sound-level tie from R8's u_span list, or restructure tie modeling.

2. **The error channel cannot express "best effort."** `Result` (`src/include/mx/api/Result.h`)
   carries exactly one `ApiError` and is all-or-nothing: an error from `createFromScore` means no
   document. Unresolved u_spans + "worst case your document has ill-specified spanners" needs
   success-with-warnings. Decide: (a) hard-fail `createFromScore` on unresolved u_spans, or (b)
   build the info/warning/error reporting design first as a hard prerequisite.

3. **Ratify the lowering-pre-pass architecture.** The spike implemented u_span resolution as a
   ScoreData -> ScoreData transform in the `ScoreWriter` constructor (it already owns a mutable copy
   and sorts it), after `sort()` and before `SpannerNumberResolver::resolvePart`. Everything
   downstream (number resolution, writers) then runs unchanged on pure s_spans. This replaces the
   "check while emitting" wording in the Design Direction and makes the read-side the symmetric
   raising transform. It worked cleanly in the spike.

4. **Matching order is serialization order, not time order.** The walk is measures -> staves ->
   voices -> notes (mirroring `SpannerNumberResolver::resolvePart`), so "first element carrying the
   end-id" can differ from first-in-musical-time when the endpoint is in another voice or staff.
   Also: duplicate IDs mean first-match-wins silently; a u_span whose end-id is on its own start
   note or earlier never resolves and becomes an error. Ratify these semantics or restrict matching
   (e.g. same voice only) per element type.

5. **Disposition of an unresolved u_span.** The spike dropped the whole spanner (start never
   written) and produced an error message. Alternative: write a dangling `s_span:start`. Decide.

6. **Number pairing for materialized stops.** The lowering pass copies the start's `SpannerNumber`
   to the stop; when the start's number is unspecified it generates a shared identity (e.g.
   `mx::uspan::1`) so the resolver pairs the halves. Generated identities can collide with
   client-chosen identity strings. Decide: reserve a namespace, or pair by a non-string mechanism.
   Note also `TupletStart`/`TupletStop` use a raw `numberLevel` int, not `SpannerNumber` -- number
   pairing is per-type work.

7. **Choice-type fallback requires default-constructible payloads.** The choice pattern's wrong-kind
   accessor returns a default-constructed copy, but `CurveStart`/`CurveStop` have no default
   constructor (they require a `CurveType`). The spike added defaults with `CurveType::unspecified`,
   which the writer silently skips. Ratify or choose another fallback.

8. **U_span scores do not round-trip structurally in the MVP.** Writing a score authored with
   u_spans and reading it back yields s_span pairs, so `ScoreData` equality fails against the
   original. Affects the api-roundtrip harness and user expectations until the read-side raising
   pass exists. Bless this asymmetry for the MVP.

Implementation note (no decision needed): `SpannerNumberResolver` keys spanner events by object
address; for curve choices the wrapper address is the right key, matching the existing
`DirectionChoice` precedent in `SpannerNumberResolver.cpp`.

> claude --resume 459bba13-2e60-404a-b75f-3be7dec02b01

### Implementation Details

TODO
