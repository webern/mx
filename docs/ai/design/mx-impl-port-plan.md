# Phase 2 Design: the `mx::impl` Port

Status: APPROVED, BINDING. This is the decision record for Phase 2 of the newgen integration
(the `mx::impl` port), produced by studying the tree and interviewing the owner. It settles
every open question in `mx-impl-port-brief.md` §5 and amends plan §8's scope where noted
(owner-approved). Execute Phase 2 from this document. Scope authority for goal/acceptance
remains `newgen-integration-plan.md` §8; where this document says "amended," it overrides.

## 1. Goal and acceptance

**Goal:** `mx::api` works again, implemented via `mx::impl` against the new generated
`mx::core`.

**Acceptance (plan §8, with owner-approved amendments):**

- preserved `mxtest/api` + `mxtest/impl` suites ALL GREEN (per-file fates in §7);
- the new corpus-wide api roundtrip gate (§8) green with **zero regression vs. the pre-port
  baseline**;
- `MX_API` default ON;
- `lib`/`dev`/`test`/`examples-run` Makefile targets restored; examples build AND run;
- CI green tip-to-toe, including the new `linux-api` job and the extended coverage/macos jobs
  (§10);
- coverage job naturally grows to cover api/impl.

**Scope amendments vs. plan §8's "api changes minimally invasive (test-harness fallout
only)":** the owner approved two deliberate public-api changes — the `DocumentManager`
error channel (§3: `Result`-returning signatures, no escaping exceptions) and the version
fields on `ScoreData` (§4). Everything else on the api surface stays put.

**Out of scope (Phase 3):** xcode/xcframework/iOS/macOS distribution; the SMuFL
glyph-name migration (§12 — recorded there as open questions with evidence).

## 2. Port strategy: mechanical translation, algorithms preserved

**The cardinal rule of the port (owner-mandated): the impl algorithms are preserved
exactly.** They are the glue that makes `mx::api` produce the documents users want; the
logic must be the same — only the *shape of `mx::core` access* changes. NoteReader.cpp
stays NoteReader.cpp; the 77-file structure stays; the reader/writer split and the
`Converter` hub stay.

The idiom translation table (old core → new core):

| Old-core idiom | New-core translation |
|---|---|
| `#include "mx/core/elements/Foo.h"`, `Enums.h`, `Decimals.h`, `Document.h` | `#include "mx/core/generated/Foo.h"` etc. (per-type headers; aggregates are gone) |
| `core::NotePtr` / `makeNote()` shared-ptr graph | value construction; `core::Note note{};` |
| `el->setHasFoo(true); el->getFoo()->setValue(v)` | `el.setFoo(V{v})` (`std::optional` presence) |
| `attr->hasDefaultX` / `attr->defaultX.getValue()` | `el.defaultX().has_value()` / `el.defaultX()->value()` (attributes are members of the element class; no separate Attributes struct) |
| `Choice` enum + `getChoice()` + co-allocated getters | `std::variant` alternative classes; positional `Kind`/index dispatch (`choice.kind()`, `asFoo()`) |
| `getFooSet()` (vector of shared ptrs) | `foos()` (`std::vector<Foo>` / `OneOrMore<Foo>` by value) |
| old enum `core::StepEnum::a` | factory wrapper `core::Step::a()`, compare via `tag()` |
| ad-hoc clamping on import; invalid values representable | clamp-on-construct wrappers; invalid unrepresentable |
| unbounded `addBeam(...)` | bounded `addBeam(...) -> Result<void>` (propagate, see §3 row `createFromScore`) |
| required-with-unbounded: default element pre-pushed | `OneOrMore<T>` (first element exists by type) |
| `fromXDoc`/`toXDoc` per element | parse/serialize exist ONLY at the document boundary (`generated/Document.h`) |

**Where the old maneuver is unrepresentable** (e.g. code that built a partially-invalid
intermediate state and fixed it up later, or relied on shared-ptr aliasing to mutate at a
distance), restructure as locally as possible — observable input→output behavior must be
identical.

**The adjudication log (§11) is mandatory.** Any place where old observable behavior
*cannot* be reproduced (the new core forbids the output, clamps a value the old core passed
through, rejects a file the old api accepted, or vice versa) is not a silent judgment call:
log it in §11 with the decision taken, and update api tests only when the new behavior is
deliberately accepted.

**Why the vertical slice is non-negotiable:** the `MX_API=ON` error wall was measured
(2026-06: 74 translation units fail, every one a `fatal error` on a missing include —
old aggregate/per-element headers and ezxml). Because each TU dies at its first missing
include, the wall is *shallow*: it confirms the file inventory and reveals zero semantic
errors. There is no cheap semantic census; fallout only surfaces after includes and type
spellings are already translated. The slice (work order step 2) is the price-discovery
mechanism for the whole port.

## 3. The `DocumentManager` rewrite

The int-handle/singleton/mutex contract survives. The registry holds
`std::shared_ptr<mx::core::Document>` (wrapping the new value type), which preserves the
`getDocument()` escape hatch byte-for-byte (`DocumentManager.h` itself declares the
`DocumentPtr` alias). ezxml is gone; load/save go through pugixml +
`mx::core::parse`/`mx::core::serialize`.

**Error channel (owner decision): no exceptions escape; `Result`-style signatures; mx::api
owns its error vocabulary** (public headers must not include private `mx::core` headers;
mirroring keeps the layering and lets api errors carry api-level codes core has no business
knowing).

```cpp
namespace mx::api {

enum class ResultCode {
    ioError,                      // file open/read/write failure       (api-level)
    xmlSyntaxError,               // pugixml could not parse the bytes  (api-level)
    unknownElement, unknownAttribute,
    missingRequiredElement, missingRequiredAttribute,
    wrongElementOrder, tooManyElements,
    invalidDocument, unsupportedVersion,   // mirrored from core::ErrorCode
    badDocumentId,                // handle not in the registry         (api-level)
    internalError,                // caught exception; nothing escapes  (api-level)
};

struct ApiError { ResultCode code; std::string path; std::string message; };
template <typename T> class Result;    // ok()/value()/error(), same shape as core's
                                       // Result<void> specialization for writes
}
```

| Method | New signature | Semantics |
|---|---|---|
| `createFromFile` | `Result<int>` | pugixml load (`ioError`/`xmlSyntaxError`) → `core::parse` (core errors mirrored) → handle |
| `createFromStream` | `Result<int>` | same, minus file I/O |
| `createFromScore` | `Result<int>` | CAN now fail: bounded appends (beam > 8) etc. **Refuse, don't drop**: when a `ScoreData` describes something the new core won't represent, propagate the error; silently dropping user data is worse than refusing |
| `writeToFile` | `Result<void>` | `badDocumentId` replaces the old silent no-op; `ioError` on write failure |
| `writeToStream` | `Result<void>` | `badDocumentId` replaces the old silent no-op |
| `getData` | `Result<ScoreData>` | `badDocumentId` replaces the old silent default-`ScoreData` return |
| `destroyDocument` | `void` (unchanged) | idempotent destruction stays a silent no-op — that is a feature |
| `getDocument` | `mx::core::DocumentPtr` (unchanged) | the documented escape hatch; tests depend on it heavily |
| `getUniqueId` | `int` (unchanged) | |

Every public method body is wrapped; any `std::exception` from below becomes
`internalError`. `MX_THROW` disappears from the api layer. The mxl (compressed)
special-case message in old `createFromFile` survives as an `xmlSyntaxError` whose message
keeps the issue-66 hint.

Fallout: every `mxtest/api` caller updates to the new signatures (mechanical), and the
ezxml usage in `ApiTester.cpp`/`PitchDataTest.cpp`/`SlurTest.cpp` is rewritten on pugixml.
The set of parseable files may shift (new core: strict names/structure, lenient values) —
adjudicated per-test in §11.

## 4. Version semantics

- **Write side: the root `version` attribute is always `"4.0"`, unconditionally.** mx does
  not support writing documents at other versions (that would be a much bigger project);
  emitting "3.0" from a 4.0 model was a fiction. `ScoreData::musicXmlVersion` no longer
  influences output. Every test expectation asserting `version="3.0"` or an absent
  attribute updates as adjudicated fallout.
- **Read side: verbatim preservation.** New field
  `std::string ScoreData::declaredMusicXmlVersion` — exactly what the parsed root carried,
  empty if absent. `// TODO: verify that we support the declared version` at the api read
  seam (core already hard-rejects anything newer than 4.0 with `unsupportedVersion`; the
  TODO covers classifying older/malformed declarations).
- The legacy `MusicXmlVersion` enum stays frozen (`unspecified`, `ThreePointZero`) for
  source compatibility; the reader still sets `ThreePointZero` iff the declared string is
  exactly `"3.0"`. Documented as read-side-informational only.

## 5. Timewise ↔ partwise

The old `core::Document::convertContents()` died with the old core; the new core has no
pivot. Port old `ScoreConversions.{h,cpp}` (165 lines at `71fc402`, a shallow-copy regroup
of parts-of-measures ↔ measures-of-parts plus header) into **`mx::impl`**
(`impl/ScoreConversions.{h,cpp}`), translated to value semantics:

- Its only consumer is `DocumentManager`; the new core's charter (generated model + minimal
  hand-written runtime) does not grow hand-written schema-shaped transforms. Promotion to
  core would be a later, separate decision.
- The old mutate-and-restore dance in `getData()` (convert stored doc to partwise, read,
  convert back) dies: convert into a local `ScorePartwise` copy, read that, stored document
  untouched. Same observable behavior (`musicXmlType == "timewise"` round-trips).
- `createFromScore` with `musicXmlType == "timewise"`: build partwise via `ScoreWriter`,
  pivot, store a timewise-rooted `Document`.
- The old pivot's shallow copies become real copies under value semantics — strictly safer,
  behavior-identical for this use.

## 6. `mx::utility` fates

| Header | Fate |
|---|---|
| `Throw.h`, `StringToInt.h`, `Round.h` | keep as-is (live consumers) |
| `Parse.h`, `Enum.h`, `EnumWithString.h`, `JitAllocate.h`, `Lock.h` | delete (dead once `mxtest/import`'s harness goes; zero other refs) |
| `OptionalMembers.h` | rebuild for the new core's shape |

`OptionalMembers.h` re-keying: the old macros SFINAE-detect public data members
(`attr.defaultX` + `attr.hasDefaultX` + `.getValue()`); the new core exposes member
functions returning `std::optional` (`el.defaultX()`, `el.setDefaultX(...)`). Same generic
application algorithm in `PositionFunctions`/`FontFunctions` etc.; the detection idiom
becomes C++20 `requires`-expressions (`if constexpr (requires { el.setDefaultX(v); })`).
`PositionFunctionsTest` (including the compile-only bogus-class case) pins it.

General rule (owner): deleting genuinely dead code that follows from these decisions needs
no further sign-off.

## 7. Test suite fates

### 7.1 `mxtest/api` — preserved in full; THE acceptance suite

All 30 test files + ScoreData headers + harness infra (`mxtest/file`, `mxtest/control`)
come back green. This includes the nine **ApiTester file-roundtrip tests**
(ApiChordSimple, ApiK007a, ApiK007c, ApiK009bSlur, ApiK014aFermatas, ApiK015aLayout,
ApiK016aMisc, ApiLy43e, ApiMuAccidentals1): file → `createFromFile` → `getData` → compare
full `ScoreData` against the hand-written expected values → `createFromScore` → write →
re-load → compare again. **Every file tested that way before is still tested that way.**
Changes are limited to: new `DocumentManager` signatures, ezxml→pugixml in the three files
that touch it, and §11-adjudicated expectation updates.

### 7.2 `mxtest/impl` — port 7, delete 1

| File | Fate |
|---|---|
| `MagicTemplatesTest.cpp` | delete (17 lines, includes only, zero tests) |
| other 7 (CurveFunctions, DirectionReader, DirectionWriter, MeasureWriter, PositionFunctions, PrintFunctions, ScoreWriter) | port mechanically: same assertions, new core spellings |

A ported test that *constructed* a state the new core forbids is adjudicated via §11, not
silently dropped.

### 7.3 `mxtest/import` — old harness deleted; replaced by corert

Owner-confirmed: `ImportTest*`, `ExpectedFiles*`, `ChangeValues*`, `SortAttributes*` are
deleted. Measured fact: that harness round-tripped `XDoc → core::Document::fromXDoc →
toXDoc` — the OLD CORE only; `ScoreData`/impl never appear. corert supersedes it strictly
(DOM comparison, numeric equivalence, sidecar fixups, three languages, pinned 829/0/0).
`Normalize.{h,cpp}` + `DecimalFields.h` stay (corert's). **`data/expected/` is deleted**
(its only reader was the dead harness).

### 7.4 Examples

`mx/examples/` (api-only consumers) compile and run; `examples-run` restored (§9 step 6).

## 8. The corpus-wide api roundtrip (new gate)

The critical test path the old tree never had corpus-wide: **file → api → impl → file**.
Flow per file: load → `createFromFile` → `getData` → `createFromScore` → write → compare
output document to input document, both normalized with corert's pipeline (`Normalize` +
DOM comparison with numeric equivalence).

`mx::api` is deliberately a subset of MusicXML: files using unsupported features cannot
round-trip with fidelity, by design. The gate therefore uses a **pinned pass-list**
(owner decision), with two modes:

- **Regression mode (CI):** run exactly the pinned list; every file must pass; zero
  tolerance. Files off the list still run as load-survival.
- **Discovery mode (manual make target, never a CI gate):** run the whole corpus, report
  which files newly pass/fail — the source of pass-list growth. Additions are deliberate
  commits touching the manifest, never automatic.

**Step 0 — baseline capture (BEFORE any porting; owner requires zero regression vs. the
old library):**

1. Check out old `master` (`71fc402`) in a scratch worktree; build it with its own build
   system (mx-sdk has the toolchain).
2. Run a one-off driver over the corpus: `createFromFile → getData → createFromScore →
   writeToFile` — the old library's honest api roundtrip output per file (recording files
   it cannot load).
3. Judge each output against its input with the NEW normalize-and-compare machinery (the
   same comparator the Phase-2 gate uses — not the old string percent-match).
4. The passing files are the baseline. Commit the manifest
   (`src/private/mxtest/api/roundtrip-baseline.txt`) and the capture driver's source.

**The gate: every baseline file passes the new api roundtrip. Zero regression, hard CI
failure otherwise.** (Old master is a 3.0-era api: files using 3.1/4.0 features mostly land
off-list; the new system passing them later grows the list — progress, not noise.)
Complementary corpus-wide coverage: `ApiLoadSurvivalTest` (already in `mxtest/api`)
continues as the survival check over all files.

## 9. Order of work and gating

Constraint: the `mx` library only links when all 77 impl files compile, so staging is
deliberate. Gating rule (as Phase 1): **nothing advertised flips until step 5 is green.**

- **Step 0 — baseline capture** (§8). First, before porting.
- **Step 1 — the seam:** `mx::api` Result/error types, `DocumentManager` rewrite,
  `impl/ScoreConversions` port. These compile standalone against the new core.
- **Step 2 — the vertical slice:** translate `Converter` (the hub) + the PitchData path
  (`NoteReader`/`NoteWriter` and dependencies), compiled via a scratch target, to validate
  the §2 translation table and surface semantic fallout early.
- **Step 3 — the wave:** mechanically translate the remaining impl files (checklist:
  Appendix A). Library compiles at the end of the wave.
- **Step 4 — suites in order:** `mxtest/impl` (unit pins) → `mxtest/api` (acceptance; §11
  log live) → examples build + run.
- **Step 5 — the corpus roundtrip:** build the harness (both modes), run discovery, verify
  zero regression vs. the step-0 baseline, commit the pinned list.
- **Step 6 — flip the defaults:** `MX_API` default ON; restore `lib`/`dev`/`test`/
  `examples-run` Makefile targets; add CI `linux-api`; extend coverage and macos (§10).

### 9.1 Progress (living section; tick Appendix A per file)

- **Step 0 DONE** (commit `860cce7`): baseline captured from old master 71fc402; the pinned
  manifest is `src/private/mxtest/api/roundtrip-baseline.txt` (exactly 1 file passes the
  strict comparator — see `src/private/mxtest/api/baseline/README.md`). The shared
  normalize/compare machinery lives in `mxtest/corert/Compare.{h,cpp}`; the judge builds
  with `-DMX_BASELINE_JUDGE=on`.
- **Step 1 DONE** (commit `550b48c`): `mx/api/Result.h`, `ScoreData.declaredMusicXmlVersion`,
  the DocumentManager rewrite (write boundary pins version="4.0" on a serialization-time
  copy), `impl/ScoreConversions`, `impl/WriteRefusal.h` (writers throw it for bounded-append
  refusal; `createFromScore` catches).
- **Step 2 DONE** (commit `c1ad11b`): Converter + NoteReader/NoteWriter + the
  OptionalMembers/Position/Font/Print machinery. Slice findings: translation table held;
  Converter tables are now `EnumMap` pair-vectors (wrapper enums have no ordering / no
  public Tag ctor); variant choices bridge by nested `Kind` enums; NoteWriter builds
  values then installs (`assembleNoteChoice`).
- **Steps 3–6 DONE**: ported files are ticked in Appendix A. The scratch target
  `-DMX_IMPL_SLICE=on` (CMakeLists) compiles every ported TU as a static archive — add each
  newly ported .cpp to its list. Canonical ported examples to imitate: `Converter.h/.cpp`,
  `NoteReader.cpp`, `NoteWriter.cpp`, `PositionFunctions.h` + `utility/OptionalMembers.h`,
  `CurveFunctions.h` (generic Slur/Tied handling), `TimeReader.cpp` (old `properties` is the
  new core's `attributes`; old `TimeChoice::timeSignature` is `TimeChoice::Kind::group`),
  `MetronomeReader.cpp` (old `BeatUnitPerOrNoteRelationNoteChoice` is `MetronomeChoice`,
  beatUnitPer=Kind::group, noteRelationNote=Kind::group2). `MX_UNUSED` was rehomed to
  `mx/utility/Unused.h`. Verify each TU with
  `g++ -std=c++20 -fsyntax-only -I src/private -I src/include <file>`; then build
  `cmake -S . -B build/core-dev -DCMAKE_BUILD_TYPE=Debug -DMX_CORE_DEV=on -DMX_IMPL_SLICE=on`
  and keep corert green (`build/core-dev/mxtest-core-dev`, 830 cases). `make fmt`
  (MX_RUNNING_IN_DOCKER=1 natively) before committing.

  **Step-3 translation notes (from porting sessions):**
  - `getAttributes()` is gone — attrs are direct optional members of the element class; pass
    the element itself to `getPositionData<T>()`, `getFontData<T>()`,
    `setAttributesFromMarkData()`, etc.
  - Enum wrappers use factory constructors (`StartStop::stop()`) and `.tag()` comparisons;
    there is no public Tag ctor, so `EnumMap` pair-vectors replace the old switch tables in
    `Converter.cpp`.
  - `DynamicsChoice` is a span of `Empty`/`OtherText` variants (one per dynamic mark), not a
    single-value enum; `Dynamics::choice()` returns `std::span<const DynamicsChoice>`.
  - `FermataShape::emptystring` was renamed to `FermataShape::empty()` in the new core.
  - `TupletPortion` replaces the old `TupletActual`/`TupletNormal` pair;
    `TimeModificationGroup::normalType()` returns `NoteTypeValue` directly (not a wrapped
    element ptr); `TimeModificationGroup::normalDot()` is a span.
  - Credit writing: `FormattedTextID` words → `CreditChoiceGroup` →
    `CreditChoice::group(group)`; reading: `choice().isGroup()` →
    `group.choice().isCreditWords()` → `group.choice().asCreditWords().value()`.
  - ArticulationsChoice, OrnamentsGroupChoice, TechnicalChoice all use `kind()` + `asXxx()`
    dispatch; new 4.0 cases (softAccent, brassBend, flip, smear, open, halfMuted, harmonMute,
    golpe, invertedVerticalTurn, haydn) are stubbed with `return false`.
  - Mutable optional field pattern: `auto x = outer.field().value_or(T{}); modify(x);
    outer.setField(x);` (copy-modify-set, no in-place mutation through optional).

- **Still to port** (Appendix A unticked): NotationsWriter.cpp (its .h is ported),
  DirectionReader/Writer, NoteFunctions, PropertiesWriter, MeasureReader/Writer,
  PartReader/Writer, ScoreReader/Writer (.h files for the last two are ported; ScoreWriter
  port should DROP the old version-echo switch — the write boundary pins "4.0" now, §4).
  Then steps 4-6.

## 10. CI plan

- `linux-gen`, `linux-core`, `linux-targets`: unchanged (Makefile recipes build named
  targets; `MX_API=ON` default does not drag api code into them).
- **New `linux-api`:** build `mx` + `mxtest` through mx-sdk; run the mxtest suites
  (api/impl/file/control); run the corpus roundtrip in regression mode; build and run the
  examples.
- **coverage:** the `--coverage` build now includes `mx` (api/impl/utility); mechanics
  unchanged, table grows rows — satisfies the "coverage naturally grows" acceptance clause.
- **macos:** extends to build `mx` and RUN the api/impl suites natively with AppleClang
  (owner-approved). Same Phase-1 principle: AppleClang fallout is our code, found here,
  fixed in Phase 2.
- Discovery mode stays a manual make target, never a CI gate.

End-of-phase definition of done: the branch tip drives this entire CI matrix green.

## 11. Adjudication log (behavioral changes)

Living section, updated during the port. Every entry: what the old behavior was, why it
cannot or should not be reproduced, the decision, and the tests updated. Pre-registered
entries from this design (details in the sections cited):

| # | Change | Decision | Where |
|---|---|---|---|
| 1 | `createFromFile`/`createFromStream`/`createFromScore` returned `int`, threw on failure | `Result<int>`, no exceptions escape | §3 |
| 2 | `getData`/`writeToFile`/`writeToStream` silently no-opped (or returned default `ScoreData`) on a bad handle | `badDocumentId` errors | §3 |
| 3 | `createFromScore` silently accepted unrepresentable `ScoreData` (e.g. 9 beams) | refuse with error; never drop data silently | §3 |
| 4 | root `version` attribute echoed `ScoreData::musicXmlVersion` (3.0 or omitted) | always `"4.0"` on write | §4 |
| 5 | declared version other than "3.0" was discarded on read | preserved verbatim in `declaredMusicXmlVersion` | §4 |
| 6 | the old `partwiseTimewise` pivot walked a part's measure iterator `currentMeasureIndex` steps with no bounds check: on a ragged score (parts with unequal measure counts) it dereferenced past-the-end — undefined behavior, not a reproducible output | the ported pivot bounds-checks and skips a part that has no measure at that index; behavior identical for every well-formed (equal-measure-count) score | §5, `impl/ScoreConversions.cpp` |
| 7 | `ChordData::miscData` (`vector<MiscData{name,data}>`) was persisted as XML ProcessingInstructions on the `<harmony>` element — a non-standard MusicXML extension that the old `mx::core` bolted on outside the XSD | the new generated core is strictly typed from the MusicXML 4.0 XSD and has no ProcessingInstruction mechanism; `miscData` is accepted on write but not emitted and not read back; `SaveProcessingInstructions` test updated to expect empty round-trip | `DirectionWriter.cpp`, `DirectionReader.cpp`, `ChordDataSaveAndLoadTest.cpp` |

(Subsequent entries appended here as the port discovers them.)

## 12. Deferred to Phase 3: the SMuFL glyph-name migration (open questions)

Evidence gathered during this design session (so it is never re-derived):

- The pre-4.0 workaround lives in the consumer (Komp, `MxNoteWriter::handleMarks`): SMuFL
  canonical glyph names (e.g. `dynamicNiente`, accent-staccato/marcato-tenuto/
  marcato-staccato/tenuto-accent glyphs, `brassMuteClosed`) are smuggled as the TEXT
  CONTENT of `other-articulation`/`other-technical`/`other-dynamics` via
  `MarkData::name` + `MarkType::other*`.
- mx-side breadcrumbs: three `TODO - SMUFLKILL` comments (`impl/Converter.cpp`,
  `impl/OrnamentsFunctions.cpp`, `impl/NotationsWriter.cpp`) sit exactly at the
  `other-*` read/write sites; `MarkType::customAccentTenuto` round-trips through
  `<other-articulation>` with the sentinel string `"gjhsldjhglskdjhfg"`
  (`MarkData.h:193`); `##misc-data##` footnote smuggling (`NoteData.h:123`) is a separate,
  generic channel.
- The proper 4.0 home exists in the new core: the `smufl` attribute (type
  `smufl-glyph-name`) on every `other-*` element.

Open questions for the Phase-3 design session:

1. Add `std::string MarkData::smufl`? (Sketch: read `smufl` attribute into it; write it
   back as the attribute.)
2. Legacy-detection rule for migrating smuggled names: lexical shape vs. a vendored SMuFL
   glyph-name lexicon vs. no auto-promotion (Komp migrates on its own side)?
3. Old-reader compatibility: must new-mx output remain readable by old Komp builds (emit
   both text and attribute), or attribute-only (clean migration, Komp updates in lockstep)?
4. Fate of `customAccentTenuto`/`getMarkTypeFromCustomString` and the `SMUFLKILL` TODOs.

Resolution:

- Exact glyph names live in mark-specific `MarkDataChoice` payloads, not as another common
  `MarkData` field.
- A compound dynamic owns its ordered standard and `other-dynamics` components; neighboring marks
  are never interpreted as one dynamic.
- Text and `smufl` may coexist. mx does not promote legacy text to a SMuFL name automatically.
- The `customAccentTenuto` compatibility path remains unchanged and can be retired separately.

## Appendix A: port checklist

### A.1 `src/private/mx/api/` (4 of 13 .cpp touch core/ezxml)

- [x] `DocumentManager.cpp` (the seam; §3)
- [x] `EncodingData.cpp`
- [x] `MarkData.cpp`
- [x] `SoundID.cpp`
- [x] public headers: add `mx/api/Result.h` (+`ApiError`), `ScoreData.h` version fields (§4)

### A.2 `src/private/mx/impl/` translation units

- [x] AccidentalMarkFunctions.cpp/.h
- [x] ArpeggiateFunctions.cpp/.h
- [x] ArticulationsFunctions.cpp/.h
- [x] Converter.cpp/.h (slice, step 2)
- [x] Cursor.cpp/.h
- [x] CurveFunctions.h
- [x] DirectionReader.cpp/.h
- [x] DirectionWriter.cpp/.h
- [x] DynamicsReader.cpp/.h
- [x] DynamicsWriter.cpp/.h
- [x] EncodingFunctions.cpp/.h
- [x] FermataFunctions.cpp/.h
- [x] FontFunctions.h
- [x] LayoutFunctions.cpp/.h
- [x] LcmGcd.cpp/.h
- [x] LineFunctions.h
- [x] LyricType.cpp/.h
- [x] MarkDataFunctions.h
- [x] MeasureCursor.h
- [x] MeasureReader.cpp/.h
- [x] MeasureWriter.cpp/.h
- [x] MetronomeReader.cpp/.h
- [x] MxVersionDefines.h
- [x] NonArpeggiateFunctions.cpp/.h
- [x] NotationsWriter.cpp/.h
- [x] NoteFunctions.cpp/.h
- [x] NoteReader.cpp/.h (slice, step 2)
- [x] NoteWriter.cpp/.h (slice, step 2)
- [x] OrnamentsFunctions.cpp/.h
- [x] PageTextFunctions.cpp/.h
- [x] PartReader.cpp/.h
- [x] PartWriter.cpp/.h
- [x] PositionFunctions.h
- [x] PrintFunctions.h
- [x] PropertiesWriter.cpp/.h
- [x] ScoreConversions.cpp/.h (NEW, ported from old core; §5)
- [x] ScoreReader.cpp/.h
- [x] ScoreWriter.cpp/.h
- [x] SlideFunctions.cpp/.h
- [x] SpannerFunctions.h
- [x] StaffFunctions.cpp/.h
- [x] TechnicalFunctions.cpp/.h
- [x] TimeReader.cpp/.h
- [x] TupletReader.cpp/.h

### A.3 `src/private/mx/utility/` (§6)

- [x] delete `Parse.h`, `Enum.h`, `EnumWithString.h`, `JitAllocate.h`, `Lock.h`
- [x] rebuild `OptionalMembers.h` (C++20 requires)

### A.4 tests and infra

- [x] delete `mxtest/import/{ImportTest*,ImportTestImpl*,ExpectedFiles*,ChangeValues*,SortAttributes*}`, `data/expected/`
- [x] `mxtest/impl`: delete MagicTemplatesTest.cpp; port the other 7
- [x] `mxtest/api`: signature fallout (all files); ezxml→pugixml in ApiTester.cpp, PitchDataTest.cpp, SlurTest.cpp
- [x] baseline capture driver + `roundtrip-baseline.txt` (step 0)
- [x] corpus api roundtrip harness (discovery + regression modes)
- [x] examples compile/run
- [x] CMake: `MX_API` default ON (step 6); Makefile: restore `lib`/`dev`/`test`/`examples-run`
- [x] CI: `linux-api` job; coverage + macos extension
