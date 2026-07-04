# mx::api design principles

mx::api is not "MusicXML minus rare features." It re-shapes MusicXML's information into a model
where invalid or ambiguous documents are hard or impossible to express. MusicXML's recurring
defects are: stateful encodings, flat streams with label fields, one fact stated in two places,
id-linked declarations, and order-dependent meaning. Each principle below counters one. When
adding a feature, do not mirror the element's shape; ask which defect it carries and apply the
matching move. The price of every simplification is paid in `mx::impl`: the writer regenerates
the spec-compliant encoding, the reader accepts every spec-legal variant.

## 1. Store absolute values, not running state

If interpreting an element requires remembering an earlier element, resolve it and store the
result on each item. Canonical: `ScoreData::ticksPerQuarter` + `NoteData::tickTimePosition`
replace `<divisions>`/`<backup>`/`<forward>`; the writer synthesizes backup/forward from tick
deltas (`impl/MeasureWriter.cpp`, `writeForwardOrBackupIfNeeded`). Test: could a reader process
your feature's items in isolation, in any order? If not, you have hidden state.

## 2. Express membership by containment, not by label fields

When MusicXML tags each element with a number or id (voice, staff, part id), put the element
inside the thing it belongs to. Canonical: `MeasureData` -> staves -> voices map -> notes
(`StaffData.h`, `VoiceData.h`); `PartData` merges the `<part-list>` declaration and the `<part>`
body, killing the id join. Test: if two fields can disagree about where something belongs, or an
id can dangle, restructure into the tree.

## 3. One fact, one field; the writer says it twice

Where MusicXML states a fact in a sound place and a notation place, expose a single api field and
emit both encodings on write. Canonical: `isTieStart`/`isTieStop` produce both `<tie>` and
`<tied>` (`impl/NoteWriter.cpp`, `addTie`); part-name formatting is written only to the
non-deprecated `*-display` home (`PartData.h` header comment). Test: if a user can construct a
self-contradictory document by setting two api fields, merge them.

## 4. No neighbor-dependent meaning

A field on note N must not change meaning based on note N-1. Canonical: `NoteData::isChord` is
true for every chord member; the `<chord>` tag's first-note-omitted rule is computed by the
writer (`impl/NoteWriter.cpp`, `setNoteChoiceAndFullNoteGroup`). Test: copy one item out of a
vector -- does it still mean the same thing alone?

## 5. Denormalize effective state onto the thing it governs

Each measure/staff carries the attribute state in effect (`timeSignature` with `isImplicit`,
`keys`, `clefs`), even when the source did not restate it; the writer decides where
`<attributes>` is emitted. Test: can a consumer answer "what is the time signature here?"
without scanning previous measures?

## 6. Make the common case a plain value; quarantine the rare case

Do not let an edge case complicate the mainline type -- and do not drop it. Canonical:
`PitchData::alter` is an int, microtones go in a separate `cents` field; the lone
`<tied type="let-ring">` is its own optional `TieLetRing` struct, not a third tie boolean
(`NoteData.h`). Test: does the 95% use read as one obvious field, while the 5% is still
expressible?

## 7. Fidelity knobs must default to "automatic" and be ignorable

When byte-level round-trip needs a source quirk recorded, add a field whose default applies the
sensible rule; only the reader sets it. Canonical: `ClefData::writeStaffNumber`,
`DurationData::isDurationNameSpecified`, `DirectionData::orderedComponents` ("Do NOT populate"
when authoring). Test: can an author produce correct MusicXML while never touching the knob?
Never let authoring correctness depend on one. Where fidelity and simplification truly conflict,
prefer documented normalization (part-name formatting; output is always MusicXML 4.0) over
complicating the model.

Presence/absence conventions (`std::optional`, `unspecified` enumerators, the legacy `-1` /
`is...Specified` sentinels) are in AGENTS.md, "mx::api conventions".
