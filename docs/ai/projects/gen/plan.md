# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Complete 2026-05-21. Generator produces every C++ class in `mx/core` from `docs/musicxml.xsd`.
`SKIP_ELEMENTS` and `CHOICE_SKIP` empty. See `log.md`.

## Milestone 2: fix-gen — fix generator bugs surfaced by failing tests ✅

Complete 2026-05-22. `make test-all` 3028/3028. All fixes landed in the generator; workarounds
reverted. One item deferred to M5/M6: HEAD's hand-applied MusicXML 4.0 `UpDownNone` backport vs.
schema-faithful 3.x regen (TODOs in `mx/impl/NotationsWriter.cpp:398`,
`mx/impl/ArpeggiateFunctions.cpp:35`).

## Milestone 3: fix-core-dev — fix bugs surfaced by new core-dev test mode ✅

Complete 2026-05-22 (5 iterations). 31 corert failures triaged into 10 `{file}.invalid` markers and
4 real fixes. The `src/private/mxtest/corert/` harness is now part of `make test-all`. See `log.md`
M3 for per-iteration breakdown.

## Milestone 4: test fixer (clamping-aware comparison) ✅

Complete 2026-05-25. A new `Fixer` patches the expected tree before comparison via per-file
`<file>.fixup.xml` sidecars. Convention documented in `data/README.md`. Design doc at
`src/private/mxtest/corert/Fixer.h`. Closed at 387/387 `test-core-dev` pass and 3065/3065
`test-all`.

## Milestone 5: increase test coverage ✅

Completed 2026-05-30. Added real-world corpus files. Generated 235 synthetic files
(`data/synthetic/`).

## Milestone 6: better-gen — fix garbage

The generator is `~14k` lines of bad Python. Refactor. Use the corert suite as the correctness
oracle.

Sub-milestones:

### 6A_QUAL_GATES: research code quality analysis tools for python. ✅

Delivered 2026-06-01: `make gen-quality` and `make gen-lint`

### 6B_DATA_MODEL: ✅

Refactor edthe generator into a `parse -> configure -> render` pipeline - see
`design/m6b-data-model.md`.

### 6C_NEXT_AND_BEYOND

Definition of the next steps is intentionally left TBD depending on the output ob 6B. I don't want
to burdon the LLM with where we are going next.

## Milestone 7: mxml4-types — generate MusicXML 4.1 types

Replace `docs/musicxml.xsd` with MusicXML 4.1, regenerate, fix all existing tests. Watch for
backported / bolted-on features (SMuFL, `UpDown`, …) that were added with hacks to 3.0/3.1 but are
first-class in 4.0. Be backward-compatible with files mx may have written using those hacks. Restore
the `mx/impl` TODOs left from revgen.

## Milestone 8: Surface MusicXML 4.1 features in mx/api

