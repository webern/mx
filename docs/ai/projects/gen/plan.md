# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Build a generator that produces every C++ class in `mx/core` from `docs/musicxml.xsd` with no
skipped elements.

**Complete** (2026-05-21, iteration 40). SKIP_ELEMENTS and CHOICE_SKIP both empty.

## Milestone 2: fix-gen — fix generator bugs surfaced as failing tests ✅

**Complete** (2026-05-22). All 2678 tests pass.

The commit "src: issues caused by revgen" contained hand-edits to non-generated code (mx/impl,
mx/core/FromXElement, some tests) that worked around generator bugs. Each was triaged as BUG /
BENIGN / WEIRD, generator bugs were fixed, and the workarounds reverted.

One WEIRD item deferred to M5: original `mx/core` had a hand-applied MusicXML 4.0 `UpDownNone`
backport that a schema-faithful 3.x regen overwrites. The `// TODO: fixme - MusicXML 4.0 ...`
comments in `mx/impl` (NotationsWriter.cpp:398, ArpeggiateFunctions.cpp:35) bookmark this.

## Milestone 3: fix-core-dev - fix bugs surfaced by new core-dev test mode ✅

**Complete** (2026-05-22). All gates green. Final iteration (5) fixed the `slash`/`beat-repeat`
slash-type emission bug — the writer unconditionally emitted `<slash-type>eighth</slash-type>` even
though the XSD `slash` group is `minOccurs="0"`. Removed two `CHILD_MIN_OCCURS_OVERRIDE` entries
in gen, regenerated `Slash.{h,cpp}` and `BeatRepeat.{h,cpp}` with a `myHasSlashType` flag, removed
a matching `addChildIfNone` workaround in the api-import test harness, and added 6 surgical
`setHasSlashType(true)` calls to 4 `mxtest/core/*Test.cpp` fixture files. Commits d43a222c and
9c8efa24.

### Session sequence (kept for reference)

Session sequence:
- `rm -rf data/testOutput/*`
- run `make test-core-dev` : record the number of falures as baseline_core_dev
- run `make test` : record the number of failures as baseline_test (should be zero)
- diff each before/after pair in `data/testOutput/corert` and choose the one with the smallest diff.
  This becomes test_to_fix
- analyze test_to_fix and try to decide, is this a bug in `gen/*`? For context, bugs in my original
  hand-rolled gen efforts were dutifly replicated by revgen. So, for example `lang="it"` as default
  is not a sensible behavior for the library. It is a bug that preexisted before revgen. There are
  other potential cases, for example defaulting to having an element present when minOcurrs=0 in the
  spec.
  - If you think this is a pre-existing bug reproduced by revgen, it might be worth checking with
    the user to save us all time and tokens, unless you're really sure that a MusicXML file
    shouldn't be the way that we are treating it right now
  - If you think this is just a bug in the way revgen decided what to do, that's a no-brainer, it
    should be fixed
- When the bug is fixed
  - run `gen` to regenerate mx/core
  - `make fmt`
  - `make check`
  - `rm -rf data/testOutput/*`
  - `make test-core-dev` : you must see that you fixed the test you were trying to fix without introducing any new failures
  - `make test` : should still be at zero failures
- Commit your work
- update state.md and any tracking documents you decide to use.
- report done

## Milestone 4: increase test coverage

The dedicated mx/core-level round-trip harness now exists (the corert suite under
`src/private/mxtest/corert/`, exercised by `make test-core-dev` and as part of `make test-all`),
built out during M3. The harness side of the original M4 statement is therefore done.

What remains: add a lot more MusicXML round-trip input files. Public test suites, hand-curated
edge cases, or generated inputs targeting specific spec features not yet covered. No specific
design yet — first M4 session should scope this with the user.

## Milestone 5: better-gen — fix garbage

The gen program is 12k lines of bad Python. Fix it. Use dedicated mx/core round-trip tests as the
north star for correctness.

## Milestone 6: mxml4-types — generate MusicXML 4.0 types

Replace `docs/musicxml.xsd` with MusicXML 4.0, regenerate, fix all existing tests. Watch for
backported or bolted-on features (SMuFL, UpDown, etc.) that were added with hacks to 3.0/3.1 but are
first-class in 4.0. Be backward-compatible with files `mx` may have written using those hacks.
Restore the `mx/impl` TODOs from revgen.
