# gen State

## Milestone

**M6A complete.** Generator quality gates (`make gen-quality` + `make gen-lint`) are built, wired
into CI, and gating against a ratcheted floor. Next up: M6B_DATA_MODEL.

## What the last session did (2026-06-01, M6A)

Implemented M6A

## What the next session should do

Implement M6B.

- Proper refactor and code improvement is the goal. `gen-quality` is just a regression detector.
- The generator code should not cause any diff in the C++ code (pure refactor)

## Gotchas

- The score is deterministic only with the pinned analyzer versions (Dockerfile). Bumping them
  shifts the score - re-measure and reset `GEN_QUALITY_FLOOR` if you do.
- `make gen-quality` and `make gen-lint` must not regress at the end of a session.
- `make test-all` is slow (>10 min); iterate on `make test-core-dev` for mx/core changes.
