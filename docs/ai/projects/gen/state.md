# gen State

## Milestone

**M3: fix-core-dev** in progress. After iteration 1: `make test-core-dev` = 14 failed
(down from 31); `make test` = 0 failed.

## What the previous session did (M3 iteration 1)

Picked the smallest core-roundtrip diff (`foundsuite_Invention 2.xml`, 4 lines:
`width="564.40"` vs `width="564.4"`). Root cause: hand-written
`PreciseDecimal::toStream` in `src/private/mx/core/Decimals.cpp` strips trailing zeros
on tenths-typed values. The test harness already normalizes 9 other tenths fields
via `mxtest::stripZerosFromDecimalFields` (set in
`src/private/mxtest/import/DecimalFields.h`); `width` was missing from the set.

Per user direction, fixed on the test side rather than changing the library's decimal
serialization. Added `"width"` to `decimalFields`. Cleared 17 failures (all driven by
the same pattern across MuseScore/foundsuite files). Committed as `639d46a3`.

## What the next session should do (M3 iteration 2)

Per `plan.md` M3 session sequence:

1. `rm -rf data/testOutput/*`
2. `make test-core-dev` — record failure count as baseline_core_dev (should be 14).
3. `make test` — should be 0.
4. Diff each pair in `data/testOutput/corert` and pick the smallest diff. That is
   `test_to_fix`. Report it to the user with a one-paragraph analysis (root cause:
   generator bug, pre-existing library quirk, etc.). One test per session.
5. Wait for user direction before fixing.
6. After fix: regen `mx/core` if the fix was in `gen/`, `make fmt`,
   `make check`, then re-run baselines.

## Gotchas

- **One test per session.** Do not try to fix multiple failures in one session even
  if they share a root cause; the user wants explicit attention on each. A single
  targeted fix may incidentally clear other failures with the same root cause
  (iteration 1 cleared 17 with a one-line `decimalFields` addition) \u2014 that's
  fine, but the focus and analysis must be on the one chosen test.
- **Don't touch tests carelessly.** The cardinal rule "never change tests" applies
  to test cases. Test infrastructure (normalization helpers, harness code) is
  fair game when the user authorizes; default to flagging before changing.
- **`make test-all` is the M2 gate, `make test-core-dev` is the M3 daily driver.**
  `make test-core-dev` is faster and surfaces the corert failures; `make test`
  (api import + others) must also stay at zero each iteration.
- **HEAD has a hand-applied UpDownNone backport** in `ArpeggiateAttributes.h`
  that conflicts with a schema-faithful regen. As long as M3 changes do not
  require regen this is invisible; if a generator fix is needed, follow M2's
  workflow (`python3 gen/generate.py && make fmt` THEN test, reset after).
- **`make fmt` runs in Docker** and may time out on first pull (registry latency).
  Just retry.
