# gen State

## Milestone

M6B_DATA_MODEL, is done.

## What the last session did (2026-06-02, M6B session 1)

Stood up `gen/parse.py` and `gen/ids.py` as a pure internal extraction. Zero C++ diff. See `log.md`
2026-06-02 08:15 for detail. Headline:

- Moved the nine XSD dataclasses, `XsdModel`, and `pascal()` into `gen/parse.py`. `generate.py`
  imports them back. `camel`/`has_flag_name`/`CPP_KEYWORDS` stayed (C++ lexicon).
- Config coupling solved by injection, not by moving config into parse: `generate.py` keeps the
  seven structural-config globals and passes them via a new `ParseConfig` dataclass into
  `XsdModel(xsd_path, cfg)`. The four sets are by-reference, so synthetic groups the parser records
  are visible to emission. Avoids a `generate`<->`parse` import cycle; parse stays config-free.
- Enum docs moved into parse (`model.enum_docs`); `generate_enums_h` reads it. The "pattern B"
  predicate moved in too (`model.complex_content_or_group_cts`); `_ct_has_complex_content` reads it.
- `gen/ids.py` defines `NodeId` (frozen value, canonical `kind:name` / `kind#ordinal` path string).
  `XsdModel._assign_ids` assigns one to every dataclass-backed node. `node_id` fields are
  `field(default=None, compare=False)`. Additive and unconsumed.
- Deleted dead `gen/gen_attrs.py`, `gen/gen_enums.py`, `gen/gen_enum_members.py`.

## IMPORTANT correction to the design's self-containment claim

The design said `generate_enums_h` was "the one current violation" reaching into `model.root`. That
was wrong: there were SIX `model.root` users. Two were general-path and are now migrated into parse
(enum docs, complex-content predicate). **Four are bespoke handlers** (harmony-chord, score-wrapper,
music-data, full-note) and still walk `model.root` directly. So `model.tree` is severed but
`model.root` survives, scoped to bespoke-only, until those families migrate. Do not try to delete
`model.root` until the bespoke families are migrated.

## What the next session should do (M6B session 2)

Migrate the first unit-kind through the new path: **enums**. Per the migration plan in
`design/m6b-data-model.md`:

1. Build an enum context struct (start `gen/contexts.py`) and a pure renderer (start
   `gen/render/`, e.g. `render/enums.py`) that takes the struct and returns the Enums.h string -
   reads only its struct, no `model`/config/XSD.
2. Build the enum contexts in a configure step (decide whether to start `gen/configure.py` now or
   keep the build inline in `generate.py` for one more session - your call, but the renderer must be
   pure).
3. Route only `generate_enums_h` through the new path; leave every other kind on the old path.
4. Verify zero C++ diff (see oracle below) and that `gen-quality`/`gen-lint` still clear their floors.

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
