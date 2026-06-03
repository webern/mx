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

### 6C_CONFIG_FILE (next: not started)

Further refactor the gen program so that it reads a toml config file instead of embedding all of the
bespoke decisions into the python code itself. This requires an excellent design. I want this config
file to be extensible for future code gen use cases (e.g. Rust, Go, etc. and even perhaps generating
a new specification that improves upon MusicXML).

Areas to consider during the design phase:

- certain choices I made in the handling of MusicXML might be considered canonically correct, we
  should see if any enshrined XSD deviations should be hard-coded and if so whether they could be
  present in the contexts layer automatically without configuration.

- the configuration layer should probably enrich the contexts, or is that the right hook point

- what things will be needed for different use cases, how different can a configuration look and why

### 6D_TEMPLATES

Refactor out the "f-strings" from python. Use a proper template library and move the C++ boilerplate
to template files that are rendered by the generator.

### 6E_STAND_BACK

Likely multi-session

How good is our design. Let's have an architect look at it through the lense of supporting future
use cases such as generating code to a different language or generating a new spec inspired by the
MusicXML spec. For example, let's imagine we want to restructure MusicXML significantly to be easier
to use and write that new spec as a JSON spec. What needs to be done to make our generator
extensible in the future (even if we don't add those extensions now, how does the current design break).

Are there oddities in `mx/core`'s codegen that we could removed to get a cleaner generator design?

Try it out with MusicXML 4.0 temporarily. Where did it break. Is it a design problem?

Write a design doc better_generator.md

## Milestone 7: mxml4-types — generate MusicXML 4.0 types

Replace `docs/musicxml.xsd` with MusicXML 4.0, regenerate, fix all existing tests. Watch for
backported / bolted-on features (SMuFL, `UpDown`, …) that were added with hacks to 3.0/3.1 but are
first-class in 4.0. Be backward-compatible with files mx may have written using those hacks. Restore
the `mx/impl` TODOs left from revgen.

## Milestone 8: Surface MusicXML 4.0 features in mx/api

