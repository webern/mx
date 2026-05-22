# gen State

## Milestone

**M3 complete** (2026-05-22). All gates green: `make test-core-dev` 350/350,
`make test` 234/234 (2717 assertions), `make test-all` 3028/3028 (9914 assertions),
`make check` passed. Final M3 commits: `d43a222c`, `9c8efa24`, `6c4e18d4`.
See `log.md` for detail.

## What the next session should do (M4 kickoff)

Start M4: increase test coverage. The corert harness now exists; what's missing
is more MusicXML round-trip input files.

First task is scoping with the user, not coding. Topics:

- Where do additional files come from? Public test suites (Lilypond, MuseScore
  corpora, W3C samples), hand-curated edge cases targeting specific XSD features,
  generated/fuzzed inputs.
- What's the coverage goal? Count of files, or coverage of specific spec features
  not yet exercised? Is there a way to measure XSD-feature coverage of the
  current corpus?
- Where do new files live? Existing convention: subdirectories of `data/`,
  picked up automatically by corert (`CoreRoundtripImpl.cpp::discoverInputFiles`)
  unless excluded (`expected/`, `testOutput/`, `generalxml/`, `smufl/`) or
  marked `.invalid`.

Ask the user before designing.

## Gotchas to carry forward

- **HEAD `UpDownNone` backport** in `ArpeggiateAttributes.h` conflicts with a
  schema-faithful regen. Stays invisible as long as M4 doesn't regen. Lives until
  M5/M6.
- **Hand-written vs generated.** Files directly under `src/private/mx/core/`
  (e.g. `XsString.cpp`, `CommaSeparatedPositiveIntegers.cpp`) are mostly
  hand-written; files under `src/private/mx/core/elements/` are codegen output.
  `gen/generate.py` references hand-written ones by header path only; bugs in
  them are fixed in-place, no regen.
- **`make test-all` is the authoritative gate** for changes touching
  `src/private/mx/core/*`. `make test` skips the slow `mxtest/file/`
  round-trippers. `make test-core-dev` is the daily driver for core-only work.
- **Generator formatting drift.** Even with no semantic gen changes,
  `python3 gen/generate.py` can produce different clang-format line wrapping
  than HEAD. Always `make fmt` after regenerating; expect only the
  semantically-changed files to remain in `git diff` afterwards.
- **Test cases vs test harness.** "Never change tests" applies to test cases.
  Harness code (normalization, discovery, expected-file generation) is fair
  game with user authorization; default to flagging before changing.
- **Validating against MusicXML XSD with `xmllint`.** The schema imports
  `xml.xsd` and `xlink.xsd` from URLs that 404. Workaround:
  `curl -sSf -o /tmp/xml.xsd https://www.w3.org/2001/xml.xsd`,
  `curl -sSf -o /tmp/xlink.xsd https://www.w3.org/1999/xlink.xsd`,
  copy `docs/musicxml.xsd` to `/tmp/mx.xsd` and `sed` the two `schemaLocation`
  values to the local paths, then `xmllint --noout --schema /tmp/mx.xsd <file>`.
- **`.invalid` marker convention.** Sibling file `{file}.invalid` next to any
  MusicXML input that is intentionally not schema-valid. Body is a
  human-readable explanation. Corert skips them; api import does not.
  Documented in `data/README.md`.
- **`make fmt` runs in Docker** and may time out on first pull. Retry.
- **`CHILD_MIN_OCCURS_OVERRIDE` is now empty** but the mechanism remains.
  Useful escape hatch if another hand-rolled "always-present optional"
  surfaces.
