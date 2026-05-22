# gen State

## Milestone

**M3: fix-core-dev** in progress. After iteration 3:
`make test-core-dev` = 2 failed (down from 3); `make test` = 0 failed;
`make test-all` = 2 failed (3026/3028 cases).

## What the previous session did (M3 iteration 3)

Iteration 3 picked the smallest core-roundtrip diff,
`lysuite/ly45f_Repeats_InvalidEndings.xml` (8 lines). The `ending`
element's `number` attribute was round-tripping `"1, 2, 3"` →
`"1,2,3"`. The XSD `ending-number` pattern
`[1-9][0-9]*(, ?[1-9][0-9]*)*` allows both forms; the library was
losing the lexical choice on import.

Root cause: `CommaSeparatedListOfPositiveIntegers::parse` in
`src/private/mx/core/CommaSeparatedPositiveIntegers.cpp` ran
`onlyAllow(text, "", "1234567890,-")` (strips spaces) before
tokenising and never set `myIsSpacingDesired`, so output always took
the compact form. `toStream` already handled both forms; only import
needed the fix.

The type is hand-written, not generated — `gen/generate.py` only maps
its header path as a dependency. Fix lives directly in the .cpp.

Fix: scan the raw input for digit-comma-space-digit and set
`myIsSpacingDesired` accordingly. Narrow detection (requires digits on
both sides of `", "`) so the existing
`StringsTest::EndingNumber02` case — which feeds junk and expects
`"1,2,3"` out — stays green. Added `#include <cctype>`. Committed as
`461b96d2`.

## Two remaining real bugs

1. `lysuite/ly41e_StaffGroups_InstrumentNames_Linebroken.xml`
   (10-line diff): `part-name` text mismatch. Library normalizing
   linebroken text content. **Next session target.**
2. `lysuite/ly22b_Staff_Notestyles.xml` (24-line diff): child count
   mismatch inside `measure-style` (attributes block, measure[0]).

## What the next session should do (M3 iteration 4)

Per `plan.md` M3 session sequence:

1. `rm -rf data/testOutput/*`
2. `make test-core-dev` — record failure count as baseline_core_dev
   (should be 2).
3. `make test` — should be 0.
4. Diff each pair in `data/testOutput/corert` and pick the smallest.
   That is `test_to_fix` (expected: `ly41e_StaffGroups_...`). Report
   to user with one-paragraph analysis. One test per session.
5. Wait for user direction before fixing.
6. After fix: regen `mx/core` if the fix was in `gen/`, `make fmt`,
   `make check`, then re-run `make test-core-dev` and `make test`. If
   the fix touched anything under `src/private/mx/core/`, also run
   `make test-all` (the AGENTS.md rule).

## Gotchas

- **`.invalid` marker convention**: if you encounter a test failure
  caused by intentionally invalid MusicXML input, the right move is a
  `{file}.invalid` marker, not a code change. The iteration-2 sweep
  covered every then-failing file, so a *new* corert failure is most
  likely a real library bug; confirm by reading the file and running
  xmllint --schema against `/tmp/mx.xsd` (recipe below).
- **Validating against the MusicXML XSD with xmllint**: the XSD
  imports `http://www.musicxml.org/xsd/xml.xsd` and `xlink.xsd` which
  404 today. Workaround:
  - `curl -sSf -o /tmp/xml.xsd https://www.w3.org/2001/xml.xsd`
  - `curl -sSf -o /tmp/xlink.xsd https://www.w3.org/1999/xlink.xsd`
  - copy `docs/musicxml.xsd` to `/tmp/mx.xsd` and `sed` the two
    `schemaLocation` values to `/tmp/xml.xsd` and `/tmp/xlink.xsd`.
  - then `xmllint --noout --schema /tmp/mx.xsd <file>`.
- **One test per session.** Do not try to fix multiple failures in
  one session even if they share a root cause.
- **Hand-written types vs generated types.** Some `mx/core` files
  (e.g. `CommaSeparatedPositiveIntegers.cpp`, `EndingNumber.h`) are
  hand-written and only referenced as a dependency mapping in
  `gen/generate.py`. Bugs in those files are fixed in the .cpp/.h
  directly, no regen. Check whether the file appears under
  `src/private/mx/core/elements/` (generated) vs the parent
  `src/private/mx/core/` (mostly hand-written core types).
- **Don't touch tests carelessly.** The "never change tests" rule
  applies to test cases. Test infrastructure (normalization helpers,
  harness code, discovery rules) is fair game when authorized;
  default to flagging before changing.
- **`make test-all` is the M2 gate, `make test-core-dev` is the M3
  daily driver.** test-core-dev surfaces the corert failures fast;
  `make test` (api import + others) must also stay at zero each
  iteration. If a fix touches anything under
  `src/private/mx/core/`, also run `make test-all` per AGENTS.md.
- **HEAD has a hand-applied UpDownNone backport** in
  `ArpeggiateAttributes.h` that conflicts with a schema-faithful
  regen. As long as M3 changes do not require regen this is
  invisible; if a generator fix is needed, follow M2's workflow.
- **`make fmt` runs in Docker** and may time out on first pull
  (registry latency). Just retry.
