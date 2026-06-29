# Step 3: Wire the Feature Through `mx::impl`

`mx::impl` translates between `mx::api` (the public model from step 2) and `mx::core` (the generated,
strongly-typed MusicXML XSD model). Almost every feature needs work in both directions, and the two
must stay symmetric or the roundtrip test (step 1) fails. All source is under `src/private/mx/impl/`.
References below name files and `Class::method`s; grep for the symbol rather than relying on line
numbers.

## Reader / Writer symmetry

Paired classes: a Reader goes core -> api (deserialize), a Writer goes api -> core (serialize).

| Level     | Reader (core -> api)                   | Writer (api -> core)                     |
|-----------|----------------------------------------|------------------------------------------|
| Score     | `ScoreReader.cpp`                      | `ScoreWriter.cpp`                        |
| Part      | `PartReader.cpp`                       | `PartWriter.cpp`                         |
| Measure   | `MeasureReader.cpp`                    | `MeasureWriter.cpp`                      |
| Note      | `NoteReader.cpp` + `NoteFunctions.cpp` | `NoteWriter.cpp` + `NotationsWriter.cpp` |
| Direction | `DirectionReader.cpp`                  | `DirectionWriter.cpp`                    |

Small `*Functions.{h,cpp}` helpers (e.g. `DynamicsReader`/`DynamicsWriter`, `TechnicalFunctions`,
`OrnamentsFunctions`) each handle one element family; the readers/writers delegate to them.
`Converter.{h,cpp}` holds the enum-to-enum conversions used by both sides.

Delegation chain on read: `ScoreReader::getScoreData` -> a `PartReader::getPartData` per part -> a
`MeasureReader::getMeasureData` per measure, which walks the measure contents and dispatches notes to
`NoteFunctions`/`NoteReader` and directions to `DirectionReader`. The write side mirrors it exactly:
`ScoreWriter::getScorePartwise` -> `PartWriter::writeMeasures` -> `MeasureWriter::getPartwiseMeasure`,
emitting `core::MusicDataChoice` items and delegating to `NoteWriter` and `DirectionWriter`.

> The failure mode to avoid is adding only the read half. `<segno>` is read
> (`DirectionReader::parseSegno` fills `DirectionData.segnos`) but never written (`DirectionWriter`
> has no segno case), so a segno read from a file is silently dropped on write. The read half alone
> is not "done."

## How a measure is walked

A part is a `std::span<core::PartwiseMeasure>`; the interesting work is inside each measure.

Read: `MeasureReader::getMeasureData` iterates `musicData()`, a sequence of type-tagged
`core::MusicDataChoice`. `parseMusicDataChoice` is an if/else on `mdc.isNote()` / `isBackup()` /
`isForward()` / `isDirection()` / `isAttributes()` / `isHarmony()` / `isBarline()`, dispatching to
`parseNote`, `parseBackup`, `parseForward`, `parseDirection`, `parseAttributes`, `parseBarline`, etc.
`parsePrint` / `parseSound` / `parseGrouping` / `parseLink` / `parseBookmark` are deliberate no-ops.
The loop peeks one item ahead to detect whether the next note carries a `<chord>` tag, which the time
logic needs.

`api::MeasureData` is not a flat list: it is `staves[]`, each with `clefs[]`, `keys[]`,
`directions[]`, and `voices` (a `std::map<int, VoiceData>`, each voice holding `notes[]`). Read places
each note via `insertNoteData(noteData, staffIndex, voiceIndex)` and each direction via
`staff.directions.emplace_back(...)`.

Write: `MeasureWriter::writeStaves` -> `writeVoices` loops the voice map and then the notes within
each voice, emitting one `core::MusicDataChoice::note(...)` per note and interleaving clefs, keys, and
directions by tick position. Because api stores notes per voice but MusicXML is a single linear stream
per measure, the writer synthesizes `<backup>`/`<forward>` as it moves between voices and positions
(see below).

## tickTime: the part most likely to be done wrong

Musical time is tracked in integer ticks by a cursor. `MeasureCursor` (base class `Cursor`,
`Cursor.h`/`MeasureCursor.h`) carries `tickTimePosition` (current position in the measure),
`ticksPerQuarter` (the local divisions for the current note), `voiceIndex`, `staffIndex`,
`isChordActive`, `isBackupInProgress`, and `isFirstMeasureInPart`.

Local vs global divisions: each part may declare its own `<divisions>`, but api uses one score-wide
`ticksPerQuarter`. `ScoreReader::findMaxDivisionsPerQuarter` takes the LCM of every `<divisions>`
value found; that becomes `ScoreData.ticksPerQuarter` (`Cursor::getGlobalTicksPerQuarter`).
`Cursor::ticksPerQuarter` holds the current local divisions (updated by `parseAttributes`).
`Cursor::convertDurationToGlobalTickScale` rescales a local `<duration>` into global ticks
(`local_duration * global / local`, rounded).

The rule that governs both directions:

- Read: read `tickTimePosition` off the cursor and store it on the api item.
- Write: do not emit time yourself. Set the api item's `tickTimePosition` correctly; the writer
  places the item and synthesizes `<backup>`/`<forward>` as needed.

Read mechanics: `MeasureReader::advanceTickTimePosition(amount, reason)` mutates the cursor and logs a
`HistoryRecord`. A note stores `myCursor.tickTimePosition`, then `parseNote` advances by the note's
duration unless it is a chord member. `parseBackup` bumps the voice index then subtracts the backup
duration (clamped at zero); `parseForward` adds. Zero-duration items (directions, attributes,
barlines) call `advanceTickTimePosition(0, ...)` to keep the history consistent. Directions get their
position in `DirectionReader::initializeData`, adjusted for `<offset>` in `updateTimeForOffset`.

Write mechanics: `MeasureWriter` owns a `History` wrapping a `MeasureCursor`. Before each note,
`writeForwardOrBackupIfNeeded` compares the note's `tickTimePosition` to the cursor: a negative
difference emits a synthesized `core::Backup`, a positive one a `core::Forward`. After a note,
`advanceCursorIfNeeded` moves the cursor by the note's duration unless it is a non-final chord member
(`isAdvanceNeeded`). Clefs, keys, and directions flush once their `tickTimePosition <= cursor`. A
direction whose stored time differs from the cursor gets a `core::Offset` instead of a backup/forward
(`DirectionWriter`).

## Worked example: a dynamic (mark on a note)

Read: `NoteFunctions::parseNotations` switches on `core::NotationsChoice::Kind`; the `dynamics` case
constructs a `DynamicsReader` and calls `parseDynamics(...marks)`, which maps the core kind to an
`api::MarkType` via `Converter::convertDynamic`, sets `markData.tickTimePosition`, and appends to the
marks vector.

Write: `NoteWriter::getNote` constructs a `NotationsWriter` that emits the marks back into a
`core::Notations`; the dynamic itself is rebuilt by `DynamicsWriter::getDynamics`, the symmetric
inverse. `Converter` supplies the enum mapping for both directions.

## Instructions

Assume step 2 already added the api field(s)/enum(s). For each step, find the existing element most
like yours and copy its pattern - do not invent structure.

1. Classify the element to pick the reader/writer pair:
   - note attachment (articulation, ornament, technical, dynamic, fermata) -> `NoteFunctions` +
     `NotationsWriter` (or a dedicated `*Functions` pair)
   - `<direction-type>` child (wedge, words, pedal, segno, metronome) -> `DirectionReader` +
     `DirectionWriter`
   - measure-level item (barline, clef, key, attributes field) -> `MeasureReader` + `MeasureWriter`
   - note property (stem, notehead, tie) -> `NoteReader`/`NoteFunctions` + `NoteWriter`
   - part/score header field -> `PartReader`/`ScoreReader` + `PartWriter`/`ScoreWriter`

2. Add the read. Pull the value out of the core element into the api struct, guarding every optional
   core accessor with `.has_value()` before dereferencing. Copy `MeasureReader::parseBarline` (measure
   item), `DirectionReader::parseSegno` (direction child), or the `dynamics` case in
   `NoteFunctions::parseNotations` (note mark).

3. Add the symmetric write - the step most often skipped (see the segno note above). Rebuild the core
   element from the api field. Copy `MeasureWriter::writeBarlines`, or a loop in
   `DirectionWriter::getDirectionLikeThings` (e.g. the wedge loop) that does
   `core::DirectionType dt; dt.setChoice(...); addDirectionType(std::move(dt), direction);`. For a note
   mark, add the inverse case in `NotationsWriter`.

4. Add enum conversions to `Converter.cpp` (declared in `Converter.h`), both directions - core -> api
   for the reader and api -> core for the writer. Match existing overloads such as
   `Converter::convert(core::BarStyle)` and its reverse.

5. If the element is time-positioned, set `tickTimePosition` on read from the cursor
   (`apiItem.tickTimePosition = myCursor.tickTimePosition`, as clefs and directions do). Do no time
   arithmetic on write - the writer flushes by comparing `tickTimePosition` to the cursor and
   synthesizes `<backup>`/`<forward>` (`writeForwardOrBackupIfNeeded`).

6. If the element has a duration (it consumes musical time, like a note or rest), on read convert it
   with `myCursor.convertDurationToGlobalTickScale(coreDuration)`, store it in
   `durationData.durationTimeTicks`, and advance with `advanceTickTimePosition(ticks, ...)` following
   `MeasureReader::parseNote` (mind the chord case). On write, emit
   `core::PositiveDivisions{core::Decimal{static_cast<double>(ticks)}}` as in
   `NoteWriter::assembleNoteChoice`; `MeasureWriter::advanceCursorIfNeeded` handles the cursor. A
   duration is not a position - step 5 is where, this is how long.

7. Respect staff/voice placement. Read derives a zero-based staff index from the 1-based core
   `staff()`/`number()`, clamps it in range, and inserts into the correct `staves[]` slot (see
   `MeasureReader::parseDirection` and `insertNoteData`). Write reads the index back off the cursor and
   adds one (`myCursor.staffIndex + 1` in `NoteWriter::setStaffAndVoice` and `DirectionWriter`).

8. Handle core `OneOrMore<T>` containers on write. They start with one default element, so the first
   real item must replace it and later items are added - copy the `myIsFirstDirectionTypeAdded` pattern
   in `DirectionWriter::addDirectionType` or the measure-collection logic in `PartWriter::writeMeasures`.
   If a core `add*` reports failure (e.g. a capacity cap), refuse rather than silently drop - see the
   `WriteRefusal` throws in `NoteWriter`.

9. Extend, don't break, the dispatch chains. A new `core::MusicDataChoice` kind goes in
   `MeasureReader::parseMusicDataChoice` and is emitted from `MeasureWriter`; a new
   `core::NotationsChoice::Kind` gets a `case` in `NoteFunctions::parseNotations` and its inverse in
   `NotationsWriter`. Update `PartReader::calculateNumStaves` if your element carries a staff number
   that could imply more staves than otherwise detected.

10. Verify symmetry with a roundtrip. Once both halves compile, run the step-1 api roundtrip test and
    `make test`. The roundtrip deserializes then reserializes; a missing write, wrong tick position, or
    dropped attribute makes the output DOM differ from the input and the test fail. A passing roundtrip
    is the definition of symmetric and done.

11. Keep it lean and ASCII. Match the surrounding code style; run `make fmt` and `make check`. Add only
    the api surface the feature actually needs - every field added here must be both read and written.
