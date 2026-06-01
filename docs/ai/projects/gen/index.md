---
created: 2026-05-18
github_tracking_issue: 58
github_sub_issues:
  milestone_4: 156
  milestone_5: 156
  milestone_6: 157
  milestone_7: 158
  milestone_8: 159
completion_dates:
  milestone_1: 2026-05-21
  milestone_2: 2026-05-22
  milestone_3: 2026-05-22
  milestone_4: 2026-05-25
  milestone_5: 2026-05-30
  milestone_6a: 2026-06-01
---
# gen

## Goal

Reverse engineer the codegen process that produced `mx/core` from MusicXML XSD. Build a generator
that re-produces the existing C++ code from `docs/musicxml.xsd`, improve testing and coverage,
improve the generator, then point it at MusicXML 4.1 to generate updated types.

## Files

Standard project layout (see the `/project` skill).

Found Files: Music XML files under consideration for adding to the test suite(s). We need to be
cognizent of the declared MusicXML version in the file. Before we implement 4.0, then we should only
add files at 3.0 and lower to the test suite. After we implement 4.0 we want to add the rest.

- `./found-files/musetrainer/version-3.1-or-later/`: 38 musetrainer files that are MusicXML 3.1+.
  These are intentionally kept under version control. They will be moved into the test corpus
  (`data/`) once M7 (mxml4-types) is complete and `mx` targets MusicXML 4.0.
- `./found-files/`: any other batches under source control are considered temporary and will be
  deleted when the project is finally completed.

When a file is determined to be valuable to `mx` for testing, it should be moved to `data/` under a
TBD subdirectory.

## Synthetic coverage files

To exercise spec symbols absent from the corpus, we synthesized small, schema-valid MusicXML files,
one per gap. The MusicXML 3.0 batch lives in `data/synthetic/` (read by `corert`); batches for
versions newer than 3.0 are staged under `synthetic-files/` (e.g. `3.1/`, `4.0/`) since `mx` does not
yet target them. The methodology and file-naming convention are documented in
`data/synthetic/README.md`; the plan and results are in `synthetic-plan.md`. Generation and analysis
tooling is throw-away and lives in `coverage/` (delete before PR).

## Repo conventions introduced by this project

- `src/private/mxtest/corert/` introduces round trip tests that are not sensitive to symbol changes
  in `mx/core`. The purpose is so that codegen can be changed without worrying about the impact in
  `mx/impl` and `mx/api` layers. In theory, as long as our deserialization, serialization, and
  accessors are correct, then it should be trivial for an LLM retrofit `mx/impl` after innovating in
  `mx/core`.

- The core-roundtrip harness (built out during M2/M3) is part of `make test-all` and is the daily
  driver via `make test-core-dev`.

- `{file}.invalid` marker (sibling file, human-readable body) marks a MusicXML input that is not
  XSD-valid. Documented in `data/README.md`. Introduced in M3.

- `{file}.fixup.xml` sidecar (sibling file, XML) patches the expected tree before test comparison.
  Documented in `data/README.md`. Introduced in M4a.

## Generator location and how to run

The generator lives at `gen/` (not under this project directory):

```
python3 gen/generate.py          # regenerates C++ into src/private/mx/core/elements/
```

Workflow: `python3 gen/generate.py && make fmt && make test-all`. To reset:
`git checkout -- src/private/mx/core/ && git clean -fd src/private/mx/core/`.

## Generator quality gates (M6A, the better-gen regression detector)

Two Docker gates score the generator's own Python so the M6 refactor can proceed without
regressing. Both run in the `mx-sdk` image (pinned analyzers in `/opt/quality-venv`) and in CI
(`linux-gate`).

- `make gen-quality` — composite design score 0-100 (higher = better) from `gen/quality.py`.
  Rubric: structure 50% (LOC-weighted function + file size), cyclomatic 25%, cognitive 25%; one
  smooth transform `target/max(target,value)` per axis. Writes `data/testOutput/gen-quality/`
  (`score.json` for agents/CI, `report.md` for humans) with the worst 30 offenders per axis as
  `path:line` refs. Fails below `GEN_QUALITY_FLOOR` (Makefile).
- `make gen-lint` — pylint as a binary gate (`gen/.pylintrc` disables the complexity checks that
  `gen-quality` already scores). Fails below `GEN_LINT_FLOOR` (Makefile).

Both floors are a ratchet: set just under the current score so CI passes "just barely," then raised
as the generator improves. Current: `GEN_QUALITY_FLOOR=37.7`, `GEN_LINT_FLOOR=9.4`.

## Fitness function and gates

- `make test-core-dev` is authoritative. This is what tells you if `mx/core` is ok or broken.
- `make test-all` is needed when wiring `mx/core` up through the other layers. (Very slow)

`make test-all` pass/fail is authoritative. **Use `make test-all`, never `make test`** — the latter
builds `dev` and skips the slow `mxtest/file/` round-trippers. Full gate:
`make fmt && make check && make test-all`. Every iteration that touches `src/private/mx/core/*` ends
with a recorded `make test-all` result.

## Cardinal rules

- Never change test cases. Test harness code is fair game with user authorization.
- Never change `mx/api`.
- Minimize changes to `mx/impl`; prefer fixing the generator.
- Do not autonomously change the `GEN_QUALITY_FLOOR` / `GEN_LINT_FLOOR` ratchets except to raise
  them after a real improvement.
- Reset generated `mx/core/` before commit if a generator change was tried and reverted.

## Bespoke generator handlers

Six bespoke handlers in `BESPOKE_ELEMENTS` (`gen/generate.py`): credit, lyric, part-list, harmony,
score-wrapper-family, note, direction. Acceptable when an element's shape cannot be expressed by the
shared rule-based path, but they must still read the parsed XSD model so spec changes propagate.
Pattern: "custom algorithm, schema-driven data" — not a hardcoded string dump.

Always prefer extending a reusable rule-based path (`TREE_ELEMENTS`, `TREE_ELEMENT_CONFIG` flags,
shared helpers) over a new bespoke handler. If a pattern could plausibly recur for another element,
the fix belongs in the shared path with a config-driven flag.

## Key external files

- `gen/generate.py` — the generator (~14k lines of Python)
- `gen/quality.py` — design-quality scorer for `make gen-quality` (excluded from its own score)
- `gen/.pylintrc` — pylint config for `make gen-lint`
- `docs/musicxml.xsd` — input schema (currently MusicXML 3.0; swap to 4.1 in M6)
- `src/private/mx/core/elements/` — target output (~590 .h/.cpp pairs)
- `src/private/mxtest/corert/` — core-roundtrip harness

## Markdown

Do not write malformed markdown. An unclosed `~` (meaning "approximately") is malformed markdown. An
unclosed `_` or `*` is also malformed. If you use these symbols for something other than formatting,
surround them with backticks so they do not create markdown parsing errors.
