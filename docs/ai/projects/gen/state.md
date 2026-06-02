# gen State

## Milestone

M6B_DATA_MODEL, is done.

## What the last session did (2026-06-02, M6B session 1)

Stood up `gen/parse.py` and `gen/ids.py` as a pure internal extraction. Zero C++ diff. See `log.md`
2026-06-02 08:15 for detail.

## IMPORTANT correction to the design's self-containment claim

The design said `generate_enums_h` was "the one current violation" reaching into `model.root`. That
was wrong: there were SIX `model.root` users. Two were general-path and are now migrated into parse
(enum docs, complex-content predicate). **Four are bespoke handlers** (harmony-chord, score-wrapper,
music-data, full-note) and still walk `model.root` directly. So `model.tree` is severed but
`model.root` survives, scoped to bespoke-only, until those families migrate. Do not try to delete
`model.root` until the bespoke families are migrated.

## What the next session should do (M6C session 21)

Get your instructions from the user.

## Oracle (how to prove zero diff)

The committed C++ equals `python3 gen/generate.py && make fmt` - raw generator output is unformatted,
so the `make fmt` step is REQUIRED (the M6B prompt's oracle omitted it). Two ways to check, tightest
first:

- Raw-output snapshot: `cp -R src/private/mx/core /tmp/core_before` after a clean generate, make your
  change, regenerate, then `diff -rq /tmp/core_before src/private/mx/core` must be empty. This is
  byte-exact and needs no `make fmt`.
- Committed oracle: `python3 gen/generate.py && make fmt && git diff --quiet src/private/mx/core`.

Then `make test-core-dev`. Reset generated C++ before committing:
`git checkout -- src/private/mx/core` (the refactor must change only `gen/*.py`).

## Gotchas

- `make fmt` (~1 min, Docker) is part of the oracle - the generator emits unformatted C++.
- CI `linux-gate` runs `make gen-quality` (floor 37.7; currently 38.2) and `make gen-lint` (floor
  9.4; currently 9.50). New `gen/*.py` files are scored - keep functions small and add docstrings.
- `gen-quality`/`gen-lint` are otherwise ignored during the refactor (user directive) unless CI
  fails.
- Running `python3 gen/generate.py` works because Python puts `gen/` on `sys.path[0]`, so the bare
  `from parse import ...` / `from ids import ...` resolve.
- `node_id` fields are `compare=False` on purpose; keep it that way so adding IDs never perturbs
  dataclass equality.
