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

## M4a: test fixer (2026-05-22 -- 2026-05-25) ✅

Built a `Fixer` that patches the expected tree before comparison via per-file
`<file>.fixup.xml` sidecars. This lets corert handle cases where mx's clamping or
defaulting differs from the raw input without marking the file `.invalid`. Convention
documented in `data/README.md`, design doc at `src/private/mxtest/corert/Fixer.h`.
Closed at 387/387 `test-core-dev` pass and 3065/3065 `test-all`.

## M5: test coverage expansion (2026-05-25 -- 2026-05-30) ✅

Added real-world corpus files and generated 235 synthetic MusicXML files
(`data/synthetic/`) to achieve 100% symbol coverage of MusicXML 3.0/3.1/4.0 spec
symbols. Final three corert failures fixed: PlaybackSound "other" variant
(PlaybackSoundType wrapper class), xmlns:xlink preservation (XMLNS_PRESERVING_ATTRS
config in generator). Filed GitHub issue #161 for namespace-prefix limitation.

Final state: `make test-core-dev` 676/676, `make test-all` all pass.

## M6A: gen-quality tooling (2026-06-01) ✅

Designed the scoring methodology with the user via a grill, then built the tooling. Decisions, in
order: dropped maintainability-index (step-shaped + redundant with CC/Halstead/LOC) and Halstead
(size-proxy, redundant once a real size axis exists); excluded duplication/coupling/cohesion/DIT
after measuring that jscpd and pylint both report ~0% duplication on this f-string-heavy emission
code (the dupes are semantic, not token-identical, so any detector misreads them as clean); moved
pylint out of the score into a separate `make gen-lint` binary gate. Final rubric: structure 50%
(LOC-weighted function + file size), cyclomatic 25%, cognitive 25%, all via one smooth
`target/max(target,value)` transform so partial refactor wins register and tiny stub functions
cannot game the size axis.

Implemented: rewrote `gen/quality.py` (scores every `gen/*.py` except itself; writes
`data/testOutput/gen-quality/score.json` with 30 offenders/axis as `path:line` refs, plus
`report.md` and stdout); added `gen/.pylintrc` (disables the complexity checks gen-quality scores);
added a pinned analyzer venv to the Dockerfile; added `make gen-quality` / `make gen-lint` with bash
floor gates; wired both into CI `linux-gate` with a job-summary line and a per-push PR comment.
Deleted dead `gen/eval.py`, `gen/eval_config.yaml`, and the old `gen/quality-baseline.json`.

Floors are a ratchet, set just under the measured in-container baseline (deterministic, identical to
local): `GEN_QUALITY_FLOOR=37.7` (composite 37.7 = structure 20.1, cyclomatic 62.8, cognitive 47.9),
`GEN_LINT_FLOOR=9.4` (pylint 9.49). Generator behavior was off-limits by user direction, so this is
a tooling-only change; the dead `OVERWRITE_FILE_STEMS` set in `generate.py` was left untouched.
