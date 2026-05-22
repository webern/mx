# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Complete 2026-05-21. Generator produces every C++ class in `mx/core` from
`docs/musicxml.xsd`. `SKIP_ELEMENTS` and `CHOICE_SKIP` empty. See `log.md`.

## Milestone 2: fix-gen — fix generator bugs surfaced by failing tests ✅

Complete 2026-05-22. `make test-all` 3028/3028. All fixes landed in the generator;
workarounds reverted. One item deferred to M5/M6: HEAD's hand-applied MusicXML 4.0
`UpDownNone` backport vs. schema-faithful 3.x regen (TODOs in
`mx/impl/NotationsWriter.cpp:398`, `mx/impl/ArpeggiateFunctions.cpp:35`).

## Milestone 3: fix-core-dev — fix bugs surfaced by new core-dev test mode ✅

Complete 2026-05-22 (5 iterations). 31 corert failures triaged into 10 `{file}.invalid`
markers and 4 real fixes. The `src/private/mxtest/corert/` harness is now part of
`make test-all`. See `log.md` M3 for per-iteration breakdown.

### Reusable corert-iteration template (for M4)

1. `rm -rf data/testOutput/*`
2. `make test-core-dev` — record `baseline_core_dev`.
3. `make test` — record `baseline_test` (should be zero).
4. Diff each before/after pair in `data/testOutput/corert`; pick the smallest. That's
   `test_to_fix`. One test per session.
5. Analyze: pre-existing hand-rolled bug replicated by revgen? schema-invalid input?
   library bug in a hand-written type? Flag pre-existing-bug calls to the user.
6. Fix. If gen changed, regen mx/core. `make fmt && make check`.
7. `rm -rf data/testOutput/*`; `make test-core-dev` must show the fixed test green and
   no new failures; `make test` must stay at zero.
8. `make test-all` if the change touched `src/private/mx/core/*`.
9. Commit. Update `state.md` and other tracking.

## Milestone 4: increase test coverage

The corert harness is in place; what's missing is more MusicXML round-trip input
files. Sources to consider — public test suites, hand-curated edge cases targeting
spec features not yet exercised, generated/fuzzed inputs. No design yet. First M4
session scopes this with the user.

## Milestone 5: better-gen — fix garbage

The generator is ~14k lines of bad Python. Refactor. Use the corert suite as the
correctness oracle.

## Milestone 6: mxml4-types — generate MusicXML 4.0 types

Replace `docs/musicxml.xsd` with MusicXML 4.0, regenerate, fix all existing tests.
Watch for backported / bolted-on features (SMuFL, `UpDown`, …) that were added with
hacks to 3.0/3.1 but are first-class in 4.0. Be backward-compatible with files mx
may have written using those hacks. Restore the `mx/impl` TODOs left from revgen.
