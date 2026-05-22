# gen State

## Milestone

**M3: fix-core-dev** in progress. After iteration 2 + sweep:
`make test-core-dev` = 3 failed (down from 13); `make test` = 0 failed.

## What the previous session did (M3 iteration 2 + invalid-marker sweep)

Iteration 2 picked the smallest core-roundtrip diff
(`musuite/testInvalid.xml`, 4 lines, two non-XSD elements dropped on
`fromXDoc`). Not a bug — the file is intentionally invalid MusicXML and
self-documents that fact. A strongly-typed schema-generated DOM cannot
preserve unknown elements without an architectural passthrough we do not
want.

Per user direction introduced a repo-wide marker convention: a sibling
file `{file}.invalid` next to any invalid MusicXML input, body is a
human-readable explanation. Documented in `data/README.md`. Updated
`src/private/mxtest/corert/CoreRoundtripImpl.cpp::discoverInputFiles` to
skip any input with a sibling `.invalid` marker. Committed as
`b62b663f`.

Sweep: ran xmllint --schema against the MusicXML XSD (with xml.xsd /
xlink.xsd downloaded locally to /tmp and schema imports rewritten in a
working copy at /tmp/mx.xsd) for every failing corert input. For each
file, cross-checked the schema violation against the corert diff
symptom. Marked a file only when the schema violation explains the
round-trip diff. Ten more files got markers; three remained as genuine
library bugs on schema-valid input.

## Three remaining real bugs (candidates for iteration 3+)

These are schema-valid inputs whose round-trip diff is a real library
bug. Pick the smallest next session.

1. `lysuite/ly41e_StaffGroups_InstrumentNames_Linebroken.xml` (10-line
   diff): `part-name` text mismatch. Likely the library normalizing
   linebroken text content.
2. `lysuite/ly45f_Repeats_InvalidEndings.xml` (8-line diff): ending
   `number` attribute `"1, 2, 3"` round-trips as `"1,2,3"`. Despite the
   "Invalid" in the filename, the schema's `ending-number` pattern
   `[1-9][0-9]*(, ?[1-9][0-9]*)*` allows both forms. Library is dropping
   spaces.
3. `lysuite/ly22b_Staff_Notestyles.xml` (24-line diff): child count
   mismatch inside `measure-style` (attributes block, measure[0]).

## What the next session should do (M3 iteration 3)

Per `plan.md` M3 session sequence:

1. `rm -rf data/testOutput/*`
2. `make test-core-dev` — record failure count as baseline_core_dev
   (should be 3).
3. `make test` — should be 0.
4. Diff each pair in `data/testOutput/corert` and pick the smallest diff.
   That is `test_to_fix`. Report it to the user with a one-paragraph
   analysis. One test per session.
5. Wait for user direction before fixing.
6. After fix: regen `mx/core` if the fix was in `gen/`, `make fmt`,
   `make check`, then re-run baselines.

## Gotchas

- **`.invalid` marker convention**: if you encounter a test failure
  caused by intentionally invalid MusicXML input, the right move is
  usually a `{file}.invalid` marker, not a code change. The sweep this
  session covered every file failing under iteration 2's baseline, so a
  *new* corert failure is most likely a real library bug; still confirm
  by reading the file and running xmllint --schema against
  `/tmp/mx.xsd` (or rebuild it from `docs/musicxml.xsd` per the recipe
  below).
- **Validating against the MusicXML XSD with xmllint**: the XSD imports
  `http://www.musicxml.org/xsd/xml.xsd` and `xlink.xsd` which 404 today.
  Workaround used this session:
  - `curl -sSf -o /tmp/xml.xsd https://www.w3.org/2001/xml.xsd`
  - `curl -sSf -o /tmp/xlink.xsd https://www.w3.org/1999/xlink.xsd`
  - copy `docs/musicxml.xsd` to `/tmp/mx.xsd` and `sed` the two
    `schemaLocation` values to point at `/tmp/xml.xsd` and
    `/tmp/xlink.xsd`.
  - then `xmllint --noout --schema /tmp/mx.xsd <file>`.
- **One test per session.** Do not try to fix multiple failures in one
  session even if they share a root cause; the user wants explicit
  attention on each.
- **Don't touch tests carelessly.** The cardinal rule "never change
  tests" applies to test cases. Test infrastructure (normalization
  helpers, harness code, discovery rules) is fair game when the user
  authorizes; default to flagging before changing.
- **`make test-all` is the M2 gate, `make test-core-dev` is the M3 daily
  driver.** `make test-core-dev` is faster and surfaces the corert
  failures; `make test` (api import + others) must also stay at zero
  each iteration.
- **HEAD has a hand-applied UpDownNone backport** in
  `ArpeggiateAttributes.h` that conflicts with a schema-faithful regen.
  As long as M3 changes do not require regen this is invisible; if a
  generator fix is needed, follow M2's workflow (`python3 gen/generate.py
  && make fmt` THEN test, reset after).
- **`make fmt` runs in Docker** and may time out on first pull
  (registry latency). Just retry.
