# gen Log

Append chronologically, oldest on top.

## M1: revgen (2026-05-18 — 2026-05-21, 40 iterations) ✅

Reverse-engineered the codegen, iteratively shrinking `SKIP_ELEMENTS` and `CHOICE_SKIP` to
empty. Closed with the generator producing every C++ class in `mx/core`. Tests still
failing; commit `d4f25ee6` "src: issues caused by revgen" carries hand-edits to
non-generated consumers that kept the build working — the input to M2.

## M2: fix-gen (2026-05-21 — 2026-05-22) ✅

Triaged `d4f25ee6` into 6 root causes (A–F) and ~129 `make test-all` failures into clusters
(R1–R7, D1–D4, plus an isFirst-separator family). Fixed iteratively to 0 failures.

Notable mechanisms introduced (still in `gen/generate.py`):
- `EXTENSION_OPTIONAL_GROUP_RENAME`, `SUPPRESS_GROUP_SUFFIX`,
  `WRAPPER_AS_ELEMENT_SYNTH_GROUPS` (MetronomeTuplet group flattening, issue C).
- `TREE_ELEMENT_CONFIG["key"] = {"parent_imports_choice_groups": True}` →
  `Key::import{Traditional,NonTraditional}Key` (issues E/F).
- Required-set seeding rule in `generate_element_cpp`
  (`min_occurs>=1, max_occurs!=1, !is_group`) fixed HarpPedals SIGSEGV (R5).
- `is_container` + `trigger_names` on `TreeChoiceBranch`; `importContainer<X>` dispatch
  in `generate_tree_parent_cpp` (D1, D4 — Metronome reader).
- `ATTR_DEFAULT_OVERRIDE` (~17 entries), `CHILD_INIT_VALUE_OVERRIDE` (Scaling,
  StaffDetails).
- `_emit_direction_family` bespoke driven by
  `model.complex_types["direction-type"].content_tree`.
- `ELEMENT_HAS_CONTENTS_ALWAYS_TRUE` (MeasureLayout, NoteheadText), notehead-text
  `seed_choice_set`, MeasureLayout explicit child-presence check for `isOneLineOnly`.

Deferred: issue A — original `mx/core` had a hand-applied MusicXML 4.0 `UpDownNone`
backport overwritten by schema-faithful 3.x regen. TODO comments in
`mx/impl/NotationsWriter.cpp:398` and `mx/impl/ArpeggiateFunctions.cpp:35`. Belongs to
M5/M6.

### Lessons captured (operational invariants)

- `git checkout -- src/private/mx/core/` preserves mtimes; incremental cmake then links
  partly-old `.o` files and reports stale test counts. Use `make clean && make test-all`
  for authoritative measurements.
- `make test-all` must run with generated files present — HEAD's `UpDownNone` backport
  is incompatible with a schema-faithful regen, so a reset-first build will not compile.
- When removing a previously-emitted byte from a shared template, survey the whole HEAD
  population that template emits (R2 burned us by regressing DirectionType).
- Bespoke handlers should still read the parsed XSD model — "custom algorithm,
  schema-driven data".

## M3: fix-core-dev (2026-05-22, 5 iterations) ✅

Each iteration picked the smallest core-roundtrip diff in `data/testOutput/corert` and
triaged it: either a hand-rolled mx-side fix or a `{file}.invalid` marker for files that
don't conform to the XSD. Per-iteration template now lives in `plan.md`.

- **i1** — tenths-typed `width` attribute trailing-zero strip. Added `"width"` to
  `decimalFields` in `src/private/mxtest/import/DecimalFields.h`. Cleared 17 failures.
  Commit `639d46a3`.
- **i2** — `musuite/testInvalid.xml` is intentionally invalid; introduced the
  `{file}.invalid` marker convention (honored by `CoreRoundtripImpl.cpp::discoverInputFiles`;
  api import keeps processing). Documented in `data/README.md`.
- **i3 (static-analysis sweep)** — ran `xmllint --schema` against MusicXML XSD on
  remaining failing files; marked 10 as `.invalid` where the schema violation explained
  the diff. Three real bugs survived: ly22b, ly41e, ly45f.
- **i3 (cont.)** — ly45f: `CommaSeparatedListOfPositiveIntegers::parse` in hand-written
  `src/private/mx/core/CommaSeparatedPositiveIntegers.cpp` discarded "1, 2, 3" vs
  "1,2,3" spacing on import. Added a `", "` detection that sets `myIsSpacingDesired`.
  Commit `461b96d2`.
- **i4** — ly41e: `XsString::toStream` escaped only `<`, `>`, `&`. A raw `\r` was
  normalized to `\n` on the next read (pugixml `parse_eol`, XML 1.0 §2.11). Added
  `'\r'` → `"&#xd;"`. Hand-written type, no regen. Commit `040b2152`.
- **i5** — ly22b: XSD `slash` group is `minOccurs="0"` inside complexType `slash` and
  `beat-repeat`, so empty `<slash/>` is valid. HEAD treated `slash-type` as
  always-present; revgen preserved this via `CHILD_MIN_OCCURS_OVERRIDE`. Removed the
  overrides, regenerated `Slash.{h,cpp}` and `BeatRepeat.{h,cpp}` with a `myHasSlashType`
  flag, removed a matching `addChildIfNone` workaround in
  `src/private/mxtest/import/ExpectedFiles.cpp`. `make test-all` then surfaced 25
  assertions in 23 `mxtest/core/*Test.cpp` cases that codified the bug; added 6
  `setHasSlashType(true)` calls across 4 fixture files. Commits `d43a222c`, `9c8efa24`.

Final state: `make test-core-dev` 350/350, `make test` 2717/2717, `make test-all`
3028/3028 (9914 assertions), `make check` passed. PR-merge commit `6c4e18d4`.
