# Step 1: Test Strategy

Decide how the new `mx::api` feature will be tested before you write data-model or impl code. The
end state of this step is concrete red/green tests in the repo that exercise the feature through the
api. This file explains the testing machinery, then gives an ordered procedure.

## Background

`mx::api` is a deliberate subset of MusicXML. Most real corpus files lose fidelity on an api
round-trip by design (the api drops or reorders `part-group`, `credit`, most of `defaults`,
`footnote`/`level`, etc.). Keep that reality in mind: it is why corpus-level testing rarely grows
and why unit tests carry most of the weight.

There are two kinds of api tests.

### 1. Unit tests (the workhorse)

- Location: `src/private/mxtest/api/` (e.g. `NoteDataTest.cpp`, `DirectionDataTest.cpp`,
  `MeasureDataTest.cpp`, `PitchDataTest.cpp`, `MetronomeApiTest.cpp`).
- They compile into the `mxtest` binary and run via `make test`.
- Every file is gated by `#include "mxtest/control/CompileControl.h"` then
  `#ifdef MX_COMPILE_API_TESTS ... #endif`. That flag is defined in
  `src/private/mxtest/control/CompileControl.h` (currently always on). Wrap any new test file the
  same way.
- The harness is `cpul` (a Catch2 wrapper). A test is `TEST(caseName, GroupName) { ... } T_END;`
  from `src/private/cpul/cpulVsTest.h`. Assertions: `CHECK`, `CHECK_EQUAL(expected, actual)`,
  `REQUIRE(...)`, `CHECK_DOUBLES_EQUAL(a, b, eps)`. For floating point use `MX_API_EQUALITY_EPSILON`
  (`src/include/mx/api/ApiCommon.h`).
- New `.cpp` files are picked up automatically: `CMakeLists.txt` globs
  `src/private/mxtest/api/*.cpp` into `mxtest` (it excludes only `CorpusRoundtripMain.cpp`, which
  has its own `main`). No registration list to edit.

Round-trip helpers (this is the canonical pattern -- build a `ScoreData` in memory, push it through
api -> core -> XML -> api, assert the fields survived):

- `src/private/mxtest/api/RoundTrip.h`: `mxtest::roundTrip(ScoreData)` returns the round-tripped
  `ScoreData` (createFromScore -> writeToStream -> createFromStream -> getData).
- `src/private/mxtest/api/TestHelpers.h`: `mxtest::toXml(ScoreData)` returns the serialized XML
  string (use it to assert exact XML element/attribute output), and `mxtest::fromXml(string)` parses
  XML back to `ScoreData`.

Anatomy of a typical unit test (see `MeasureDataTest.cpp` `forwardRepeat`, `PitchDataTest.cpp`):

1. Construct a `ScoreData` and grow the tree by hand: `score.parts.emplace_back();` ->
   `parts.back().measures.emplace_back();` -> `measure.staves.emplace_back();` -> notes go on a
   voice via `staff.voices[0].notes.emplace_back();` (voices is a map keyed by voice number);
   directions/tempos go on `staff.directions`, barlines on `measure.barlines`. Set
   `score.ticksPerQuarter` when timing matters.
2. Set the field(s) the feature adds on the relevant struct.
3. Round-trip: `auto out = mxtest::roundTrip(score);` (or `toXml`/`fromXml` to also assert exact
   serialized text, as `staffLinesRoundTrip` and `PitchDataTest` do).
4. Walk back down the output tree (`out.parts.back().measures.back()...`) and assert the field
   survived with `CHECK`/`CHECK_EQUAL`.

There is also a file-based unit pattern (`ApiTester` + an in-code `ScoreData` builder, e.g.
`ApiK016aMiscTest.cpp` + `ApiK016aMiscScoreData.h`): load a corpus file, build the expected
`ScoreData` in code, and assert `expectedScoreData == actualScoreData`. Heavier; prefer the
`roundTrip` style for a single feature unless you are mirroring an existing file fixture.

### 2. Corpus roundtrip gate (rarely grows)

`src/private/mxtest/api/CorpusRoundtripMain.cpp` builds a *separate* binary `mxtest-api-roundtrip`
with two modes. Both do: `createFromFile -> getData -> createFromScore -> writeToStream`, then strip
whitespace, strip the mx `<software>` provenance stamp, apply `.fixup.xml` fixups to the expected
side, and run a strict DOM compare (`mxtest/corert/Compare`). Zero tolerance on the gate.

- `regression <dataRoot> <baselineFile>`: runs only the files listed in
  `src/private/mxtest/api/roundtrip-baseline.txt`. Non-zero exit on any failure. This is the CI
  gate. Run with `make test-api-roundtrip`. CI runs it in `.github/workflows/ci.yaml` (the
  `linux-api` job step "Run corpus api roundtrip (regression mode)" and again in the docker job).
- `discovery <dataRoot>`: walks the whole corpus (excludes `expected/`, `testOutput/`,
  `generalxml/`, `smufl/`, `*.fixup.xml`, and any file with a sibling `*.invalid` marker), prints
  one tab-separated line per file
  `PASS|FAIL|SKIP|LOADFAIL|GETDATAFAIL|CREATEFAIL <tab> relpath <tab> detail`, and always exits 0.
  Run with `make discover-api-roundtrip`. Never a gate.

`roundtrip-baseline.txt` currently pins exactly one file (`ksuite/k016a_Miscellaneous_Fields.xml`).
Read its header comment: of 829 discovered files, only one survives the strict DOM compare, because
the api is a subset. The baseline grows only by deliberate manual commits, never automatically.

### 3. Synthetic corpus

`data/synthetic/` holds `~390` files (see `data/README.md`) named `<element>.<musicxml-version>.xml`
(version = the schema the construct first appears in; root `version` stays `3.0`). Each isolates one
element/attribute set for symbol coverage. But they are dense: every file wraps the construct in
`part-group` + `footnote` + `level` scaffolding that the api drops, so a synthetic file almost
always fails the api round-trip by design and is not a drop-in baseline candidate. Treat synthetic
files as references for valid MusicXML shape, and as `fromXml(...)` *input* fixtures in a unit test
(load the file's XML, assert the api surfaced the field) -- not as new `roundtrip-baseline.txt`
entries unless the file genuinely survives the strict compare.

## Build note

`make test-api-roundtrip` and `make discover-api-roundtrip` both depend on the `dev` target, which
builds `mxtest-api-roundtrip`. `make test` builds and runs `mxtest` (the unit tests) plus examples.
The procedure below runs these as it goes: discovery before you implement (step 2), then `make test`
and the quality gates afterward (steps 8-10).

## Procedure

Do these in order. The discovery snapshot must be captured before you implement.

1. Pin down the feature: the exact `mx::api` struct/field (or new struct) that will represent it,
   and the MusicXML element/attribute it maps to. Find an existing synthetic file for that element
   under `data/synthetic/` (e.g. `ls data/synthetic | grep <element>`); read it to learn the valid
   MusicXML shape and attribute set.

2. Capture the pre-implementation discovery baseline:
   `make discover-api-roundtrip > /tmp/api-discovery-before.txt`. This is your reference for which
   corpus files fail and WHY (the `detail` column). Keep it.

3. Grep that snapshot for files whose failure looks attributable to the missing feature -- e.g.
   `grep <element-name> /tmp/api-discovery-before.txt`, and inspect the `detail` strings of nearby
   `FAIL` lines. Note any candidate files; you will re-check them in step 9. Expect that many
   features have zero candidates, because other subset gaps in the same file also break the strict
   compare.

4. Write a failing unit test first (red). Create `src/private/mxtest/api/<Feature>ApiTest.cpp`,
   gated with `#include "mxtest/control/CompileControl.h"` + `#ifdef MX_COMPILE_API_TESTS`. Model it
   on `MeasureDataTest.cpp` or `PitchDataTest.cpp`: build a minimal `ScoreData`, set the new field,
   `mxtest::roundTrip(score)` (and/or `mxtest::toXml`/`fromXml` to assert exact serialized text),
   then assert the field survived. It will not compile or will fail until the field exists -- that
   is the intended red state. (You may park it behind an `#if 0` only if a non-compiling test blocks
   unrelated work; remove the guard before step 8.)

5. If a synthetic file isolates the feature, add a second unit test that uses it as input: read the
   file's XML, `mxtest::fromXml(xml)`, and assert the api exposes the new field. This pins the core
   -> api read path. (Optional but recommended when a clean synthetic file exists.)

6. If no suitable synthetic file exists and the feature warrants it, add a minimal new synthetic
   file `data/synthetic/<element>.<version>.xml` that exercises exactly this feature, following the
   naming and shape rules in `data/README.md`. Keep it minimal. This is optional; prefer the
   in-memory `roundTrip` test as the primary gate and only add a synthetic file when an on-disk XML
   fixture adds real value.

7. Implement the feature (skill steps 2-3: data model in `mx::api`, wiring in `mx::impl`).

8. Make the unit test pass (green): `make test` (runs `mxtest`). Iterate until your new `TEST(...)`
   cases pass. Run `make fmt` and `make check` so CI gates stay green.

9. Re-run discovery and diff: `make discover-api-roundtrip > /tmp/api-discovery-after.txt`, then
   `diff /tmp/api-discovery-before.txt /tmp/api-discovery-after.txt`. Look for files that moved to
   `PASS` (or whose `FAIL` detail changed). For each newly-PASSing file, confirm the pass is
   actually attributable to your feature (re-read the file; check the old `detail` named your
   element), not an unrelated side effect.

10. Grow the baseline only if vetted: for each newly-PASSing, attributable file, add its
    `data/`-relative path to `src/private/mxtest/api/roundtrip-baseline.txt` with a short comment
    explaining why it now passes. Then `make test-api-roundtrip` to confirm the full pinned list
    (zero tolerance) still passes. Commit this deliberately.

11. If discovery yielded nothing (common), do not force a baseline entry. The unit test(s) from
    steps 4-5 (plus any synthetic fixture) are the feature's regression coverage. Confirm the end
    state: at least one red-turned-green `TEST(...)` in `src/private/mxtest/api/` that exercises the
    new field through the api, passing under `make test`.
