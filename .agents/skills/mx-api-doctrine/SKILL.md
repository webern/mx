---
name: mx-api-doctrine
description: >
  The mx::api design doctrine. You MUST read this before designing or changing anything in the
  mx::api layer: adding or reshaping public types in src/include/mx/api/, wiring them through
  mx::impl, writing comments in api headers, or reviewing any of the above.
disable-model-invocation: false
user-invocable: true
---
# mx::api doctrine

`mx::api` is not "MusicXML minus rare features." It re-shapes MusicXML into a model where invalid
or ambiguous documents are hard or impossible to express. MusicXML's recurring defects --
stateful encodings, flat streams with label fields, one fact stated in two places, id-linked
declarations, order-dependent meaning -- each have a countering principle below. Never mirror an
element's raw shape. The price of every simplification is paid in `mx::impl`: the writer
regenerates the spec-compliant encoding, the reader accepts every spec-legal variant.

## The audience is a musician, not the round-trip harness

Corpus round-tripping is how we develop `mx::api`; it is not what the api is for. The api exists
for people who create music and who build, modify, and interpret MusicXML files. Public-header
comments must explain the notation semantics, how MusicXML encodes them, and how to author or
read them through the api.

- Never write from the harness's viewpoint. "The source", "preserved on round-trip", "populated
  by the reader" are wrong unless the field truly records non-semantic source fidelity (a
  principle-7 knob) -- then say so plainly (i.e. "Do not populate this when authoring").
- Do not document what is not modeled; listing what
  isn't there is noise.
- No separator/banner comments.

Bad:  `// staffIndex is zero-based; a source with an explicit number attribute always populates`
Good: `// Which staff of the part this layout applies to, zero-based from the top staff.`

## Failure model: misuse must be boring

Responses to wrong api usage, in order of preference:

1. Unrepresentable: shape the type so the wrong state cannot be expressed (choice types below;
   merged fields, principle 3).
2. Defined fallback: document a harmless result and return it. A wrong-kind choice accessor
   returns a default-constructed copy; the writer drops the half of an encoding that is
   meaningless for the note it is on (a tie on a silent cue note is written as `<tied>`
   notation only, never as a sound-level `<tie>`). No signal to the caller.
3. `Result<T>` (`Result.h`): the error channel of last resort. It exists for the
   `DocumentManager` I/O boundary, where failure is real (unreadable file, unparseable XML). Do
   not spread it into the data model.

Never:

- UB. No public call sequence may reach undefined behavior: no unchecked `std::get`, no
  returned reference whose validity depends on a precondition, no "caller must check first or
  else".
- Exceptions. Nothing throws across the api boundary, and an exception is never how a failed
  precondition is reported to the caller. `DocumentManager` catches everything
  (`ResultCode::internalError`).

## Choice types: when you wish for a Rust enum

For a true either/or with per-alternative payloads, do not bolt kind-specific fields onto one
struct, and do not expose a raw `std::variant`. Follow `TimeChoice.h` / `MarkDataChoice.h`
(headers and .cpp):

- A class holding a private `std::variant` (`std::monostate` when "none" is a state), an
  `enum class Kind`, `kind()`, and `is...()` queries.
- Accessors return a copy; wrong-kind access returns a default-constructed value -- no throw,
  no UB.
- Constructors take each alternative by value. If one alternative can express the other,
  collapse automatically (a simple-equivalent `ComplexTimeSignature` becomes a simple
  `TimeChoice`).
- Fields common to all alternatives live on the choice class (`TimeChoice::isImplicit`).
- Keep the common case prominent; quarantine complex machinery in its own header
  (`TimeSignatureData` vs `ComplexTimeSignature`). A user writing 3/4 must never wade through
  the composite interface, and the common type gets the obvious name -- not "component".
- Consistency with sibling fields (`MarkDataChoice::Kind` matching `MarkData::markType`) is a
  documented convention, not enforced by throwing.
- Grow aggregates the same way: a payload that applies only to some `MarkType`s goes in
  `MarkDataChoice` as a new alternative, never as another loose `MarkData` field.

## Design principles

### 1. Store absolute values, not running state

If interpreting an element requires remembering an earlier element, resolve it and store the
result on each item. Canonical: `ScoreData::ticksPerQuarter` + `NoteData::tickTimePosition`
replace `<divisions>`/`<backup>`/`<forward>`; the writer synthesizes backup/forward from tick
deltas (`impl/MeasureWriter.cpp`, `writeForwardOrBackupIfNeeded`). Test: could a reader process
your feature's items in isolation, in any order? If not, you have hidden state.

### 2. Express membership by containment, not by label fields

When MusicXML tags each element with a number or id (voice, staff, part id), put the element
inside the thing it belongs to. Canonical: `MeasureData` -> staves -> voices map -> notes
(`StaffData.h`, `VoiceData.h`); `PartData` merges the `<part-list>` declaration and the `<part>`
body, killing the id join. Where full containment is impractical, key a map by the owner
(`MeasureData::staffTimeSignatures`) rather than stamping a label field on the item. Test: if
two fields can disagree about where something belongs, or an id can dangle, restructure.

### 3. One fact, one field; the writer says it twice

Where MusicXML states a fact in a sound place and a notation place, expose a single api field and
emit both encodings on write. Canonical: `isTieStart`/`isTieStop` produce both `<tie>` and
`<tied>` (`impl/NoteWriter.cpp`, `addTie`); part-name formatting is written only to the
non-deprecated `*-display` home (`PartData.h` header comment). Test: if a user can construct a
self-contradictory document by setting two api fields, merge them.

### 4. No neighbor-dependent meaning

A field on note N must not change meaning based on note N-1. Canonical: `NoteData::isChord` is
true for every chord member; the `<chord>` tag's first-note-omitted rule is computed by the
writer (`impl/NoteWriter.cpp`, `setNoteChoiceAndFullNoteGroup`). Test: copy one item out of a
vector -- does it still mean the same thing alone?

### 5. Denormalize effective state onto the thing it governs

Each measure/staff carries the attribute state in effect (`timeSignature` with `isImplicit`,
`keys`, `clefs`), even when the source did not restate it; the writer decides where
`<attributes>` is emitted. Test: can a consumer answer "what is the time signature here?"
without scanning previous measures?

### 6. Make the common case a plain value; quarantine the rare case

Do not let an edge case complicate the mainline type -- and do not drop it. Canonical:
`PitchData::alter` is an int, microtones go in a separate `cents` field; the lone
`<tied type="let-ring">` is its own optional `TieLetRing` struct, not a third tie boolean
(`NoteData.h`). Test: does the 95% use read as one obvious field, while the 5% is still
expressible?

### 7. Fidelity knobs must default to "automatic" and be ignorable

When byte-level round-trip needs a source quirk recorded, add a field whose default applies the
sensible rule; only the reader sets it. Canonical: `ClefData::writeStaffNumber`,
`DurationData::isDurationNameSpecified`, `DirectionData::offset` (leave absent when authoring;
the reader fills it in). Test: can an author produce correct MusicXML while never touching the knob?
Never let authoring correctness depend on one. Where fidelity and simplification truly conflict,
prefer documented normalization (part-name formatting; output is always MusicXML 4.0) over
complicating the model.

Presence/absence conventions (`std::optional`, `unspecified` enumerators, the legacy `-1` /
`is...Specified` sentinels) are in AGENTS.md, "mx::api conventions".
